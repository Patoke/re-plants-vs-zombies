#include "Quantize.h"
#include <assert.h>
#include <math.h>

using namespace Sexy;

#define ColorMatch(p,q) (((p)->red == (q)->red) && \
  ((p)->green == (q)->green) && ((p)->blue == (q)->blue) && ((p)->opacity) == ((q)->opacity))

#define MaxRGB 256

#define MaxTreeDepth  8
#define NodesInAList  1536

#define ExceptionQueueLength  16
#define MaxNodes  266817

// Typdef declarations.

typedef struct _QuantizeInfo
{
  unsigned long
    number_colors;
  
  unsigned int
    tree_depth;      
} QuantizeInfo;

typedef struct _PixelPacket
{
  uchar
    blue,
    green,
    red,
    opacity;
} PixelPacket;

typedef struct _NodeInfo
{
  struct _NodeInfo
    *parent,
    *child[8];

  double
    number_unique,
    total_red,
    total_green,
    total_blue,
    quantize_error;

  unsigned long
    color_number;

  unsigned char
    id,
    level,
    census;
} NodeInfo;

typedef struct _Nodes
{
  NodeInfo
    nodes[NodesInAList];

  struct _Nodes
    *next;
} Nodes;

typedef struct _CubeInfo
{
  NodeInfo
    *root;  

  unsigned long
    colors;

  PixelPacket
    color,
    *colormap;

  double
    distance,
    pruning_threshold,
    next_threshold;

  unsigned long
    nodes,
    free_nodes,
    color_number;

  NodeInfo
    *next_node;

  Nodes
    *node_queue;

  long
    *cache;  

  double
    weights[ExceptionQueueLength];

  const QuantizeInfo
    *quantize_info;

  long
    x,
    y;

  unsigned int
    depth;
} CubeInfo;

/*
  Include declarations.
*/

static void *AcquireMemory(const size_t size)
{
  void
    *allocation;

  assert(size != 0);
  allocation=malloc(size);
  return(allocation);
}

static void *CloneMemory(void *destination, const void *source,
  const size_t size)
{
  register unsigned char
    *q;

  register const unsigned char
    *p;

  register long
    i;

  assert(destination != (void *) NULL);
  assert(source != (const void *) NULL);
  p=(const unsigned char *) source;
  q=(unsigned char *) destination;
  if ((p <= q) || ((p+size) >= q))
    return(memcpy(destination,source,size));
  /*
    Overlap, copy backwards.
  */
  p+=size;
  q+=size;
  for (i=size-1; i >= 0; i--)
    *--q=(*--p);
  return(destination);
}

static void LiberateMemory(void **memory)
{
  assert(memory != (void **) NULL);
  if (*memory == (void *) NULL)
    return;
  free(*memory);
  *memory=(void *) NULL;
}

static void ReacquireMemory(void **memory,const size_t size)
{
  void
    *allocation;

  assert(memory != (void **) NULL);
  if (*memory == (void *) NULL)
    {
      *memory=AcquireMemory(size);
      return;
    }
  allocation=realloc(*memory,size);
  if (allocation == (void *) NULL)
    LiberateMemory((void **) memory);
  *memory=allocation;
}

static NodeInfo* GetNodeInfo(CubeInfo* cube_info, const unsigned int id, const unsigned int level, NodeInfo* parent)
{
	NodeInfo *node_info;

	if (cube_info->free_nodes == 0)
	{
		Nodes* nodes;

		// Allocate a new nodes of nodes.      
		nodes = (Nodes*) AcquireMemory(sizeof(Nodes));
		if (nodes == (Nodes *) NULL)
			return((NodeInfo *) NULL);

		nodes->next = cube_info->node_queue;
		cube_info->node_queue = nodes;
		cube_info->next_node = nodes->nodes;
		cube_info->free_nodes = NodesInAList;
	}

	cube_info->nodes++;
	cube_info->free_nodes--;
	node_info = cube_info->next_node++;
	memset(node_info,0,sizeof(NodeInfo));
	node_info->parent = parent;
	node_info->id = id;
	node_info->level = level;

	return node_info;
}

static CubeInfo* GetCubeInfo(QuantizeInfo* theQuantizeInfo, unsigned int depth)
{
	CubeInfo *cube_info;
	
	// Initialize tree to describe color cube_info.	
	cube_info = (CubeInfo*) AcquireMemory(sizeof(CubeInfo));
	
	if (cube_info == (CubeInfo *) NULL)
		return((CubeInfo *) NULL);
	
	memset(cube_info,0,sizeof(CubeInfo));
	
	if (depth > MaxTreeDepth)
		depth = MaxTreeDepth;

	if (depth < 2)
		depth = 2;
	cube_info->depth = depth;
	
	// Initialize root node.	
	cube_info->root = GetNodeInfo(cube_info,0,0,(NodeInfo *) NULL);

	if (cube_info->root == (NodeInfo *) NULL)
		return((CubeInfo *) NULL);

	cube_info->root->parent=cube_info->root;

	cube_info->quantize_info = theQuantizeInfo;

	return cube_info;
}

static void DestroyCubeInfo(CubeInfo *cube_info)
{
	register Nodes* nodes;  

	//  Release color cube tree storage.
	do
	{
		nodes = cube_info->node_queue->next;
		LiberateMemory((void **) &cube_info->node_queue);
		cube_info->node_queue = nodes;
	} while (cube_info->node_queue != (Nodes *) NULL);  

	LiberateMemory((void **) &cube_info->cache);
	LiberateMemory((void **) &cube_info);
}

static void PruneChild(CubeInfo *cube_info,const NodeInfo *node_info)
{
	NodeInfo *parent;

	register long id;


	// Traverse any children.
	if (node_info->census != 0)
	for (id=0; id < MaxTreeDepth; id++)
		if (node_info->census & (1 << id))
			PruneChild(cube_info,node_info->child[id]);  

	//  Merge color statistics into parent.  
	parent = node_info->parent;
	parent->census &= ~(1 << node_info->id);
	parent->number_unique += node_info->number_unique;
	parent->total_red += node_info->total_red;
	parent->total_green += node_info->total_green;
	parent->total_blue += node_info->total_blue;
	cube_info->nodes--;
}

static void PruneLevel(CubeInfo *cube_info,const NodeInfo *node_info)
{
	register long id;
	// Traverse any children.

	if (node_info->census != 0)
	{
		for (id=0; id < MaxTreeDepth; id++)
		{
			if (node_info->census & (1 << id))
				PruneLevel(cube_info,node_info->child[id]);
		}
	}

	if (node_info->level == cube_info->depth)
		PruneChild(cube_info,node_info);
}

static bool Classification(CubeInfo *cube_info, PixelPacket* thePixelPackets, int theWidth, int theHeight)
{
	double bisect, mid_red, mid_green, mid_blue;

	long count, y;

	NodeInfo* node_info;

	register double blue, green, red;
	register long x;
	register const PixelPacket* p;
	unsigned int index, level, id;

	PixelPacket* aPixelRow = thePixelPackets;

	for (y=0; y < (long) theHeight; y++)
	{
		p = aPixelRow;

		if (cube_info->nodes > MaxNodes)
		{			
			// Prune one level if the color tree is too large.			
			PruneLevel(cube_info,cube_info->root);
			cube_info->depth--;
		}

		for (x=0; x < (long) theWidth; x+=count)
		{			
			// Start at the root and descend the color cube tree.
			
			for (count=1; (x+count) < (long) theWidth; count++)			
				if (!ColorMatch(p, p+count))
					break;			

			index = MaxTreeDepth-1;
			bisect = (MaxRGB+1)/2.0;
			mid_red = MaxRGB/2.0;
			mid_green = MaxRGB/2.0;
			mid_blue = MaxRGB/2.0;
			node_info = cube_info->root;
			for (level=1; level <= cube_info->depth; level++)
			{
				bisect *= 0.5;
				id = (unsigned int) ((((p->red) >> index) & 0x01) << 2 |
					(((p->green) >> index) & 0x01) << 1 |
					(((p->blue) >> index) & 0x01));

				mid_red += id & 4 ? bisect : -bisect;
				mid_green += id & 2 ? bisect : -bisect;
				mid_blue += id & 1 ? bisect : -bisect;

				if (node_info->child[id] == (NodeInfo *) NULL)
				{				
					// Set colors of new node to contain pixel.					
					node_info->census |= (1 << id);
					node_info->child[id] = GetNodeInfo(cube_info,id,level,node_info);
					            
					if (node_info->child[id] == (NodeInfo *) NULL)
					{
						//TODO: Exception
					}

					if (level == cube_info->depth)
						cube_info->colors++;
				}
				
				// Approximate the quantization error represented by this node.				
				node_info = node_info->child[id];
				red = (double) p->red-mid_red;
				green = (double) p->green-mid_green;
				blue = (double) p->blue-mid_blue;
				node_info->quantize_error += count*red*red+count*green*green+count*blue*blue;
				cube_info->root->quantize_error += node_info->quantize_error;

				index--;
			}
			
			// Sum RGB for this leaf for later derivation of the mean cube color.			
			node_info->number_unique += count;
			node_info->total_red += count*p->red;
			node_info->total_green += count*p->green;
			node_info->total_blue += count*p->blue;
			p += count;
		}

		aPixelRow += theWidth;
	}

	return true;
}

static void Reduce(CubeInfo *cube_info,const NodeInfo *node_info)
{
	register unsigned int id;
	
	// Traverse any children.	
	if (node_info->census != 0)
	{
		for (id=0; id < MaxTreeDepth; id++)
		{
			if (node_info->census & (1 << id))		
				Reduce(cube_info,node_info->child[id]);
		}
	}

	if (node_info->quantize_error <= cube_info->pruning_threshold)
	{
		PruneChild(cube_info,node_info);
	}
	else
	{		
		// Find minimum pruning threshold.		
		if (node_info->number_unique > 0)
			cube_info->colors++;
		if (node_info->quantize_error < cube_info->next_threshold)
			cube_info->next_threshold=node_info->quantize_error;
	}
}

static void Reduction(CubeInfo *cube_info,const unsigned long number_colors)
{
	unsigned long span;

	span = cube_info->colors;
	cube_info->next_threshold = 0.0;
	while (cube_info->colors > number_colors)
	{
		cube_info->pruning_threshold = cube_info->next_threshold;
		cube_info->next_threshold = cube_info->root->quantize_error-1;
		cube_info->colors = 0;
		Reduce(cube_info, cube_info->root);    
	}
}

static void ClosestColor(CubeInfo *cube_info,const NodeInfo *node_info)
{
	if (cube_info->distance != 0.0)
	{
		register unsigned int id;
		
		// Traverse any children.		
		if (node_info->census != 0)
		{
			for (id=0; id < MaxTreeDepth; id++)
			{
				if (node_info->census & (1 << id))
					ClosestColor(cube_info,node_info->child[id]);
			}
		}
		
		if (node_info->number_unique != 0)
		{
			double distance;

			register double blue, green, red;

			register PixelPacket *color;
			
			// Determine if this color is "closest".
			
			color = cube_info->colormap+node_info->color_number;
			red = (double) (color->red-cube_info->color.red);
			distance = red*red;
			if (distance < cube_info->distance)
			{
				green = (double) (color->green-cube_info->color.green);
				distance += green*green;
				if (distance < cube_info->distance)
				{
					blue = (double) (color->blue-cube_info->color.blue);
					distance += blue*blue;
					if (distance < cube_info->distance)
					{
						cube_info->distance = distance;
						cube_info->color_number = node_info->color_number;
					}
				}
			}
		}
	}
}

static void DefineColormap(CubeInfo *cube_info,NodeInfo *node_info)
{
	register unsigned int id;

	// Traverse any children.

	if (node_info->census != 0)
	{
		for (id=0; id < MaxTreeDepth; id++)
		{
			if (node_info->census & (1 << id))
				DefineColormap(cube_info,node_info->child[id]);
		}
	}

	if (node_info->number_unique != 0)
	{
		register double number_unique;
		
		// Colormap entry is defined by the mean color in this cube.		
		number_unique = node_info->number_unique;
		cube_info->colormap[cube_info->colors].red = 
			(uchar) ((node_info->total_red+0.5*number_unique)/number_unique);
		cube_info->colormap[cube_info->colors].green = 
			(uchar) ((node_info->total_green+0.5*number_unique)/number_unique);
		cube_info->colormap[cube_info->colors].blue = 
			(uchar) ((node_info->total_blue+0.5*number_unique)/number_unique);
		node_info->color_number = cube_info->colors++;
	}
}

static unsigned int Assignment(CubeInfo *cube_info, PixelPacket* thePixelPackets, int theWidth, int theHeight, uchar* theIndices, PixelPacket* theColorTable)
{
	uchar index;
	long count, y;

	register uchar *indexes;

	register long i, x;

	register const NodeInfo* node_info;

	register PixelPacket* q;

	unsigned int id;
	
	// Allocate image colormap.
	
	cube_info->colormap = theColorTable;
	cube_info->colors = 0;
	DefineColormap(cube_info, cube_info->root);  

	PixelPacket* aPixelRow = thePixelPackets;
	uchar* aIndexRow = theIndices;

	for (y=0; y < (long) theHeight; y++)
	{
		q = aPixelRow;

		indexes=aIndexRow;

		for (x=0; x < (long) theWidth; x+=count)
		{	
			// Identify the deepest node containing the pixel's color.
			
			for (count=1; (x+count) < (long) theWidth; count++)
				if (!ColorMatch(q,q+count))
					break;

			node_info=cube_info->root;
			for (index=MaxTreeDepth-1; (long) index > 0; index--)
			{
				id=(unsigned int) ((((q->red) >> index) & 0x01) << 2 |
					(((q->green) >> index) & 0x01) << 1 |
					(((q->blue) >> index) & 0x01));

				if ((node_info->census & (1 << id)) == 0)
					break;

				node_info = node_info->child[id];
			}
			
			// Find closest color among siblings and their children.
			
			cube_info->color.red = q->red;
			cube_info->color.green = q->green;
			cube_info->color.blue = q->blue;
			cube_info->distance = 3.0*(MaxRGB+1)*(MaxRGB+1);
			ClosestColor(cube_info, node_info->parent);
			index = (unsigned int) cube_info->color_number;

			for (i = 0; i < count; i++)
			{
				indexes[x+i] = index;

				//if (image->storage_class == PseudoClass)
				//indexes[x+i]=index;

				//q->red=theColorTable[index].red;
				//q->green=theColorTable[index].green;
				//q->blue=theColorTable[index].blue;

				q++;
			}
		}

		aPixelRow += theWidth;
		aIndexRow += theWidth;
	}

	return true;
}

void Sexy::Quantize8Bit(const ulong* theSrcBits, int theWidth, int theHeight, uchar* theDestColorIndices, ulong* theDestColorTable)
{
	QuantizeInfo aQuantizeInfo;
	aQuantizeInfo.number_colors = 256;
	aQuantizeInfo.tree_depth = 8;

	CubeInfo *cube_info;

	bool status;

	unsigned long number_colors = aQuantizeInfo.number_colors;  
	unsigned int depth = aQuantizeInfo.tree_depth;

	// Initialize color cube.  
	
	cube_info = GetCubeInfo(&aQuantizeInfo, depth);	

	if (cube_info == (CubeInfo *) NULL)
		return;

	ulong aThing = timeGetTime();
	// 277ms
	status = Classification(cube_info, (PixelPacket*) theSrcBits, theWidth, theHeight);
	int aTiming = timeGetTime() - aThing;

	if (status != false)
	{
		// Reduce the number of colors in the image.
		aThing = timeGetTime();
		// 1437ms
		Reduction(cube_info, number_colors);
		aTiming = timeGetTime() - aThing;

		aThing = timeGetTime();
		// 296 ms
		Assignment(cube_info, (PixelPacket*) theSrcBits, theWidth, theHeight, (uchar*) theDestColorIndices, (PixelPacket*) theDestColorTable);
		aTiming = timeGetTime() - aThing;
	}
	
	DestroyCubeInfo(cube_info);
}



// / / / / //

struct ColorRecord
{	
	ulong					mColor;
	uchar					mIndex;
	double					mPriority;
	ColorRecord*			mNext;
};

const int MAX_COLOR_RECORDS = 60000;
const int HASH_TABLE_SIZE = 16*16*16*16;

ColorRecord gRecordPool[MAX_COLOR_RECORDS];
ColorRecord* gColorRecordsHashTable[HASH_TABLE_SIZE]; // 4 bits each for RGBA

int gSignedPowerTable[512];

void Sexy::Quantize8Bit2(const ulong* theSrcBits, int theWidth, int theHeight, uchar* theDestColorIndices, ulong* theDestColorTable)
{
	int i;

	for (i = 0; i < 512; i++)
		gSignedPowerTable[i] = (i-256) * (i-256);

	for (i = 0; i < HASH_TABLE_SIZE; i++)
		gColorRecordsHashTable[i] = NULL;

	int aSize = 0;

	int aPoolSize = 0;

	// Allocate from the color pool and stuff
	const ulong* aSrcBitsPtr = theSrcBits;	
	for (int aRow = 0; aRow < theHeight; aRow++)
	{
		for (int aCol = 0; aCol < theWidth; aCol++)
		{
			ulong src = *(aSrcBitsPtr++);

			ushort aHashIndex = (ushort) 
				(((src & 0xFF000000) >> (3*8 - 3*4)) |
				 ((src & 0x00FF0000) >> (2*8 - 2*4)) |
				 ((src & 0x0000FF00) >> (1*8 - 1*4)) |
				 ((src & 0x000000FF) >> (0*8 - 0*4)));

			ColorRecord* aColorRecord = gColorRecordsHashTable[aHashIndex];
			if (aColorRecord == NULL)
			{
				if (aPoolSize < MAX_COLOR_RECORDS)
				{
					aColorRecord = &gRecordPool[aPoolSize++];
					aColorRecord->mColor = src;
					aColorRecord->mIndex = 0;
					aColorRecord->mPriority = 1;					
					aColorRecord->mNext = NULL;

					gColorRecordsHashTable[aHashIndex] = aColorRecord;
				}
			}
			else
			{
				for (;;)
				{
					if (aColorRecord->mColor == src)
					{
						aColorRecord->mPriority++;
						break;
					}

					if (aColorRecord->mNext == NULL)
					{
						if (aPoolSize < MAX_COLOR_RECORDS)
						{
							ColorRecord* aNextRecord = &gRecordPool[aPoolSize++];
							aNextRecord->mColor = src;
							aNextRecord->mIndex = 0;
							aNextRecord->mPriority = 1;
							aNextRecord->mNext = NULL;

							aColorRecord->mNext = aNextRecord;
						}

						break;
					}

					aColorRecord = aColorRecord->mNext;
				}				
			}
		}
	}

	int aColorTableSize = 0;	

	while (aColorTableSize < 256)	
	{
		double aBestPriority = 0;
		ColorRecord* aBestColorRecord = NULL;

		// Find record to use
		for (int aPoolIdx = 0; aPoolIdx < aPoolSize; aPoolIdx++)
		{
			ColorRecord* aColorRecord = &gRecordPool[aPoolIdx];
			if (aColorRecord->mPriority > aBestPriority)
			{
				aBestPriority = aColorRecord->mPriority;
				aBestColorRecord = aColorRecord;
			}			
		}

		// Is the color table already completed?
		if (aBestColorRecord == NULL)
			break;

		ulong aBestColor = aBestColorRecord->mColor;

		for (int aPoolIdx = 0; aPoolIdx < aPoolSize; aPoolIdx++)
		{
			ColorRecord* aColorRecord = &gRecordPool[aPoolIdx];
			
			ulong aColor = aColorRecord->mColor;

			ulong aDistance = 
				gSignedPowerTable[((aColor >> 24)       ) - ((aBestColor >> 24)       ) + 256] +
				gSignedPowerTable[((aColor >> 16) & 0xFF) - ((aBestColor >> 16) & 0xFF) + 256] +
				gSignedPowerTable[((aColor >>  8) & 0xFF) - ((aBestColor >>  8) & 0xFF) + 256] +
				gSignedPowerTable[((aColor      ) & 0xFF) - ((aBestColor      ) & 0xFF) + 256];

			aColorRecord->mPriority -= aColorRecord->mPriority / ((aDistance * 0.01) + 1);
		}

		aBestColorRecord->mIndex = aColorTableSize;
		theDestColorTable[aColorTableSize] = aBestColor;
		aColorTableSize++;
	}

	// Assign colors now
	aSrcBitsPtr = theSrcBits;	
	uchar* aDestIndexPtr = theDestColorIndices;
	for (int aRow = 0; aRow < theHeight; aRow++)
	{
		for (int aCol = 0; aCol < theWidth; aCol++)
		{
			ulong src = *(aSrcBitsPtr++);

			ushort aHashIndex = (ushort) 
				(((src & 0xFF000000) >> (3*8 - 3*4)) |
				 ((src & 0x00FF0000) >> (2*8 - 2*4)) |
				 ((src & 0x0000FF00) >> (1*8 - 1*4)) |
				 ((src & 0x000000FF) >> (0*8 - 0*4)));

			ulong aClosestDistance = 0x7FFFFFFF;
			uchar aClosestIndex = 0;

			for (int aColorIdx = 0; aColorIdx < aColorTableSize; aColorIdx++)
			{
				ulong aColor = theDestColorTable[aColorIdx];
														
				ulong aDistance = 
					gSignedPowerTable[((aColor >> 24)       ) - ((src >> 24)       ) + 256] +
					gSignedPowerTable[((aColor >> 16) & 0xFF) - ((src >> 16) & 0xFF) + 256] +
					gSignedPowerTable[((aColor >>  8) & 0xFF) - ((src >>  8) & 0xFF) + 256] +
					gSignedPowerTable[((aColor      ) & 0xFF) - ((src      ) & 0xFF) + 256];

				if (aDistance == 0)
				{
					aClosestDistance = 0;
					aClosestIndex = aColorIdx;
					break;
				}
				else if (aDistance < aClosestDistance)
				{
					aClosestDistance = aDistance;
					aClosestIndex = aColorIdx;
				}				
			}

			/*ushort aHashIndex = (ushort) 
				(((src & 0xFF000000) >> (3*8 - 3*4)) |
				 ((src & 0x00FF0000) >> (2*8 - 2*4)) |
				 ((src & 0x0000FF00) >> (1*8 - 1*4)) |
				 ((src & 0x000000FF) >> (0*8 - 0*4)));

			ulong aClosestDistance = 0x7FFFFFFF;
			uchar aClosestIndex = 0;

			ColorRecord* aColorRecord = gColorRecordsHashTable[aHashIndex];
			
			while (aColorRecord != NULL)
			{
				ulong aColor = aColorRecord->mColor;
														
				ulong aDistance = 
					gSignedPowerTable[((aColor >> 24)       ) - ((src >> 24)       ) + 256] +
					gSignedPowerTable[((aColor >> 16) & 0xFF) - ((src >> 16) & 0xFF) + 256] +
					gSignedPowerTable[((aColor >>  8) & 0xFF) - ((src >>  8) & 0xFF) + 256] +
					gSignedPowerTable[((aColor      ) & 0xFF) - ((src      ) & 0xFF) + 256];

				if (aDistance == 0)
				{
					aClosestDistance = 0;
					aClosestIndex = aColorRecord->mIndex;
					break;
				}
				else if (aDistance < aClosestDistance)
				{
					aClosestDistance = aDistance;
					aClosestIndex = aColorRecord->mIndex;
				}

				aColorRecord = aColorRecord->mNext;
			}*/
			
			*(aDestIndexPtr++) = aClosestIndex;			
		}
	}	
}


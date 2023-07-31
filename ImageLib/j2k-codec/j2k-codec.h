
#ifndef __J2K_LIBRARY__
#define __J2K_LIBRARY__


#ifdef J2K_CODEC_EXPORTS

 #ifdef J2K_CODEC_STATIC
	#define J2K_CODEC_API(ret) extern "C" ret __stdcall
 #else
	#define J2K_CODEC_API(ret) extern "C" __declspec(dllexport) ret __stdcall
 #endif	

#else

 #ifdef __cplusplus

  #include <windows.h> // Required for resources support in C++ Wrapper

  #ifdef J2K_CODEC_STATIC
	#define J2K_CODEC_API(ret) extern "C" ret __stdcall
  #else	
	#define J2K_CODEC_API(ret) extern "C" __declspec(dllimport) ret __stdcall
  #endif

 #else // plain C

  #ifdef J2K_CODEC_STATIC
	#define J2K_CODEC_API(ret) extern ret __stdcall
  #else	
	#define J2K_CODEC_API(ret) __declspec(dllimport) ret __stdcall
  #endif

 #endif

 enum J2K_ErrorList
 {
	 J2KERR_SUCCESS,
	 J2KERR_NEED_MMX,
	 J2KERR_NOT_ENOUGH_MEMORY,
	 J2KERR_CORRUPTED_DATA,
	 J2KERR_PIPE_FAILURE,
	 J2KERR_INVALID_ARGUMENT,
	 J2KERR_CANCELED,
	 J2KERR_CANT_OPEN_FILE,
	 J2KERR_OPTION_UNRECOGNIZED,
	 J2KERR_NO_SUCH_TILE,
	 J2KERR_NO_SUCH_RESOLUTION,
	 J2KERR_BPP_TOO_SMALL,
	 J2KERR_BUFFER_TOO_SMALL,
	 J2KERR_NOT_PART1_FORMAT,
	 J2KERR_IMAGE_IS_TOO_LARGE,
	 J2KERR_TOO_MANY_RES_LEVEL,
	 J2KERR_TOO_LARGE_CODEBLOCKS,
	 J2KERR_NO_LAZINESS,
	 J2KERR_NO_VCAUSAL,
	 J2KERR_TOO_MANY_COMPONENTS,
	 J2KERR_ONLY_8_BIT_COMPONENTS,
	 J2KERR_ONLY_UNSIG_COMPONENTS,
	 J2KERR_DOWNSAMPLED_COMPONENTS,
	 J2KERR_ROI_NOT_SUPPORTED,
	 J2KERR_PROGR_CHANGE_NOT_SUP,
	 J2KERR_PACKET_HDRS_NOT_SUP,
	 J2KERR_64BIT_BOXES_NOT_SUP,
	 J2KERR_INTERNAL_ERROR
 };

#endif


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_getVersion()
//
// Returns codec version and build number in the 0x105678 form, where 10 is a version (1.0)
// and 5678 is a build number.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(int) J2K_getVersion();


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_getLastError()
//                                                                                      
// Returns the code of last error. Use J2K_getErrorStr() to get textual description of
// the error.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(int) J2K_getLastError();


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_getErrorStr()
//                                                                                      
// Returns the textual description of an error by the error code obtained from getLastError().
// If the errCode is < 0 or > Max number of errors, the "<unknown>" string is returned.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(const char*) J2K_getErrorStr(int errCode);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_getLastErrorStr()
//                                                                                      
// Returns the textual description of the last error.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(const char*) J2K_getLastErrorStr();


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_Unlock()
//
// This function unlocks the full functionality of J2K-Codec. After you have purchased your
// personal registration key, you need to pass it to this function.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(void) J2K_Unlock(const char *key);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_StartLogging()
//                                                                                      
// This function creates a log-file and starts logging debug messages of J2K-Codec.
// 'level' can be 0 (Normal logging) or 1 (Detailed logging).
// Passing 'append' = 1 allows to append the log session to existing log-file.
// Returns 0 if there was no error.
// NOTES: 
//  1. Log-file name is 'j2k-codec.log'
//  2. The performance will degrade significantly if the logging is on.
/////////////////////////////////////////////////////////////////////////////////////////*/

#define J2K_LOG_NORMAL		0
#define J2K_LOG_DETAILED	1
#define J2K_LOG_ALL		    J2K_LOG_DETAILED

#define J2K_LOG_APPEND		1

J2K_CODEC_API(int) J2K_StartLogging(int level, int append);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_StopLogging()
//                                                                                      
// Use this function to stop logging, initiated by J2K_StartLogging().
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(void) J2K_StopLogging();


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_Open()
//                                                                                      
// Returns pointer to an internal class, representing the J2K image, or 0 if
// there was an error. Use J2K_getLastError() to obtain the error code.
/////////////////////////////////////////////////////////////////////////////////////////*/

typedef struct
{
	int  (__stdcall *seek)  (void *data_source, int offset);
	int  (__stdcall *read)  (void *ptr, int size, void *data_source);
	void (__stdcall *close) (void *data_source);
}
J2K_Callbacks;

J2K_CODEC_API(void*) J2K_OpenFile(const char *filename);
J2K_CODEC_API(void*) J2K_OpenMemory(unsigned char *src_buffer, int src_size);
J2K_CODEC_API(void*) J2K_OpenCustom(void *data_source, J2K_Callbacks *cbs);

#ifdef __cplusplus
	__inline void* J2K_Open(const char *filename){ return J2K_OpenFile(filename); }
	__inline void* J2K_Open(unsigned char *src_buffer, int src_size){ return J2K_OpenMemory(src_buffer, src_size); }
	__inline void* J2K_Open(void *data_source, J2K_Callbacks *cbs){ return J2K_OpenCustom(data_source, cbs); }
#endif


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetInfo()
//                                                                                      
// Gets various information about the image.
// Returns J2KERR_SUCCESS or error code if function failed.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(int) J2K_GetInfo(void *image, int *width, int *height, int *components);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetInfoEx()
//                                                                                      
// Gets extended information about the image.
// Returns J2KERR_SUCCESS or error code if function failed.
/////////////////////////////////////////////////////////////////////////////////////////*/

typedef struct
{
	int Version;         // J2K-Codec version as returned by J2K_getVersion()
	int Width, Height;   // Width and height of the image in pixels
	int Components;      // The number of components in the image
	int FileType;        // The image file type (0 = Lossy J2K, 1 = Lossy JP2, 2 = Lossless J2K, 3 = Lossless JP2)
	int hTiles, vTiles;  // The number of tiles in horizontal and vertical directions
}
J2K_Info;

J2K_CODEC_API(int) J2K_GetInfoEx(void *image, J2K_Info *info);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetMetaData()
//                                                                                      
// Gets meta data, embedded into the JP2 image.
// Returns J2KERR_SUCCESS or error code if function failed.
/////////////////////////////////////////////////////////////////////////////////////////*/

enum JP2_META_DATA_TYPES
{
 JP2_METADATA_COMMENT_STR,
 JP2_METADATA_COMMENT_BIN,
 JP2_METADATA_GEOTIFF,
 JP2_METADATA_XML,
 JP2_METADATA_URL,
 JP2_METADATA_PAL,
 JP2_METADATA_ICC,
 JP2_METADATA_UNKNOWN 
};

J2K_CODEC_API(int) J2K_GetMetaData(void *image, int *no, int *type, unsigned char **data, int *size);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_SelectTiles()
//                                                                                      
// Selects or unselects a tile or a tile range, depending on 'action'.
// If action!=0 then the tiles are selected for decoding, otherwise - unselected.
// The range is defined by start and end tile numbers (inclusive) in the raster order.
// If end_tile==-1 then the max tile number will be used instead.
// Returns J2KERR_SUCCESS or error code if function failed.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(int) J2K_SelectTiles(void *image, int start_tile, int end_tile, int action);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetResolutions()
//                                                                                      
// Gets the number of available resolution levels.
// Returns J2KERR_SUCCESS or error code if function failed.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(int) J2K_GetResolutions(void *image, int *resolutions);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetResolutionDimensions()
//                                                                                      
// Gets the dimensions of a resolution level.
// Returns J2KERR_SUCCESS or error code if function failed.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(int) J2K_GetResolutionDimensions(void *image, int res_level, int *width, int *height);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_Decode()
//                                                                                      
// Decodes the image, previously created with J2K_Open().
// If 'buffer' is null, then the required amount of memory is allocated and its size returned
// through the 'size' argument.
// 'buff_pitch' is a buffer pitch - distance, in bytes, to the start of next line.
/////////////////////////////////////////////////////////////////////////////////////////*/

#ifdef __cplusplus
 J2K_CODEC_API(int) J2K_Decode(void *image, unsigned char **buffer, int *size, char *options=0, int *pitch=0); 
#else
 J2K_CODEC_API(int) J2K_Decode(void *image, unsigned char **buffer, int *size, char *options, int *pitch); 
#endif


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_EasyDecode()
//
// Decodes JPEG2000 file into memory buffer.
// Buffer, image width, height and the number of components are passed back via specified pointers.
// If succeeded - returns pointer to an internal structure, which represents the image, otherwise - NULL.
// NOTE:
//  1. You must not free the buffer yourself - use J2K_Close(j2k_image) instead!
//  2. Unlike C++ Wrapper's easyDecode(), which allows to use the user buffer, C EasyDecode() doesn't.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(void*) J2K_EasyDecode(char *filename, unsigned char **buffer, int *width, int *height, int *components, char *options);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_Cancel()
//                                                                                      
// Cancels J2K_Decode() operations.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(void) J2K_Cancel(void *image);


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_Close()
//                                                                                      
// Destroys the image, previously created by J2K_Open(). All images must be closed using
// this function to avoid memory leaks.
/////////////////////////////////////////////////////////////////////////////////////////*/

J2K_CODEC_API(void) J2K_Close(void *image);


/*/////////////////////////////////////////////////////////////////////////////////////////
//
// C++ Wrapper
//
/////////////////////////////////////////////////////////////////////////////////////////*/


#ifndef J2K_CODEC_EXPORTS

#ifdef __cplusplus

// UNDOCUMENTED! You must never use this function directly!
J2K_CODEC_API(int) J2K_GetResolutionDimensionsByStr(void *image, char *options, int *width, int *height); 

/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K_Image
/////////////////////////////////////////////////////////////////////////////////////////*/

class J2K_Image
{
 protected:

   void *image;

  int  error(int code){ errStr=J2K_getErrorStr(code); return code; }
  void reset(){ image=0; width=height=components=0; hTiles=vTiles=0; fileType=0; errStr=""; buffer=0; size=0; pitch=0; }
  int _open()
  {
   if(!image) return error(J2K_getLastError());

   J2K_Info info; int j2k_err=J2K_GetInfoEx(image, &info); if(j2k_err!=J2KERR_SUCCESS){ close(); return error(j2k_err); }

   width  = info.Width;  height = info.Height; components = info.Components;
   hTiles = info.hTiles; vTiles = info.vTiles; fileType   = info.FileType; 

   return J2KERR_SUCCESS;
  }

 public:

   int width, height, components, hTiles, vTiles, fileType; const char *errStr;

   unsigned char *buffer; int size, pitch;

  J2K_Image(){ reset(); }

  int  open(char *filename){ if(image) close(); image=J2K_Open(filename); return _open(); }
  int  open(unsigned char *src_buffer, int src_size){ if(image) close(); image=J2K_Open(src_buffer, src_size); return _open(); }
  int  open(int resource_no)
  {
   if(image) close(); 

   HRSRC hRes  = FindResource(NULL, MAKEINTRESOURCE(resource_no), RT_RCDATA); if(!hRes) return error(J2KERR_CANT_OPEN_FILE);
   HGLOBAL res = LoadResource(NULL, hRes);   if(!res)      return error(J2KERR_CANT_OPEN_FILE);
   int res_size= SizeofResource(NULL, hRes); if(!res_size) return error(J2KERR_CANT_OPEN_FILE);

   unsigned char *res_data = (unsigned char*)LockResource(res);
 
   image=J2K_Open(res_data, res_size); return _open(); 
  }

  int  open(void *data_source, J2K_Callbacks *cbs){ if(image) close(); image=J2K_Open(data_source, cbs); return _open(); }

  int  getMetaData(int *no, int *type, unsigned char **data, int *size)
  {
   int j2k_err = J2K_GetMetaData(image, no, type, data, size);

   if(j2k_err!=J2KERR_SUCCESS) return error(j2k_err); else return J2KERR_SUCCESS;
  }

  int  selectTiles(int start_tile, int end_tile, int action)
  {
   int j2k_err = J2K_SelectTiles(image, start_tile, end_tile, action); 

   if(j2k_err!=J2KERR_SUCCESS) return error(j2k_err); else return J2KERR_SUCCESS;
  }

  int  decode(char *options=0)
  {
   if(!image) return error(J2KERR_SUCCESS); // Do nothing if there's nothing to do
  
   int j2k_err=J2K_GetResolutionDimensionsByStr(image, options, &width, &height);
   if(j2k_err==J2KERR_SUCCESS || j2k_err==J2KERR_NO_SUCH_RESOLUTION)
   {
    j2k_err=J2K_Decode(image, &buffer, &size, options, &pitch); if(j2k_err==J2KERR_SUCCESS) return J2KERR_SUCCESS;
   }

   close(); return error(j2k_err);
  }

  int  easyDecode(char *filename, char *options=0)
  {
   int j2k_err=open(filename); if(j2k_err==J2KERR_SUCCESS) return decode(options); else return error(j2k_err);
  }

  int  easyDecode(unsigned char *src_buffer, int src_size, char *options=0)
  {
   int j2k_err=open(src_buffer, src_size); if(j2k_err==J2KERR_SUCCESS) return decode(options); else return error(j2k_err);
  }

  int  easyDecode(int resource_no, char *options=0)
  {
   int j2k_err=open(resource_no); if(j2k_err==J2KERR_SUCCESS) return decode(options); else return error(j2k_err);
  }

  void close(){ if(image) J2K_Close(image); reset(); }

 ~J2K_Image(){ close(); }
};


/*/////////////////////////////////////////////////////////////////////////////////////////
// J2K Frames
/////////////////////////////////////////////////////////////////////////////////////////*/

#include <string.h>

class J2K_Frames
{
 protected:

   int **frame, currFrame, TotalFrames; bool isAlpha;

  int  error(int code){ errStr=J2K_getErrorStr(code); return code; }
  void reset(){ frame=0; currFrame=0; TotalFrames=0; isAlpha=false; errStr=""; }
  int _open(void *image, char *options)
  {
	close(); if(!image) return error(J2K_getLastError());

	J2K_Info info; char new_options[128]; // char *new_options = (char*)alloca(strlen(options)+10);

	int j2k_err = J2K_GetInfoEx(image, &info); if(j2k_err!=J2KERR_SUCCESS){ close(); return error(j2k_err); }

	isAlpha = (info.Components==4);

	TotalFrames = info.hTiles * info.vTiles;
	frame = new int * [TotalFrames]; if(!frame) return error(J2KERR_NOT_ENOUGH_MEMORY);

	for(int i=0; i<TotalFrames; i++)
	{
		frame[i] = NULL; 

		J2K_SelectTiles(image, 0,-1, 0); J2K_SelectTiles(image, i,i, 1); // Unselect all tiles and select current tile

		int tileWidth, tileHeight;

		if(J2K_GetResolutionDimensionsByStr(image, options, &tileWidth, &tileHeight) == J2KERR_SUCCESS)
		{
			frame[i] = new int[tileWidth*tileHeight + 3];
			if(frame[i])
			{
				frame[i][0] = tileWidth; frame[i][1] = tileHeight; frame[i][2] = isAlpha ? 1 : 0;

				unsigned char *buffer = (unsigned char*)(frame[i]+3);
				int size = tileWidth*tileHeight*4;

				strcpy(new_options, options); strcat(new_options, ",bpp=4"); // Override user's bpp, because frames have always bpp = 4

				j2k_err = J2K_Decode(image, &buffer, &size, new_options); if(j2k_err!=J2KERR_SUCCESS){ delete frame[i]; frame[i]=NULL; }
			}
		}
	}

	J2K_Close(image); return J2KERR_SUCCESS;
  }

  void inc(){ currFrame++; if(currFrame>=TotalFrames) currFrame=0;  }
  void dec(){ currFrame--; if(currFrame<0) currFrame=TotalFrames-1; }

 public:

   const char *errStr;

  J2K_Frames(){ reset(); }

  int open(char *filename, char *options){ return _open(J2K_Open(filename), options); }
  int open(unsigned char *src_buffer, int src_size, char *options){ return _open(J2K_Open(src_buffer, src_size), options); }
  int open(void *data_source, J2K_Callbacks *cbs, char *options){   return _open(J2K_Open(data_source, cbs), options); }
  int open(int resource_no, char *options)
  {
   HRSRC hRes  = FindResource(NULL, MAKEINTRESOURCE(resource_no), RT_RCDATA); if(!hRes) return error(J2KERR_CANT_OPEN_FILE);
   HGLOBAL res = LoadResource(NULL, hRes);   if(!res)      return error(J2KERR_CANT_OPEN_FILE);
   int res_size= SizeofResource(NULL, hRes); if(!res_size) return error(J2KERR_CANT_OPEN_FILE);

   unsigned char *res_data = (unsigned char*)LockResource(res); return _open(J2K_Open(res_data, res_size), options); 
  }

  __inline int getFrames(){ return TotalFrames; }

  __inline unsigned char* getFrame(int no)
  {
   if(no<0) no=0; if(no>=TotalFrames) no=TotalFrames-1; 
  
   if(frame) return (unsigned char*)frame[no]; else return NULL;
  }

  __inline unsigned char* operator()(int no){ return getFrame(no); }
  __inline unsigned char* operator()(){ return getFrame(currFrame); }

  __inline void operator=(int no)
  {
   if(no<0) no=0; if(no>=TotalFrames) no=TotalFrames-1; 
  
   currFrame=no; 
  }

  __inline void operator++(   ){ inc(); }
  __inline void operator++(int){ inc(); }

  __inline void operator--(   ){ dec(); }
  __inline void operator--(int){ dec(); }

  void close(){ if(frame){ for(int i=0; i<TotalFrames; i++) if(frame[i]) delete frame[i];   delete frame; } reset(); }

 ~J2K_Frames(){ close(); }
};

#endif

#endif

/* ///////////////////////////////////////////////////////////////////////////////////// */

#endif

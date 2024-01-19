#include <memory>
#define XMD_H

#include "ImageLib.h"
#include "png/png.h"
#include <math.h>
#include "paklib/PakInterface.h"
#include "Common.h"

extern "C"
{
#include "jpeg/jpeglib.h"
#include "jpeg/jerror.h"
}

using namespace ImageLib;

//////////////////////////////////////////////////////////////////////////
// PNG Pak Support

static void png_pak_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_size_t check;

	/* fread() returns 0 on error, so it is OK to store this in a png_size_t
	* instead of an int, which is what fread() actually returns.
	*/
	check = (png_size_t)p_fread(data, (png_size_t)1, length,
		(PFILE*)png_get_io_ptr(png_ptr));

	if (check != length)
	{
		png_error(png_ptr, "Read Error");
	}
}

std::unique_ptr<Image> GetPNGImage(const std::string& theFileName)
{
	png_structp png_ptr;
	png_infop info_ptr;
	//unsigned int sig_read = 0;
	png_uint_32 width, height;
	//int bit_depth, color_type, interlace_type;
	PFILE *fp;

	if ((fp = p_fopen(theFileName.c_str(), "rb")) == NULL)
		return NULL;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);
	png_set_read_fn(png_ptr, (png_voidp)fp, png_pak_read_data);

	if (png_ptr == NULL)
	{
		p_fclose(fp);
		return NULL;
	}

	/* Allocate/initialize the memory for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		p_fclose(fp);
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return NULL;
	}

	 /* Set error handling if you are using the setjmp/longjmp method (this is
		* the normal method of doing things with libpng).  REQUIRED unless you
		* set up your own error handlers in the png_create_read_struct() earlier.
		*/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		/* Free all of the memory associated with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		p_fclose(fp);
		/* If we get here, we had a problem reading the file */
		return NULL;
	}

	//png_init_io(png_ptr, fp);

	//png_ptr->io_ptr = (png_voidp)fp;

	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, NULL, NULL,
			 NULL, NULL, NULL);

	/* Add filler (or alpha) byte (before/after each RGB triplet) */
	png_set_expand(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	//png_set_gray_1_2_4_to_8(png_ptr);
	png_set_palette_to_rgb(png_ptr);
	png_set_gray_to_rgb(png_ptr);
	png_set_bgr(png_ptr);

//  int aNumBytes = png_get_rowbytes(png_ptr, info_ptr) * height / 4;
	png_bytep row_pointers[height];
	std::unique_ptr<uint32_t[]> aBits = std::make_unique<uint32_t[]>(width*height);
	for (uint i = 0; i < height; i++) {
		row_pointers[i] = (png_bytep)(aBits.get() + i*width);
	}
	png_read_image(png_ptr, row_pointers);
	/*
	unsigned long* anAddr = aBits;
	for (unsigned int i = 0; i < height; i++)
	{
		png_read_rows(png_ptr, (png_bytepp) &anAddr, NULL, 1);
		anAddr += width;
	}*/

	/* read rest of file, and get additional chunks in info_ptr - REQUIRED */
	png_read_end(png_ptr, info_ptr);

	/* clean up after the read, and free any memory allocated - REQUIRED */
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	/* close the file */
	p_fclose(fp);

	std::unique_ptr<Image> anImage = std::make_unique<Image>(width, height);
	anImage->mWidth = width;
	anImage->mHeight = height;
	anImage->mBits = std::move(aBits);

	return anImage;
}

std::unique_ptr<Image> GetTGAImage(const std::string& theFileName)
{
	PFILE* aTGAFile = p_fopen(theFileName.c_str(), "rb");
	if (aTGAFile == NULL)
		return NULL;

	BYTE aHeaderIDLen;
	p_fread(&aHeaderIDLen, sizeof(BYTE), 1, aTGAFile);

	BYTE aColorMapType;
	p_fread(&aColorMapType, sizeof(BYTE), 1, aTGAFile);
	
	BYTE anImageType;
	p_fread(&anImageType, sizeof(BYTE), 1, aTGAFile);

	WORD aFirstEntryIdx;
	p_fread(&aFirstEntryIdx, sizeof(WORD), 1, aTGAFile);

	WORD aColorMapLen;
	p_fread(&aColorMapLen, sizeof(WORD), 1, aTGAFile);

	BYTE aColorMapEntrySize;
	p_fread(&aColorMapEntrySize, sizeof(BYTE), 1, aTGAFile);  

	WORD anXOrigin;
	p_fread(&anXOrigin, sizeof(WORD), 1, aTGAFile);

	WORD aYOrigin;
	p_fread(&aYOrigin, sizeof(WORD), 1, aTGAFile);

	WORD anImageWidth;
	p_fread(&anImageWidth, sizeof(WORD), 1, aTGAFile);  

	WORD anImageHeight;
	p_fread(&anImageHeight, sizeof(WORD), 1, aTGAFile); 

	BYTE aBitCount = 32;
	p_fread(&aBitCount, sizeof(BYTE), 1, aTGAFile); 

	BYTE anImageDescriptor = 8 | (1<<5);
	p_fread(&anImageDescriptor, sizeof(BYTE), 1, aTGAFile);

	if ((aBitCount != 32) ||
		(anImageDescriptor != (8 | (1<<5))))
	{
		p_fclose(aTGAFile);
		return NULL;
	}

	std::unique_ptr<Image> anImage = std::make_unique<Image>(anImageWidth, anImageHeight);

	p_fread(anImage->mBits.get(), 4, anImage->mWidth*anImage->mHeight, aTGAFile);

	p_fclose(aTGAFile);

	return anImage;
}

int ReadBlobBlock(PFILE* fp, char* data)
{
	unsigned char aCount = 0;
	p_fread(&aCount, sizeof(char), 1, fp);
	p_fread(data, sizeof(char), aCount, fp);
	return aCount;
}

std::unique_ptr<Image> GetGIFImage(const std::string& theFileName)
{
	#define BitSet(byte,bit)  (((byte) & (bit)) == (bit))
	#define LSBFirstOrder(x,y)  (((y) << 8) | (x))

	int
		opacity,
		status;

	int i;

	unsigned char *p;

	unsigned char
		background,     // 背景色在全局颜色列表中的索引（背景色：图像中没有被指定颜色的像素会被背景色填充）
		c,
		flag,       // 图像标志的压缩字节
		*global_colormap, // 全局颜色列表
		header[1664],
		magick[12];

	unsigned int
		delay,
		dispose,
		global_colors,    // 全局颜色列表大小
		image_count,
		iterations;

	/*
	Open image file.
	*/

	PFILE *fp;

	if ((fp = p_fopen(theFileName.c_str(), "rb")) == NULL)
		return NULL;
	/*
	Determine if this is a GIF file.
	*/
	status = p_fread(magick, sizeof(char), 6, fp);  // 读取文件头（包含文件签名与版本号，共 6 字节）
	(void)status; // unused

	// 文件头的 ASCII 值为“GIF87a”或”GIF89a”，其中前三位为 GIF 签名，后三位为不同年份的版本号
	if (((strncmp((char*)magick, "GIF87", 5) != 0) && (strncmp((char*)magick, "GIF89", 5) != 0)))
		return NULL;

	global_colors = 0;
	global_colormap = (unsigned char*)NULL;

	short pw;  // 图像宽度
	short ph;  // 图像高度

	// 读取逻辑屏幕描述符，共 7 字节
	p_fread(&pw, sizeof(short), 1, fp);  // 读取图像渲染区域的宽度
	p_fread(&ph, sizeof(short), 1, fp);  // 读取图像渲染区域的高度
	p_fread(&flag, sizeof(char), 1, fp);  // 读取图像标志
	p_fread(&background, sizeof(char), 1, fp);  // 读取背景色在全局颜色列表中的索引，若无全局颜色列表则此字节无效
	p_fread(&c, sizeof(char), 1, fp);  // 读取像素宽高比

	if (BitSet(flag, 0x80))  // 如果存在全局颜色列表
	{
		/*
		opacity global colormap.
		*/
		global_colors = 1 << ((flag & 0x07) + 1);  // 压缩字节的最低 3 位表示全局颜色列表的大小，设其二进制数值为 N，则列表大小 = 2 ^ (N + 1)
		global_colormap = new unsigned char[3 * global_colors];  // 每个颜色占 3 个字节，按 RGB 排列
		if (global_colormap == (unsigned char*)NULL)
			return NULL;

		p_fread(global_colormap, sizeof(char), 3 * global_colors, fp);  // 读取全局颜色列表
	}

	delay = 0;
	dispose = 0;
	iterations = 1;
	opacity = (-1);
	image_count = 0;

	for (; ; )
	{
		if (p_fread(&c, sizeof(char), 1, fp) == 0)
			break;  // 如果读取错误或读取到文件尾则退出，返回空指针

		if (c == ';')  // 当读取到 gif 结束块标记符（End Of File）
			break;  /* terminator */
		if (c == '!')  // 当读取到 gif 拓展块标记符
		{
			/*
			GIF Extension block.
			*/
			p_fread(&c, sizeof(char), 1, fp);  // 读取拓展块的功能编码号

			switch (c)
			{
			case 0xf9:
			{
				/*
				Read Graphics Control extension.
				*/
				while (ReadBlobBlock(fp, (char*)header) > 0);

				dispose = header[0] >> 2;
				delay = (header[2] << 8) | header[1];
				(void)delay; // Unused
				if ((header[0] & 0x01) == 1)
					opacity = header[3];
				break;
			}
			case 0xfe:
			{
				char* comments;
				int length;

				/*
				Read Comment extension.
				*/
				comments = (char*)NULL;
				for (; ; )
				{
					length = ReadBlobBlock(fp, (char*)header);
					if (length <= 0)
						break;
					if (comments == NULL)
					{
						comments = new char[length + 1];
						if (comments != (char*)NULL)
							*comments = '\0';
					}

					header[length] = '\0';
					strcat(comments, (char*)header);
				}
				if (comments == (char*)NULL)
					break;

				delete comments;
				break;
			}
			case 0xff:
			{
				int
					loop;

				/*
				Read Netscape Loop extension.
				*/
				loop = false;
				if (ReadBlobBlock(fp, (char*)header) > 0)
					loop = !strncmp((char*)header, "NETSCAPE2.0", 11);
				while (ReadBlobBlock(fp, (char*)header) > 0)
					if (loop)
						iterations = (header[2] << 8) | header[1];
				break;
			}
			default:
			{
				while (ReadBlobBlock(fp, (char*)header) > 0);
				break;
			}
			}
		}

		if (c != ',')  // 如果读取的不为图像描述符
			continue;

		if (image_count != 0)
		{
			/*
			Allocate next image structure.
			*/

			/*AllocateNextImage(image_info,image);
			if (image->next == (Image *) NULL)
			{
			DestroyImages(image);
			return((Image *) NULL);
			}
			image=image->next;
			MagickMonitor(LoadImagesText,TellBlob(image),image->filesize);*/
		}
		image_count++;

		short pagex;
		short pagey;
		short width;
		short height;
		int colors;
		bool interlaced;

		p_fread(&pagex, sizeof(short), 1, fp);  // 读取帧的横坐标（Left）
		p_fread(&pagey, sizeof(short), 1, fp);  // 读取帧的纵坐标（Top）
		p_fread(&width, sizeof(short), 1, fp);  // 读取帧的横向宽度（Width）
		p_fread(&height, sizeof(short), 1, fp);  // 取得帧的纵向高度（Height）
		p_fread(&flag, sizeof(char), 1, fp);  // 读取帧标志的压缩字节

		colors = !BitSet(flag, 0x80) ? global_colors : 1 << ((flag & 0x07) + 1);  // 判断使用全局颜色列表或使用局部颜色列表，并取得列表大小
		unsigned long* colortable = new unsigned long[colors];  // 申请颜色列表

		interlaced = BitSet(flag, 0x40);  // 当前帧图像数据存储方式，为 1 表示交织顺序存储，0 表示顺序存储

		delay = 0;
		dispose = 0;
		(void)dispose; // unused
		iterations = 1;
		(void)iterations; //unused
		/*if (image_info->ping)
		{
		f (opacity >= 0)
		/image->matte=true;

		CloseBlob(image);
		return(image);
		}*/
		if ((width == 0) || (height == 0))
			return NULL;
		/*
		Inititialize colormap.
		*/
		/*if (!AllocateImageColormap(image,image->colors))
		ThrowReaderException(ResourceLimitWarning,"Memory allocation failed",
		image);*/
		if (!BitSet(flag, 0x80))  // 如果使用全局颜色列表
		{
			/*
			Use global colormap.
			*/
			p = global_colormap;
			for (i = 0; i < (int)colors; i++)
			{
				int r = *p++;
				int g = *p++;
				int b = *p++;

				colortable[i] = 0xFF000000 | (r << 16) | (g << 8) | (b);
			}

			//image->background_color=
			//image->colormap[Min(background,image->colors-1)];
		}
		else
		{
			unsigned char
				* colormap;

			/*
			Read local colormap.
			*/
			colormap = new unsigned char[3 * colors];

			int pos = p_ftell(fp);
			(void)pos; // unused

			p_fread(colormap, sizeof(char), 3 * colors, fp);

			p = colormap;
			for (i = 0; i < (int)colors; i++)
			{
				int r = *p++;
				int g = *p++;
				int b = *p++;

				colortable[i] = 0xFF000000 | (r << 16) | (g << 8) | (b);
			}
			delete colormap;
		}

		/*if (opacity >= (int) colors)
		{
		for (i=colors; i < (opacity+1); i++)
		{
		image->colormap[i].red=0;
		image->colormap[i].green=0;
		image->colormap[i].blue=0;
		}
		image->colors=opacity+1;
		}*/
		/*
		Decode image.
		*/
		//status=DecodeImage(image,opacity,exception);

		//if (global_colormap != (unsigned char *) NULL)
		// LiberateMemory((void **) &global_colormap);
		if (global_colormap != NULL)
		{
			delete[] global_colormap;
			global_colormap = NULL;
		}

		//while (image->previous != (Image *) NULL)
		//    image=image->previous;

#define MaxStackSize  4096
#define NullCode  (-1)

		int
			available,
			bits,
			code,
			clear,
			code_mask,
			code_size,
			count,
			end_of_information,
			in_code,
			offset,
			old_code,
			pass,
			y;

		int
			x;

		unsigned int
			datum;

		short
			* prefix;

		unsigned char
			data_size,
			first,
			* packet,
			* pixel_stack,
			* suffix,
			* top_stack;

		/*
		Allocate decoder tables.
		*/

		packet = new unsigned char[256];
		prefix = new short[MaxStackSize];
		suffix = new unsigned char[MaxStackSize];
		pixel_stack = new unsigned char[MaxStackSize + 1];

		/*
		Initialize GIF data stream decoder.
		*/
		p_fread(&data_size, sizeof(char), 1, fp);
		clear = 1 << data_size;
		end_of_information = clear + 1;
		available = clear + 2;
		old_code = NullCode;
		code_size = data_size + 1;
		code_mask = (1 << code_size) - 1;
		for (code = 0; code < clear; code++)
		{
			prefix[code] = 0;
			suffix[code] = (unsigned char)code;
		}
		/*
		Decode GIF pixel stream.
		*/
		datum = 0;
		bits = 0;
		c = 0;
		count = 0;
		first = 0;
		offset = 0;
		pass = 0;
		top_stack = pixel_stack;

		std::unique_ptr<uint32_t[]> aBits = std::make_unique<uint32_t[]>(width * height);

		unsigned char* c = NULL;

		for (y = 0; y < (int)height; y++)
		{
			//q=SetImagePixels(image,0,offset,width,1);
			//if (q == (PixelPacket *) NULL)
			//break;
			//indexes=GetIndexes(image);

			uint32_t* q = aBits.get() + offset * width;

			for (x = 0; x < (int)width; )
			{
				if (top_stack == pixel_stack)
				{
					if (bits < code_size)
					{
						/*
						Load bytes until there is enough bits for a code.
						*/
						if (count == 0)
						{
							/*
							Read a new data block.
							*/
							int pos = p_ftell(fp);
							(void)pos; // unused

							count = ReadBlobBlock(fp, (char*)packet);
							if (count <= 0)
								break;
							c = packet;
						}
						datum += (*c) << bits;
						bits += 8;
						c++;
						count--;
						continue;
					}
					/*
					Get the next code.
					*/
					code = datum & code_mask;
					datum >>= code_size;
					bits -= code_size;
					/*
					Interpret the code
					*/
					if ((code > available) || (code == end_of_information))
						break;
					if (code == clear)
					{
						/*
						Reset decoder.
						*/
						code_size = data_size + 1;
						code_mask = (1 << code_size) - 1;
						available = clear + 2;
						old_code = NullCode;
						continue;
					}
					if (old_code == NullCode)
					{
						*top_stack++ = suffix[code];
						old_code = code;
						first = (unsigned char)code;
						continue;
					}
					in_code = code;
					if (code >= available)
					{
						*top_stack++ = first;
						code = old_code;
					}
					while (code >= clear)
					{
						*top_stack++ = suffix[code];
						code = prefix[code];
					}
					first = suffix[code];
					/*
					Add a new string to the string table,
					*/
					if (available >= MaxStackSize)
						break;
					*top_stack++ = first;
					prefix[available] = old_code;
					suffix[available] = first;
					available++;
					if (((available & code_mask) == 0) && (available < MaxStackSize))
					{
						code_size++;
						code_mask += available;
					}
					old_code = in_code;
				}
				/*
				Pop a pixel off the pixel stack.
				*/
				top_stack--;

				int index = (*top_stack);

				*q = colortable[index];

				if (index == opacity)
					*q = 0;

				x++;
				q++;
			}

			if (!interlaced)
				offset++;
			else
			{
				switch (pass)
				{
				case 0:
				default:
				{
					offset += 8;
					if (offset >= height)
					{
						pass++;
						offset = 4;
					}
					break;
				}
				case 1:
				{
					offset += 8;
					if (offset >= height)
					{
						pass++;
						offset = 2;
					}
					break;
				}
				case 2:
				{
					offset += 4;
					if (offset >= height)
					{
						pass++;
						offset = 1;
					}
					break;
				}
				case 3:
				{
					offset += 2;
					break;
				}
				}
			}

			if (x < width)
				break;

			/*if (image->previous == (Image *) NULL)
			if (QuantumTick(y,image->rows))
			MagickMonitor(LoadImageText,y,image->rows);*/
		}
		delete[] pixel_stack;
		delete[] suffix;
		delete[] prefix;
		delete[] packet;

		delete[] colortable;

		//if (y < image->rows)
		//failed = true;

		std::unique_ptr<Image> anImage = std::make_unique<Image>(width, height, std::move(aBits));

		//TODO: Change for animation crap
		p_fclose(fp);
		return anImage;
	}

	p_fclose(fp);

	return NULL;
}

typedef struct my_error_mgr * my_error_ptr;

struct my_error_mgr
{
	struct jpeg_error_mgr pub;  /* "public" fields */

	jmp_buf setjmp_buffer;  /* for return to caller */
};

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);

}

bool ImageLib::WriteJPEGImage(const std::string& theFileName, Image* theImage)
{
	FILE *fp;

	if ((fp = fopen(theFileName.c_str(), "wb")) == NULL)
		return false;

	struct jpeg_compress_struct cinfo;
	struct my_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer))
	{
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */
		jpeg_destroy_compress(&cinfo);
		fclose(fp);
		return false;
	}

	jpeg_create_compress(&cinfo);

	cinfo.image_width = theImage->mWidth;
	cinfo.image_height = theImage->mHeight;
	cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
		cinfo.optimize_coding = 1;
		jpeg_set_defaults(&cinfo);
		jpeg_set_quality(&cinfo, 80, TRUE);

	jpeg_stdio_dest(&cinfo, fp);

	jpeg_start_compress(&cinfo, true);

	int row_stride = theImage->mWidth * 3;

	unsigned char* aTempBuffer = new unsigned char[row_stride];

	uint32_t* aSrcPtr = theImage->mBits.get();

	for (int aRow = 0; aRow < theImage->mHeight; aRow++)
	{
		unsigned char* aDest = aTempBuffer;

		for (int aCol = 0; aCol < theImage->mWidth; aCol++)
		{
			unsigned long src = *(aSrcPtr++);

			*aDest++ = (src >> 16) & 0xFF;
			*aDest++ = (src >>  8) & 0xFF;
			*aDest++ = (src      ) & 0xFF;
		}

		jpeg_write_scanlines(&cinfo, &aTempBuffer, 1);
	}

	delete [] aTempBuffer;

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	fclose(fp);

	return true;
}

bool ImageLib::WritePNGImage(const std::string& theFileName, Image* theImage)
{
	png_structp png_ptr;
	png_infop info_ptr;

	FILE *fp;

	if ((fp = fopen(theFileName.c_str(), "wb")) == NULL)
		return false;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);

	if (png_ptr == NULL)
	{
		fclose(fp);
		return false;
	}

	// Allocate/initialize the memory for image information.  REQUIRED.
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return false;
	}

	 // Set error handling if you are using the setjmp/longjmp method (this is
	 // the normal method of doing things with libpng).  REQUIRED unless you
	 // set up your own error handlers in the png_create_write_struct() earlier.

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		// Free all of the memory associated with the png_ptr and info_ptr
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		// If we get here, we had a problem writeing the file
		return 0;
	}

	png_init_io(png_ptr, fp);

	png_color_8 sig_bit;
	sig_bit.red = 8;
	sig_bit.green = 8;
	sig_bit.blue = 8;
	/* if the image has an alpha channel then */
	sig_bit.alpha = 8;
	png_set_sBIT(png_ptr, info_ptr, &sig_bit);
	png_set_bgr(png_ptr);

	png_set_IHDR(png_ptr, info_ptr, theImage->mWidth, theImage->mHeight, 8, PNG_COLOR_TYPE_RGB_ALPHA,
			 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Add filler (or alpha) byte (before/after each RGB triplet)
	//png_set_expand(png_ptr);
	//png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	//png_set_gray_1_2_4_to_8(png_ptr);
	//png_set_palette_to_rgb(png_ptr);
	//png_set_gray_to_rgb(png_ptr);

	png_write_info(png_ptr, info_ptr);

	for (int i = 0; i < theImage->mHeight; i++)
	{
		png_bytep aRowPtr = (png_bytep) (theImage->mBits.get() + i*theImage->mWidth);
		png_write_rows(png_ptr, &aRowPtr, 1);
	}

	// write rest of file, and get additional chunks in info_ptr - REQUIRED
	png_write_end(png_ptr, info_ptr);

	// clean up after the write, and free any memory allocated - REQUIRED
	png_destroy_write_struct(&png_ptr, &info_ptr);

	// close the file
	fclose(fp);

	return true;
}

bool ImageLib::WriteTGAImage(const std::string& theFileName, Image* theImage)
{
	FILE* aTGAFile = fopen(theFileName.c_str(), "wb");
	if (aTGAFile == NULL)
		return false;

	BYTE aHeaderIDLen = 0;
	fwrite(&aHeaderIDLen, sizeof(BYTE), 1, aTGAFile);

	BYTE aColorMapType = 0;
	fwrite(&aColorMapType, sizeof(BYTE), 1, aTGAFile);
	
	BYTE anImageType = 2;
	fwrite(&anImageType, sizeof(BYTE), 1, aTGAFile);

	WORD aFirstEntryIdx = 0;
	fwrite(&aFirstEntryIdx, sizeof(WORD), 1, aTGAFile);

	WORD aColorMapLen = 0;
	fwrite(&aColorMapLen, sizeof(WORD), 1, aTGAFile);

	BYTE aColorMapEntrySize = 0;
	fwrite(&aColorMapEntrySize, sizeof(BYTE), 1, aTGAFile); 

	WORD anXOrigin = 0;
	fwrite(&anXOrigin, sizeof(WORD), 1, aTGAFile);

	WORD aYOrigin = 0;
	fwrite(&aYOrigin, sizeof(WORD), 1, aTGAFile);

	WORD anImageWidth = theImage->mWidth;
	fwrite(&anImageWidth, sizeof(WORD), 1, aTGAFile); 

	WORD anImageHeight = theImage->mHeight;
	fwrite(&anImageHeight, sizeof(WORD), 1, aTGAFile);  

	BYTE aBitCount = 32;
	fwrite(&aBitCount, sizeof(BYTE), 1, aTGAFile);  

	BYTE anImageDescriptor = 8 | (1<<5);
	fwrite(&anImageDescriptor, sizeof(BYTE), 1, aTGAFile);

	fwrite(theImage->mBits.get(), 4, theImage->mWidth*theImage->mHeight, aTGAFile);

	fclose(aTGAFile);

	return true;
}

typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef  enum {
	 BI_RGB = 0x0000,
	 BI_RLE8 = 0x0001,
	 BI_RLE4 = 0x0002,
	 BI_BITFIELDS = 0x0003,
	 BI_JPEG = 0x0004,
	 BI_PNG = 0x0005,
	 BI_CMYK = 0x000B,
	 BI_CMYKRLE8 = 0x000C,
	 BI_CMYKRLE4 = 0x000D
} Compression;

bool ImageLib::WriteBMPImage(const std::string& theFileName, Image* theImage)
{
	FILE* aFile = fopen(theFileName.c_str(), "wb");
	if (aFile == NULL)
		return false;

	BITMAPFILEHEADER aFileHeader;
	BITMAPINFOHEADER aHeader;

	memset(&aFileHeader,0,sizeof(aFileHeader));
	memset(&aHeader,0,sizeof(aHeader));

	int aNumBytes = theImage->mWidth*theImage->mHeight*4;

	aFileHeader.bfType = ('M'<<8) | 'B';
	aFileHeader.bfSize = sizeof(aFileHeader) + sizeof(aHeader) + aNumBytes;
	aFileHeader.bfOffBits = sizeof(aHeader); 

	aHeader.biSize = sizeof(aHeader);
	aHeader.biWidth = theImage->mWidth;
	aHeader.biHeight = theImage->mHeight;
	aHeader.biPlanes = 1;
	aHeader.biBitCount = 32;
	aHeader.biCompression = BI_RGB;

	fwrite(&aFileHeader,sizeof(aFileHeader),1,aFile);
	fwrite(&aHeader,sizeof(aHeader),1,aFile);
	DWORD *aRow = theImage->mBits.get() + (theImage->mHeight-1)*theImage->mWidth;
	int aRowSize = theImage->mWidth*4;
	(void)aRowSize; // Unused
	for (int i=0; i<theImage->mHeight; i++, aRow-=theImage->mWidth)
		fwrite(aRow,4,theImage->mWidth,aFile);

	fclose(aFile);
	return true;
}

////////////////////////////////////////////////////////////////////////// 
// JPEG Pak Reader

typedef struct {
	struct jpeg_source_mgr pub; /* public fields */

	PFILE * infile;   /* source stream */
	JOCTET * buffer;    /* start of buffer */
	boolean start_of_file;  /* have we gotten any data yet? */
} pak_source_mgr;

typedef pak_source_mgr * pak_src_ptr;

#define INPUT_BUF_SIZE 4096

METHODDEF(void) init_source (j_decompress_ptr cinfo)
{
	pak_src_ptr src = (pak_src_ptr) cinfo->src;
	src->start_of_file = TRUE;
}

METHODDEF(boolean) fill_input_buffer (j_decompress_ptr cinfo)
{
	pak_src_ptr src = (pak_src_ptr) cinfo->src;
	size_t nbytes;

	nbytes = p_fread(src->buffer, 1, INPUT_BUF_SIZE, src->infile);
	//((size_t) fread((void *) (buf), (size_t) 1, (size_t) (sizeofbuf), (file)))

	if (nbytes <= 0) {
		if (src->start_of_file) /* Treat empty input file as fatal error */
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo, JWRN_JPEG_EOF);
		/* Insert a fake EOI marker */
		src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
	}

	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;
	src->start_of_file = FALSE;

	return TRUE;
}

METHODDEF(void) skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	pak_src_ptr src = (pak_src_ptr) cinfo->src;

	if (num_bytes > 0) {
		while (num_bytes > (long) src->pub.bytes_in_buffer) {
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) fill_input_buffer(cinfo);
		}
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}

METHODDEF(void) term_source (j_decompress_ptr /* cinfo */)
{
	/* no work necessary here */
}

void jpeg_pak_src (j_decompress_ptr cinfo, PFILE* infile)
{
	pak_src_ptr src;

	/* The source object and input buffer are made permanent so that a series
	* of JPEG images can be read from the same file by calling jpeg_stdio_src
	* only before the first one.  (If we discarded the buffer at the end of
	* one image, we'd likely lose the start of the next one.)
	* This makes it unsafe to use this manager and a different source
	* manager serially with the same JPEG object.  Caveat programmer.
	*/
	if (cinfo->src == NULL) { /* first time for this JPEG object? */
		cinfo->src = (struct jpeg_source_mgr *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
			sizeof(pak_source_mgr));
		src = (pak_src_ptr) cinfo->src;
		src->buffer = (JOCTET *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
			INPUT_BUF_SIZE * sizeof(JOCTET));
	}

	src = (pak_src_ptr) cinfo->src;
	src->pub.init_source = init_source;
	src->pub.fill_input_buffer = fill_input_buffer;
	src->pub.skip_input_data = skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = term_source;
	src->infile = infile;
	src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
	src->pub.next_input_byte = NULL; /* until buffer loaded */
}


std::unique_ptr<Image> GetJPEGImage(const std::string& theFileName)
{
	PFILE *fp;

	if ((fp = p_fopen(theFileName.c_str(), "rb")) == NULL)
		return NULL;

	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer))
	{
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */
		jpeg_destroy_decompress(&cinfo);
		p_fclose(fp);
		return NULL;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_pak_src(&cinfo, fp);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	int row_stride = cinfo.output_width * cinfo.output_components;

	unsigned char** buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	std::unique_ptr<uint32_t[]> aBits = std::make_unique<uint32_t[]>(cinfo.output_width*cinfo.output_height);
	uint32_t* q = aBits.get();

	if (cinfo.output_components==1)
	{
		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, buffer, 1);

			unsigned char* p = *buffer;
			for (unsigned int i = 0; i < cinfo.output_width; i++)
			{
				int r = *p++;
				*q++ = 0xFF000000 | (r << 16) | (r << 8) | (r);
			}
		}
	}
	else
	{
		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, buffer, 1);

			unsigned char* p = *buffer;
			for (unsigned int i = 0; i < cinfo.output_width; i++)
			{
				int r = *p++;
				int g = *p++;
				int b = *p++;

				*q++ = 0xFF000000 | (r << 16) | (g << 8) | (b);
			}
		}
	}

	std::unique_ptr<Image> anImage = std::make_unique<Image>(cinfo.output_width, cinfo.output_height, std::move(aBits));

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	p_fclose(fp);

	return anImage;
}

int ImageLib::gAlphaComposeColor = 0xFFFFFF;
bool ImageLib::gAutoLoadAlpha = true;
bool ImageLib::gIgnoreJPEG2000Alpha = true;

uint32_t AverageNearByPixels(ImageLib::Image* theImage, uint32_t* thePixel, int x, int y)
{
    int aRed = 0;
    int aGreen = 0;
    int aBlue = 0;
    int aBitsCount = 0;

    for (int i = -1; i <= 1; i++)  // 依次循环上方、当前、下方的一行
    {
        if (i == 0)  // 排除当前行
        {
            continue;
        }

        for (int j = -1; j <= 1; j++)  // 依次循环左方、当前、右方的一列
        {
            if ((x != 0 || j != -1) && (x != theImage->mWidth - 1 || j != 1) && (y != 0 || i != -1) && (y != theImage->mHeight - 1 || i != 1))
            {
                unsigned long aPixel = *(thePixel + i * theImage->mWidth + j);
                if (aPixel & 0xFF000000UL)  // 如果不是透明像素
                {
                    aRed += (aPixel >> 16) & 0x000000FFUL;
                    aGreen += (aPixel >> 8) & 0x000000FFUL;
                    aBlue += aPixel & 0x000000FFUL;
                    aBitsCount++;
                }
            }
        }
    }

    if (aBitsCount == 0)
        return 0;

    aRed /= aBitsCount;
    aRed = std::min(aRed, 255);
    aGreen /= aBitsCount;
    aGreen = std::min(aGreen, 255);
    aBlue /= aBitsCount;
    aBlue = std::min(aBlue, 255);
    return (aRed << 16) | (aGreen << 8) | (aBlue);
}

std::unique_ptr<Image> ImageLib::GetImage(const std::string& theFilename, bool lookForAlphaImage, bool theDoImageSanding)
{
	if (!gAutoLoadAlpha)
		lookForAlphaImage = false;

	int aLastDotPos = theFilename.rfind('.');
	int aLastSlashPos = std::max((int)theFilename.rfind('\\'), (int)theFilename.rfind('/'));

	std::string anExt;
	std::string aFilename;

	if (aLastDotPos > aLastSlashPos)
	{
		anExt = theFilename.substr(aLastDotPos, theFilename.length() - aLastDotPos);
		aFilename = theFilename.substr(0, aLastDotPos);
	}
	else
		aFilename = theFilename;

	std::unique_ptr<Image> anImage = nullptr;

	if ((anImage == NULL) && ((strcasecmp(anExt.c_str(), ".tga") == 0) || (anExt.length() == 0)))
		anImage = GetTGAImage(aFilename + ".tga");

	if ((anImage == NULL) && ((strcasecmp(anExt.c_str(), ".jpg") == 0) || (anExt.length() == 0)))
		anImage = GetJPEGImage(aFilename + ".jpg");

	if ((anImage == NULL) && ((strcasecmp(anExt.c_str(), ".png") == 0) || (anExt.length() == 0)))
		anImage = GetPNGImage(aFilename + ".png");

	if ((anImage == NULL) && ((strcasecmp(anExt.c_str(), ".gif") == 0) || (anExt.length() == 0)))
		anImage = GetGIFImage(aFilename + ".gif");

	if ((anImage == NULL) && (strcasecmp(anExt.c_str(), ".j2k") == 0))
		unreachable(); // There are no JPEG2000 files in the project
		//anImage = GetJPEG2000Image(aFilename + ".j2k");
	if ((anImage == NULL) && (strcasecmp(anExt.c_str(), ".jp2") == 0))
		unreachable(); // There are no JPEG2000 files in the project
		//anImage = GetJPEG2000Image(aFilename + ".jp2");


	// Check for alpha images
	std::unique_ptr<Image> anAlphaImage = NULL;
	if(lookForAlphaImage)
	{
		// Check _ImageName
		anAlphaImage = GetImage(theFilename.substr(0, aLastSlashPos+1) + "_" +
			theFilename.substr(aLastSlashPos+1, theFilename.length() - aLastSlashPos - 1), false);

		// Check ImageName_
		if(anAlphaImage == NULL)
			anAlphaImage = GetImage(theFilename + "_", false);
	}

	// Compose alpha channel with image
	if (anAlphaImage != NULL) 
	{
		if (anImage != NULL)
		{
			if ((anImage->mWidth == anAlphaImage->mWidth) &&
				(anImage->mHeight == anAlphaImage->mHeight))
			{
				uint32_t* aBits1 = anImage->mBits.get();
				uint32_t* aBits2 = anAlphaImage->mBits.get();
				int aSize = anImage->mWidth*anImage->mHeight;

				for (int i = 0; i < aSize; i++)
				{
					*aBits1 = (*aBits1 & 0x00FFFFFF) | ((*aBits2 & 0xFF) << 24);
					++aBits1;
					++aBits2;
				}
			}
		}
		else if (gAlphaComposeColor==0xFFFFFF)
		{
			anImage = std::move(anAlphaImage);

			uint32_t* aBits1 = anImage->mBits.get();

			int aSize = anImage->mWidth*anImage->mHeight;
			for (int i = 0; i < aSize; i++)
			{
				*aBits1 = (0x00FFFFFF) | ((*aBits1 & 0xFF) << 24);
				++aBits1;
			}
		}
		else
		{
			const int aColor = gAlphaComposeColor;
			anImage = std::move(anAlphaImage);

			uint32_t* aBits1 = anImage->mBits.get();

			int aSize = anImage->mWidth*anImage->mHeight;
			for (int i = 0; i < aSize; i++)
			{
				*aBits1 = aColor | ((*aBits1 & 0xFF) << 24);
				++aBits1;
			}
		}
	}

    // Image Sanding. We have to move it here to avoid moving the image
    // between the CPU and GPU constantly. Sand once before uploading to
    // the GPU and everyone is happy.
	if (theDoImageSanding && anImage != NULL) {
		uint32_t* aBitsPtr = anImage->mBits.get();
		for (int y = 0; y < anImage->mHeight; y++)
		{
			for (int x = 0; x < anImage->mWidth; x++)
			{
				if ((*aBitsPtr & 0xFF000000UL) == 0)  // 如果像素的不透明度为 0
				{
					*aBitsPtr = AverageNearByPixels(anImage.get(), aBitsPtr, x, y);  // 计算该点周围非透明像素的平均颜色
				}

				aBitsPtr++;
			}
		}
	}

	return anImage;
}

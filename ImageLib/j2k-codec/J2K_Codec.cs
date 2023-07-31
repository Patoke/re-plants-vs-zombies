using System;
using System.Runtime.InteropServices;
using System.Text;

// Application namespace - change this to match your application if needed
namespace J2K_CodecPrj
{
	public abstract class J2KCodec
	{
		// Returns codec version and build number in the 0x105678 form, where 
		// 10 is a version (1.0) and 5678 is a build number.
		[DllImport("j2k-codec", EntryPoint="_J2K_getVersion@0")]
		public static extern uint GetVersion();
		
		// Returns the code of last error. Use GetErrorString() to get 
		// textual description of the error.
		[DllImport("j2k-codec", EntryPoint="_J2K_getLastError@0")]
		public static extern J2KError GetLastError();
		
		[DllImport("j2k-codec", EntryPoint="_J2K_getErrorStrVB@8")]
		private static extern void _GetErrorString(J2KError errCode, StringBuilder errStr);

		// Returns the textual description of an error by the error code 
		// obtained from getLastError().
		public static string GetErrorString(J2KError errCode)
		{
			StringBuilder sb = new StringBuilder(2048);
			_GetErrorString(errCode, sb);
			return sb.ToString();
		}

		// Returns the textual description of the last error.
		public static string GetLastErrorString()
		{
			return GetErrorString(GetLastError());
		}

		// This function unlocks the full functionality of J2K-Codec. After you
		// have purchased your personal registration key, you need to pass it 
		// to this function.
		[DllImport("j2k-codec", EntryPoint="_J2K_Unlock@4")]
		public static extern void Unlock(string key);


		// This function creates a log-file and starts logging debug messages of J2K-Codec.
		// 'level' can be Normal or Detailed.
		// Passing append as true will append the log session to existing log-file.
		// Returns 0 if there was no error.
		// NOTES: 
		//  1. Log-file name is 'j2k-codec.log'
		//  2. The performance will degrade significantly if the logging is on.
		[DllImport("j2k-codec", EntryPoint="_J2K_StartLogging@8")]
		public static extern J2KError StartLogging(J2KLogLevel level, bool append);

		// Use this function to stop logging, initiated by StartLogging().
		[DllImport("j2k-codec", EntryPoint="_J2K_StopLogging@0")]
		public static extern void StopLogging(int level, bool append);

		// Open an image from a file with given name.
		[DllImport("j2k-codec", EntryPoint="_J2K_OpenFile@4")]
		public static extern IntPtr OpenFile(string fileName);

		// Open the image from a buffer with 'size' length
		[DllImport("j2k-codec", EntryPoint="_J2K_OpenFile@4")]
		public static extern IntPtr OpenMemory(byte[] buffer, uint size);

		// Get size information from the image
		[DllImport("j2k-codec", EntryPoint="_J2K_GetInfo@16")]
		public static extern J2KError GetInfo(IntPtr image, out int width, out int height, out int components);

		// Get more detailed information from the image
		[DllImport("j2k-codec", EntryPoint="_J2K_GetInfoEx@8")]
		public static extern J2KError GetInfoEx(IntPtr image, out J2KInfo info);


		// Get meta data, embedded into the JP2 image. See help file for parameter explanation
		[DllImport("j2k-codec", EntryPoint="_J2K_GetInfoEx@8")]
		public static extern J2KError GetMetaData(IntPtr image, ref int no, ref J2KMetadata type, ref IntPtr data, ref int size);

		// Decodes the image, previously created with Open(). If 'buffer' is
		// null, then the required amount of memory is allocated and its size
		// returned through the 'size' argument. pitch - distance, in bytes, to
		// the start of next line. For options see help file.
		[DllImport("j2k-codec", EntryPoint="_J2K_Decode@20")]
		public static extern J2KError Decode(IntPtr image, ref IntPtr buffer, ref int size, string options, ref int pitch);

		// Selects or unselects a tile or a tile range, depending on 'select'.
		// If select is true then the tiles are selected for decoding, 
		// otherwise they are unselected. The range is defined by start and end
		// tile numbers (inclusive) in the raster order. If end_tile==-1 then
		// the max tile number will be used instead.
		[DllImport("j2k-codec", EntryPoint="_J2K_SelectTiles@16")]
		public static extern J2KError SelectTiles(IntPtr image, int startTile, int endTile, bool select);

		// Gets the number of available resolution levels.
		[DllImport("j2k-codec", EntryPoint="_J2K_GetResolutions@8")]
		public static extern J2KError GetResolutions(IntPtr image, ref int resolutions);

		// Gets the dimensions of a resolution level.
		[DllImport("j2k-codec", EntryPoint="_J2K_GetResolutionDimensions@16")]
		public static extern J2KError GetResolutionDimensions(IntPtr image, int resLevel, out int width, out int height);


		// Cancels Decode() operations.
		[DllImport("j2k-codec", EntryPoint="_J2K_Cancel@4")]
		public static extern J2KError Cancel(IntPtr image);

		// Destroys the image, previously created by Open(). All images must 
		// be closed using this function to avoid memory leaks.
		[DllImport("j2k-codec", EntryPoint="_J2K_Close@4")]
		public static extern J2KError Close(IntPtr image);
	}

	public enum J2KError
	{
		Success = 0,
		NeedMMX,
		NotEnoughMemory,
		CorruptedData,
		PipeFailure,
		InvalidArgument,
		Canceled,
		CantOpenFile,
		OptionUnrecognized,
		NoSuchTile,
		NoSuchResolution,
		BppTooSmall,
		BufferTooSmall,
		NotPart1Format,
		ImageIsTooLarge,
		TooManyResLevel,
		TooLargeCodeblocks,
		NoLaziness,
		NoVcausal,
		TooManyComponents,
		Only8BitComponents,
		OnlyUnsigComponents,
		DownsampledComponents,
		RoiNotSupported,
		ProgrChangeNotSup,
		PacketHeadersNotSup,
		No64BitSupport,
		InternalError
	}

	public enum J2KLogLevel
	{
		Normal = 0,
		Detailed = 1,
		All = 1
	}

	public enum J2KMetadata
	{
		CommentStr,
		CommentBin,
		Geotiff,
		Xml,
		Url,
		Pal,
		Icc,
		Unknown
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct J2KInfo 
	{
		public int Version;
		public int Width;
		public int Height;
		public int Components;
		public int FileType;
		public int HorizontalTiles;
		public int VerticalTiles;
	}
}

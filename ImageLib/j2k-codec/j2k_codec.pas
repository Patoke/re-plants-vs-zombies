unit j2k_codec;

interface

const

  J2KERR_SUCCESS = 0;
  J2KERR_NEED_MMX = 1;
  J2KERR_NOT_ENOUGH_MEMORY = 2;
  J2KERR_CORRUPTED_DATA = 3;
  J2KERR_PIPE_FAILURE = 4;
  J2KERR_INVALID_ARGUMENT = 5;
  J2KERR_CANCELED = 6;
  J2KERR_CANT_OPEN_FILE = 7;
  J2KERR_OPTION_UNRECOGNIZED = 8;
  J2KERR_NO_SUCH_TILE = 9;
  J2KERR_NO_SUCH_RESOLUTION = 10;
  J2KERR_BPP_TOO_SMALL = 11;
  J2KERR_BUFFER_TOO_SMALL = 12;
  J2KERR_NOT_PART1_FORMAT = 13;
  J2KERR_IMAGE_IS_TOO_LARGE = 14;
  J2KERR_TOO_MANY_RES_LEVEL = 15;
  J2KERR_TOO_LARGE_CODEBLOCKS = 16;
  J2KERR_NO_LAZINESS = 17;
  J2KERR_NO_VCAUSAL = 18;
  J2KERR_TOO_MANY_COMPONENTS = 19;
  J2KERR_ONLY_8_BIT_COMPONENTS = 20;
  J2KERR_ONLY_UNSIG_COMPONENTS = 21;
  J2KERR_DOWNSAMPLED_COMPONENTS = 22;
  J2KERR_ROI_NOT_SUPPORTED = 23;
  J2KERR_PROGR_CHANGE_NOT_SUP = 24;
  J2KERR_PACKET_HDRS_NOT_SUP = 25;
  J2KERR_64BIT_BOXES_NOT_SUP = 26;
  J2KERR_INTERNAL_ERROR = 27;

  j2kdll = 'j2k-codec.dll';

type PJ2KImage = Pointer;

//////////////////////////////////////////////////////////////////////////////////////////
// J2K_getVersion()
//
// Returns codec version and build number in the 0x105678 form, where 10 is a version (1.0)
// and 5678 is a build number.
//////////////////////////////////////////////////////////////////////////////////////////

function J2K_getVersion():integer; stdcall; external j2kdll name '_J2K_getVersion@0';


//////////////////////////////////////////////////////////////////////////////////////////
// J2K_getLastError()
//
// Returns the code of last error. Use J2K_getErrorStr() to get textual description of
// the error.
//////////////////////////////////////////////////////////////////////////////////////////

function J2K_getLastError():integer; stdcall; external j2kdll name '_J2K_getLastError@0';


//////////////////////////////////////////////////////////////////////////////////////////
// J2K_getErrorStr()
//
// Returns the textual description of an error by the error code obtained from getLastError().
// If the errCode is < 0 or > Max number of errors, the "<unknown>" string is returned.
//////////////////////////////////////////////////////////////////////////////////////////

function J2K_getErrorStr(errCode:integer):PChar; stdcall; external j2kdll name '_J2K_getErrorStr@4';


//////////////////////////////////////////////////////////////////////////////////////////
// J2K_getLstErrorStr()
//
// Returns the textual description of the last error.
//////////////////////////////////////////////////////////////////////////////////////////

function J2K_getLastErrorStr():PChar; stdcall; external j2kdll name '_J2K_getLastErrorStr@0';


//////////////////////////////////////////////////////////////////////////////////////////
// J2K_Unlock()
//
// This function unlocks the full functionality of J2K-Codec. After you have purchased your
// personal registration key, you need to pass it to this function.
//////////////////////////////////////////////////////////////////////////////////////////

procedure J2K_Unlock(key:PChar); stdcall; external j2kdll name '_J2K_Unlock@4';


//////////////////////////////////////////////////////////////////////////////////////////
// J2K_StartLogging()
//
// This function creates a log-file and starts logging debug messages of J2K-Codec.
// 'level' can be 0 (Normal logging) or 1 (Detailed logging).
// Passing 'append' = 1 allows to append the log session to existing log-file.
// Returns 0 if there was no error.
// NOTES:
//  1. Log-file name is 'j2k-codec.log'
//  2. The performance will degrade significantly if the logging is on.
//////////////////////////////////////////////////////////////////////////////////////////

const

  J2K_LOG_NORMAL = 0;
  J2K_LOG_DETAILED = 1;
  J2K_LOG_ALL = J2K_LOG_DETAILED;

  J2K_LOG_APPEND = 1;

function J2K_StartLogging(level,append:integer):integer; stdcall; external j2kdll name '_J2K_StartLogging@8';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_StopLogging()
//
// Use this function to stop logging, initiated by J2K_StartLogging().
///////////////////////////////////////////////////////////////////////////////////////////

procedure J2K_StopLogging(); stdcall; external j2kdll name '_J2K_StopLogging@0';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_Open()
//
// Returns pointer to an internal class, representing the J2K image, or 0 if
// there was an error. Use J2K_getLastError() to obtain the error code.
///////////////////////////////////////////////////////////////////////////////////////////

type

  TJ2KCallbacks=record
  	seek:  function(data_source:Pointer; offset:integer):integer; stdcall;
  	read:  function(ptr:Pointer; size:integer; data_source:Pointer):integer; stdcall;
  	close: procedure(data_source:Pointer); stdcall;
  end;

function J2K_OpenFile(filename:PChar):PJ2KImage; stdcall; external j2kdll name '_J2K_OpenFile@4';
function J2K_OpenMemory(src_buffer:Pointer; src_size:integer):PJ2KImage; stdcall; external j2kdll name '_J2K_OpenMemory@8';
function J2K_OpenCustom(data_source:Pointer; const cbs:TJ2KCallbacks):PJ2KImage; stdcall; external j2kdll name '_J2K_OpenCustom@8';

///////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetInfo()
//
// Gets various information about the image.
// Returns J2KERR_SUCCESS or error code if function failed.
///////////////////////////////////////////////////////////////////////////////////////////

function J2K_GetInfo(image:PJ2KImage; out width, height, components:integer):integer; stdcall; external j2kdll name '_J2K_GetInfo@16';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetInfoEx()
//
// Gets extended information about the image.
// Returns J2KERR_SUCCESS or error code if function failed.
///////////////////////////////////////////////////////////////////////////////////////////

type

  TJ2KInfo = record
  	Version:integer;         // J2K-Codec version as returned by J2K_getVersion()
  	Width, Height:integer;   // Width and height of the image in pixels
  	Components:integer;      // The number of components in the image
  	FileType:integer;        // 0 - J2K, 1 - JP2
  	hTiles, vTiles:integer;  // The number of tiles in horizontal and vertical directions
  end;

function J2K_GetInfoEx(image:PJ2KImage; out info:TJ2KInfo):integer; stdcall; external j2kdll name '_J2K_GetInfoEx@8';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetMetaData()
//
// Gets meta data, embedded into the JP2 image.
// Returns J2KERR_SUCCESS or error code if function failed.
///////////////////////////////////////////////////////////////////////////////////////////

const
  JP2_METADATA_COMMENT_STR = 0;
  JP2_METADATA_COMMENT_BIN = 1;
  JP2_METADATA_GEOTIFF = 2;
  JP2_METADATA_XML = 3;
  JP2_METADATA_URL = 4;
  JP2_METADATA_PAL = 5;
  JP2_METADATA_ICC = 6;
  JP2_METADATA_UNKNOWN = 7;


function J2K_GetMetaData(image:PJ2KImage; out no, data_type:integer; var data:Pointer; out size:integer):integer; stdcall; external j2kdll name '_J2K_GetMetaData@20';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_SelectTiles()
//
// Selects or unselects a tile or a tile range, depending on 'action'.
// If action!=0 then the tiles are selected for decoding, otherwise - unselected.
// The range is defined by start and end tile numbers (inclusive) in the raster order.
// If end_tile==-1 then the max tile number will be used instead.
// Returns J2KERR_SUCCESS or error code if function failed.
///////////////////////////////////////////////////////////////////////////////////////////

function J2K_SelectTiles(image:PJ2KImage; start_tile, end_tile, action:integer):integer; stdcall; external j2kdll name '_J2K_SelectTiles@16';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetResolutions()
//
// Gets the number of available resolution levels.
// Returns J2KERR_SUCCESS or error code if function failed.
///////////////////////////////////////////////////////////////////////////////////////////

function J2K_GetResolutions(image:PJ2KImage; out resolutions:integer):integer; stdcall; external j2kdll name '_J2K_GetResolutions@8';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_GetResolutionDimensions()
//
// Gets the dimensions of a resolution level.
// Returns J2KERR_SUCCESS or error code if function failed.
///////////////////////////////////////////////////////////////////////////////////////////

function J2K_GetResolutionDimensions(image:PJ2KImage; res_level:integer; out width, height:integer):integer; stdcall; external j2kdll name '_J2K_GetResolutionDimensions@16';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_Decode()
//
// Decodes the image, previously created with J2K_Open().
// If 'buffer' is null, then the required amount of memory is allocated and its size returned
// through the 'size' argument.
// 'buff_pitch' is a buffer pitch - distance, in bytes, to the start of next line.
///////////////////////////////////////////////////////////////////////////////////////////

function J2K_Decode(image:PJ2KImage; out buffer:Pointer; out size:integer; options:PChar; out pitch:integer):integer; stdcall; external j2kdll name '_J2K_Decode@20';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_Cancel()
//
// Cancels J2K_Decode() operations.
///////////////////////////////////////////////////////////////////////////////////////////

procedure J2K_Cancel(image:PJ2KImage); stdcall; external j2kdll name '_J2K_Cancel@4';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_Close()
//
// Destroys the image, previously created by J2K_Open(). All images must be closed using
// this function to avoid memory leaks.
///////////////////////////////////////////////////////////////////////////////////////////

procedure J2K_Close(image:PJ2KImage); stdcall; external j2kdll name '_J2K_Close@4';


///////////////////////////////////////////////////////////////////////////////////////////
// J2K_EasyDecode()
//
//  Decodes JPEG2000 file into memory buffer.
//  Buffer, image width, height and the number of components are passed back via specified pointers.
//  If succeeded - returns pointer to an internal structure, which represents the image, otherwise - NULL.
// NOTE:
//  1. You must not free the buffer yourself - use J2K_Close(j2k_image) instead!
//  2. Unlike C++ Wrapper's easyDecode(), which allows to use the user buffer, C EasyDecode() doesn't.
///////////////////////////////////////////////////////////////////////////////////////////

function J2K_EasyDecode(filename:string; out buffer:Pointer; out width, height, components:integer; options:PChar):PJ2KImage;  stdcall; external j2kdll name '_J2K_EasyDecode@24';

implementation

end.


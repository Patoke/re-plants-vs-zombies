Attribute VB_Name = "J2K_Codec"
Public Enum J2K_ERRORS
	J2KERR_SUCCESS = 0
	J2KERR_NEED_MMX
	J2KERR_NOT_ENOUGH_MEMORY
	J2KERR_CORRUPTED_DATA
	J2KERR_PIPE_FAILURE
	J2KERR_INVALID_ARGUMENT
	J2KERR_CANCELED
	J2KERR_CANT_OPEN_FILE
	J2KERR_OPTION_UNRECOGNIZED
	J2KERR_NO_SUCH_TILE
	J2KERR_NO_SUCH_RESOLUTION
	J2KERR_BPP_TOO_SMALL
	J2KERR_BUFFER_TOO_SMALL
	J2KERR_NOT_PART1_FORMAT
	J2KERR_IMAGE_IS_TOO_LARGE
	J2KERR_TOO_MANY_RES_LEVEL
	J2KERR_TOO_LARGE_CODEBLOCKS
	J2KERR_NO_LAZINESS
	J2KERR_NO_VCAUSAL
	J2KERR_TOO_MANY_COMPONENTS
	J2KERR_ONLY_8_BIT_COMPONENTS
	J2KERR_ONLY_UNSIG_COMPONENTS
	J2KERR_DOWNSAMPLED_COMPONENTS
	J2KERR_ROI_NOT_SUPPORTED
	J2KERR_PROGR_CHANGE_NOT_SUP
	J2KERR_PACKET_HDRS_NOT_SUP
	J2KERR_64BIT_BOXES_NOT_SUP
	J2KERR_INTERNAL_ERROR
End Enum

Type J2K_Info
    Version As Long
    Width As Long
    Height As Long
    Components As Long
    FileType As Long
    hTiles As Long
    vTiles As Long
End Type

Declare Function J2K_getVersion Lib "j2k-codec" Alias "_J2K_getVersion@0" () As Long
Declare Function J2K_getLastError Lib "j2k-codec" Alias "_J2K_getLastError@0" () As Long

Declare Sub J2K_getErrorStr Lib "j2k-codec" Alias "_J2K_getErrorStrVB@8" (ByVal ErrCode As Long, ByVal errStr As String)

Declare Sub J2K_Unlock Lib "j2k-codec" Alias "_J2K_Unlock@4" (ByVal Key As String)

Declare Function J2K_StartLogging Lib "j2k-codec" Alias "_J2K_StartLogging@8" (ByVal Level As Long, ByVal Append As Boolean) As Long
Declare Sub J2K_StopLogging Lib "j2k-codec" Alias "_J2K_StopLogging@0" ()

Declare Function J2K_OpenFile Lib "j2k-codec" Alias "_J2K_OpenFile@4" (ByVal FileName As String) As Long
Declare Function J2K_OpenMemory Lib "j2k-codec" Alias "_J2K_OpenMemory@8" (ByRef Buffer As Byte, ByVal Size as Long) As Long

Declare Function J2K_GetInfo Lib "j2k-codec" Alias "_J2K_GetInfo@16" (ByVal Image As Long, ByRef Width As Long, ByRef Height As Long, ByRef Components As Long) As Long
Declare Function J2K_GetInfoEx Lib "j2k-codec" Alias "_J2K_GetInfoEx@8" (ByVal Image As Long, info As J2K_Info) As Long

Declare Function J2K_SelectTiles Lib "j2k-codec" Alias "_J2K_SelectTiles@16" (ByVal Image As Long, ByVal StartTile As Long, ByVal EndTile As Long, ByVal Action As Long) As Long

Declare Function J2K_GetResolutions Lib "j2k-codec" Alias "_J2K_GetResolutions@8" (ByVal Image As Long, ByRef Resolutions As Long) As Long
Declare Function J2K_GetResolutionDimensions Lib "j2k-codec" Alias "_J2K_GetResolutionDimensions@16" (ByVal Image As Long, ByVal ResLevel As Long, ByRef Width As Long, ByRef Height As Long) As Long

Declare Function J2K_Decode Lib "j2k-codec" Alias "_J2K_Decode@20" (ByVal Image As Long, ByRef Buffer As Long, ByRef Size As Long, Optional ByVal Options As String = "", Optional ByRef Pitch As Long = 0) As Long

Declare Sub J2K_Cancel Lib "j2k-codec" Alias "_J2K_Cancel@4" (ByVal Image As Long)

Declare Sub J2K_Close Lib "j2k-codec" Alias "_J2K_Close@4" (ByVal Image As Long)

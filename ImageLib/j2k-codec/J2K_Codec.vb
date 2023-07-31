Imports System.Text, System.Runtime.InteropServices
Module J2K_Codec
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

    Public Enum J2K_Metadata
        JP2_METADATA_COMMENT_STR = 0
        JP2_METADATA_COMMENT_BIN
        JP2_METADATA_GEOTIFF
        JP2_METADATA_XML
        JP2_METADATA_URL
        JP2_METADATA_PAL
        JP2_METADATA_ICC
        JP2_METADATA_UNKNOWN
    End Enum

    Public CodingSchemes() As String = {"Lossy J2K", "Lossy JP2", "Lossless J2K", "Lossless JP2"}

    'Note that all "Long"s from the VB6 version have been changed to "Integer"
    Public Structure J2K_Info
        Dim Version As Integer
        Dim Width As Integer
        Dim Height As Integer
        Dim Components As Integer
        Dim FileType As Integer
        Dim hTiles As Integer
        Dim vTiles As Integer
    End Structure

    Declare Function J2K_getVersion Lib "j2k-codec" Alias "_J2K_getVersion@0" () As Integer

    Declare Function J2K_getLastError Lib "j2k-codec" Alias "_J2K_getLastError@0" () As Integer

    Declare Sub J2K_getErrorStr Lib "j2k-codec" Alias "_J2K_getErrorStrVB@8" (ByVal ErrCode As Integer, ByVal errStr As String)

    Declare Sub J2K_Unlock Lib "j2k-codec" Alias "_J2K_Unlock@4" (ByVal Key As String)

    Declare Function J2K_StartLogging Lib "j2k-codec" Alias "_J2K_StartLogging@8" (ByVal Level As Integer, ByVal Append As Boolean) As Integer

    Declare Sub J2K_StopLogging Lib "j2k-codec" Alias "_J2K_StopLogging@0" ()

    Declare Function J2K_OpenFile Lib "j2k-codec" Alias "_J2K_OpenFile@4" (ByVal FileName As String) As Integer

    Declare Function J2K_OpenMemory Lib "j2k-codec" Alias "_J2K_OpenMemory@8" (ByRef Buffer As Byte, ByVal Size As Integer) As Integer

    Declare Function J2K_GetInfo Lib "j2k-codec" Alias "_J2K_GetInfo@16" (ByVal Image As Integer, ByRef Width As Integer, ByRef Height As Integer, ByRef Components As Integer) As Integer
    'In Vb6 Info was declared as ByVal - it *has* to be ByRef for DotNet to work properly!
    Declare Function J2K_GetInfoEx Lib "j2k-codec" Alias "_J2K_GetInfoEx@8" (ByVal Image As Integer, ByRef info As J2K_Info) As Integer

    Declare Function J2K_SelectTiles Lib "j2k-codec" Alias "_J2K_SelectTiles@16" (ByVal Image As Integer, ByVal StartTile As Integer, ByVal EndTile As Integer, ByVal Action As Integer) As Integer

    Declare Function J2K_GetResolutions Lib "j2k-codec" Alias "_J2K_GetResolutions@8" (ByVal Image As Integer, ByRef Resolutions As Integer) As Integer

    Declare Function J2K_GetResolutionDimensions Lib "j2k-codec" Alias "_J2K_GetResolutionDimensions@16" (ByVal Image As Integer, ByVal ResLevel As Integer, ByRef Width As Integer, ByRef Height As Integer) As Integer

    Declare Function J2K_Decode Lib "j2k-codec" Alias "_J2K_Decode@20" (ByVal Image As Integer, ByRef Buffer As IntPtr, ByRef Size As Integer, Optional ByVal Options As String = "", Optional ByRef Pitch As Integer = 0) As Integer

    Declare Function J2K_GetMetaData Lib "j2k-codec" Alias "_J2K_GetMetaData@20" (ByVal Image As Integer, ByRef No As Integer, ByRef Type As Integer, ByRef Buffer As IntPtr, ByRef Size As Integer) As Integer

    Declare Sub J2K_Cancel Lib "j2k-codec" Alias "_J2K_Cancel@4" (ByVal Image As Integer)

    Declare Sub J2K_Close Lib "j2k-codec" Alias "_J2K_Close@4" (ByVal Image As Integer)
End Module

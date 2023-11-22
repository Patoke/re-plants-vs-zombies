#include "FlashWidget.h"
#include "DDImage.h"
#include "SexyAppBase.h"
#include "WidgetManager.h"
#include "Debug.h"

using namespace Sexy;

//#pragma comment(lib, "comsuppw.lib")

//////////////////////////////////////////////////////////////////////////
// Interfaces imported from flash.ocx

#include <comdef.h>

//implement our own conversion functions

//------------------------//
// Convert char * to BSTR //

inline BSTR _com_util::ConvertStringToBSTR(const char* pSrc)
{
    if(!pSrc) return NULL;

    DWORD cwch;

    BSTR wsOut(NULL);

    if(cwch = ::MultiByteToWideChar(CP_ACP, 0, pSrc, 
         -1, NULL, 0))//get size minus NULL terminator
    {
                cwch--;
            wsOut = ::SysAllocStringLen(NULL, cwch);

        if(wsOut)
        {
            if(!::MultiByteToWideChar(CP_ACP, 
                     0, pSrc, -1, wsOut, cwch))
            {
                if(ERROR_INSUFFICIENT_BUFFER == ::GetLastError())
                    return wsOut;
                ::SysFreeString(wsOut);//must clean up
                wsOut = NULL;
            }
        }

    };

    return wsOut;
};

//------------------------//
// Convert BSTR to char * //
//------------------------//
inline char* _com_util::ConvertBSTRToString(BSTR pSrc)
{
    if(!pSrc) return NULL;

    //convert even embeded NULL
    DWORD cb,cwch = ::SysStringLen(pSrc);

    char *szOut = NULL;

    if(cb = ::WideCharToMultiByte(CP_ACP, 0, 
               pSrc, cwch + 1, NULL, 0, 0, 0))
    {
        szOut = new char[cb];
        if(szOut)
        {
            szOut[cb - 1]  = '\0';

            if(!::WideCharToMultiByte(CP_ACP, 0, 
                pSrc, cwch + 1, szOut, cb, 0, 0))
            {
                delete []szOut;//clean up if failed;
                szOut = NULL;
            }
        }
    }

    return szOut;
};

#pragma pack(push, 8)

namespace ShockwaveFlashObjects 
{

//
// Forward references and typedefs
//

struct __declspec(uuid("d27cdb6b-ae6d-11cf-96b8-444553540000"))
/* LIBID */ __ShockwaveFlashObjects;
struct __declspec(uuid("d27cdb6c-ae6d-11cf-96b8-444553540000"))
/* dual interface */ IShockwaveFlash;
struct __declspec(uuid("d27cdb6d-ae6d-11cf-96b8-444553540000"))
/* dispinterface */ _IShockwaveFlashEvents;
struct /* coclass */ ShockwaveFlash;
struct /* coclass */ FlashProp;
struct __declspec(uuid("d27cdb70-ae6d-11cf-96b8-444553540000"))
/* interface */ IFlashFactory;
struct __declspec(uuid("d27cdb72-ae6d-11cf-96b8-444553540000"))
/* interface */ IFlashObjectInterface;
struct __declspec(uuid("a6ef9860-c720-11d0-9337-00a0c90dcaa9"))
/* interface */ IDispatchEx;
struct /* coclass */ FlashObjectInterface;

//
// Smart pointer typedef declarations
//

_COM_SMARTPTR_TYPEDEF(IShockwaveFlash, __uuidof(IShockwaveFlash));
_COM_SMARTPTR_TYPEDEF(_IShockwaveFlashEvents, __uuidof(_IShockwaveFlashEvents));
_COM_SMARTPTR_TYPEDEF(IFlashFactory, __uuidof(IFlashFactory));
_COM_SMARTPTR_TYPEDEF(IDispatchEx, __uuidof(IDispatchEx));
_COM_SMARTPTR_TYPEDEF(IFlashObjectInterface, __uuidof(IFlashObjectInterface));

//
// Type library items
//

struct __declspec(uuid("d27cdb6c-ae6d-11cf-96b8-444553540000"))
IShockwaveFlash : IDispatch
{
    //
    // Property data
    //

    __declspec(property(get=GetStacking,put=PutStacking))
    _bstr_t Stacking;
    __declspec(property(get=GetWMode,put=PutWMode))
    _bstr_t WMode;
    __declspec(property(get=GetMovie,put=PutMovie))
    _bstr_t Movie;
    __declspec(property(get=GetSAlign,put=PutSAlign))
    _bstr_t SAlign;
    __declspec(property(get=GetMenu,put=PutMenu))
    VARIANT_BOOL Menu;
    __declspec(property(get=GetQuality,put=PutQuality))
    int Quality;
    __declspec(property(get=GetLoop,put=PutLoop))
    VARIANT_BOOL Loop;
    __declspec(property(get=GetFrameNum,put=PutFrameNum))
    long FrameNum;
    __declspec(property(get=GetBase,put=PutBase))
    _bstr_t Base;
    __declspec(property(get=Getscale,put=Putscale))
    _bstr_t scale;
    __declspec(property(get=GetDeviceFont,put=PutDeviceFont))
    VARIANT_BOOL DeviceFont;
    __declspec(property(get=GetEmbedMovie,put=PutEmbedMovie))
    VARIANT_BOOL EmbedMovie;
    __declspec(property(get=GetBGColor,put=PutBGColor))
    _bstr_t BGColor;
    __declspec(property(get=GetQuality2,put=PutQuality2))
    _bstr_t Quality2;
    __declspec(property(get=GetReadyState))
    long ReadyState;
    __declspec(property(get=GetScaleMode,put=PutScaleMode))
    int ScaleMode;
    __declspec(property(get=GetAlignMode,put=PutAlignMode))
    int AlignMode;
    __declspec(property(get=GetBackgroundColor,put=PutBackgroundColor))
    long BackgroundColor;
    __declspec(property(get=GetTotalFrames))
    long TotalFrames;
    __declspec(property(get=GetPlaying,put=PutPlaying))
    VARIANT_BOOL Playing;
    __declspec(property(get=GetSWRemote,put=PutSWRemote))
    _bstr_t SWRemote;

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall get_ReadyState (
        /*[out,retval]*/ long * thestate ) = 0;
      virtual HRESULT __stdcall get_TotalFrames (
        /*[out,retval]*/ long * numframes ) = 0;
      virtual HRESULT __stdcall get_Playing (
        /*[out,retval]*/ VARIANT_BOOL * Playing ) = 0;
      virtual HRESULT __stdcall put_Playing (
        /*[in]*/ VARIANT_BOOL Playing ) = 0;
      virtual HRESULT __stdcall get_Quality (
        /*[out,retval]*/ int * Quality ) = 0;
      virtual HRESULT __stdcall put_Quality (
        /*[in]*/ int Quality ) = 0;
      virtual HRESULT __stdcall get_ScaleMode (
        /*[out,retval]*/ int * scale ) = 0;
      virtual HRESULT __stdcall put_ScaleMode (
        /*[in]*/ int scale ) = 0;
      virtual HRESULT __stdcall get_AlignMode (
        /*[out,retval]*/ int * align ) = 0;
      virtual HRESULT __stdcall put_AlignMode (
        /*[in]*/ int align ) = 0;
      virtual HRESULT __stdcall get_BackgroundColor (
        /*[out,retval]*/ long * color ) = 0;
      virtual HRESULT __stdcall put_BackgroundColor (
        /*[in]*/ long color ) = 0;
      virtual HRESULT __stdcall get_Loop (
        /*[out,retval]*/ VARIANT_BOOL * Loop ) = 0;
      virtual HRESULT __stdcall put_Loop (
        /*[in]*/ VARIANT_BOOL Loop ) = 0;
      virtual HRESULT __stdcall get_Movie (
        /*[out,retval]*/ BSTR * path ) = 0;
      virtual HRESULT __stdcall put_Movie (
        /*[in]*/ BSTR path ) = 0;
      virtual HRESULT __stdcall get_FrameNum (
        /*[out,retval]*/ long * FrameNum ) = 0;
      virtual HRESULT __stdcall put_FrameNum (
        /*[in]*/ long FrameNum ) = 0;
      virtual HRESULT __stdcall SetZoomRect (
        /*[in]*/ long left,
        /*[in]*/ long top,
        /*[in]*/ long right,
        /*[in]*/ long bottom ) = 0;
      virtual HRESULT __stdcall Zoom (
        /*[in]*/ int factor ) = 0;
      virtual HRESULT __stdcall Pan (
        /*[in]*/ long x,
        /*[in]*/ long y,
        /*[in]*/ int mode ) = 0;
      virtual HRESULT __stdcall Play ( ) = 0;
      virtual HRESULT __stdcall Stop ( ) = 0;
      virtual HRESULT __stdcall Back ( ) = 0;
      virtual HRESULT __stdcall Forward ( ) = 0;
      virtual HRESULT __stdcall Rewind ( ) = 0;
      virtual HRESULT __stdcall StopPlay ( ) = 0;
      virtual HRESULT __stdcall GotoFrame (
        /*[in]*/ long FrameNum ) = 0;
      virtual HRESULT __stdcall CurrentFrame (
        /*[out,retval]*/ long * FrameNum ) = 0;
      virtual HRESULT __stdcall IsPlaying (
        /*[out,retval]*/ VARIANT_BOOL * Playing ) = 0;
      virtual HRESULT __stdcall PercentLoaded (
        /*[out,retval]*/ long * __MIDL_0011 ) = 0;
      virtual HRESULT __stdcall FrameLoaded (
        /*[in]*/ long FrameNum,
        /*[out,retval]*/ VARIANT_BOOL * loaded ) = 0;
      virtual HRESULT __stdcall FlashVersion (
        /*[out,retval]*/ long * version ) = 0;
      virtual HRESULT __stdcall get_WMode (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_WMode (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_SAlign (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_SAlign (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_Menu (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_Menu (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_Base (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_Base (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_scale (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_scale (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_DeviceFont (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_DeviceFont (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_EmbedMovie (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_EmbedMovie (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_BGColor (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_BGColor (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_Quality2 (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_Quality2 (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall LoadMovie (
        /*[in]*/ int layer,
        /*[in]*/ BSTR url ) = 0;
      virtual HRESULT __stdcall TGotoFrame (
        /*[in]*/ BSTR target,
        /*[in]*/ long FrameNum ) = 0;
      virtual HRESULT __stdcall TGotoLabel (
        /*[in]*/ BSTR target,
        /*[in]*/ BSTR label ) = 0;
      virtual HRESULT __stdcall TCurrentFrame (
        /*[in]*/ BSTR target,
        /*[out,retval]*/ long * FrameNum ) = 0;
      virtual HRESULT __stdcall TCurrentLabel (
        /*[in]*/ BSTR target,
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall TPlay (
        /*[in]*/ BSTR target ) = 0;
      virtual HRESULT __stdcall TStopPlay (
        /*[in]*/ BSTR target ) = 0;
      virtual HRESULT __stdcall SetVariable (
        /*[in]*/ BSTR name,
        /*[in]*/ BSTR value ) = 0;
      virtual HRESULT __stdcall GetVariable (
        /*[in]*/ BSTR name,
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall TSetProperty (
        /*[in]*/ BSTR target,
        /*[in]*/ int property,
        /*[in]*/ BSTR value ) = 0;
      virtual HRESULT __stdcall TGetProperty (
        /*[in]*/ BSTR target,
        /*[in]*/ int property,
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall TCallFrame (
        /*[in]*/ BSTR target,
        /*[in]*/ int FrameNum ) = 0;
      virtual HRESULT __stdcall TCallLabel (
        /*[in]*/ BSTR target,
        /*[in]*/ BSTR label ) = 0;
      virtual HRESULT __stdcall TSetPropertyNum (
        /*[in]*/ BSTR target,
        /*[in]*/ int property,
        /*[in]*/ double value ) = 0;
      virtual HRESULT __stdcall TGetPropertyNum (
        /*[in]*/ BSTR target,
        /*[in]*/ int property,
        /*[out,retval]*/ double * pVal ) = 0;
      virtual HRESULT __stdcall get_SWRemote (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_SWRemote (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_Stacking (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_Stacking (
        /*[in]*/ BSTR pVal ) = 0;
};

struct __declspec(uuid("d27cdb6d-ae6d-11cf-96b8-444553540000"))
_IShockwaveFlashEvents : IDispatch
{
    //
    // Wrapper methods for error-handling
    //

    // Methods:
    HRESULT OnReadyStateChange (
        long newState );
    HRESULT OnProgress (
        long percentDone );
    HRESULT FSCommand (
        _bstr_t command,
        _bstr_t args );
};

struct __declspec(uuid("d27cdb6e-ae6d-11cf-96b8-444553540000"))
ShockwaveFlash;
    // [ default ] interface IShockwaveFlash
    // [ default, source ] dispinterface _IShockwaveFlashEvents

struct __declspec(uuid("1171a62f-05d2-11d1-83fc-00a0c9089c5a"))
FlashProp;
    // [ default ] interface IUnknown

struct __declspec(uuid("d27cdb70-ae6d-11cf-96b8-444553540000"))
IFlashFactory : IUnknown
{};

struct __declspec(uuid("a6ef9860-c720-11d0-9337-00a0c90dcaa9"))
IDispatchEx : IDispatch
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT GetDispID (
        _bstr_t bstrName,
        unsigned long grfdex,
        long * pid );
    HRESULT RemoteInvokeEx (
        long id,
        unsigned long lcid,
        unsigned long dwFlags,
        DISPPARAMS * pdp,
        VARIANT * pvarRes,
        EXCEPINFO * pei,
        IServiceProvider * pspCaller,
        unsigned int cvarRefArg,
        unsigned int * rgiRefArg,
        VARIANT * rgvarRefArg );
    HRESULT DeleteMemberByName (
        _bstr_t bstrName,
        unsigned long grfdex );
    HRESULT DeleteMemberByDispID (
        long id );
    HRESULT GetMemberProperties (
        long id,
        unsigned long grfdexFetch,
        unsigned long * pgrfdex );
    HRESULT GetMemberName (
        long id,
        BSTR * pbstrName );
    HRESULT GetNextDispID (
        unsigned long grfdex,
        long id,
        long * pid );
    HRESULT GetNameSpaceParent (
        IUnknown * * ppunk );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_GetDispID (
        /*[in]*/ BSTR bstrName,
        /*[in]*/ unsigned long grfdex,
        /*[out]*/ long * pid ) = 0;
      virtual HRESULT __stdcall raw_RemoteInvokeEx (
        /*[in]*/ long id,
        /*[in]*/ unsigned long lcid,
        /*[in]*/ unsigned long dwFlags,
        /*[in]*/ DISPPARAMS * pdp,
        /*[out]*/VARIANT * pvarRes,
        /*[out]*/EXCEPINFO * pei,
        /*[in]*/ struct IServiceProvider * pspCaller,
        /*[in]*/ unsigned int cvarRefArg,
        /*[in]*/ unsigned int * rgiRefArg,
        /*[in,out]*/ VARIANT * rgvarRefArg ) = 0;
      virtual HRESULT __stdcall raw_DeleteMemberByName (
        /*[in]*/ BSTR bstrName,
        /*[in]*/ unsigned long grfdex ) = 0;
      virtual HRESULT __stdcall raw_DeleteMemberByDispID (
        /*[in]*/ long id ) = 0;
      virtual HRESULT __stdcall raw_GetMemberProperties (
        /*[in]*/ long id,
        /*[in]*/ unsigned long grfdexFetch,
        /*[out]*/ unsigned long * pgrfdex ) = 0;
      virtual HRESULT __stdcall raw_GetMemberName (
        /*[in]*/ long id,
        /*[out]*/ BSTR * pbstrName ) = 0;
      virtual HRESULT __stdcall raw_GetNextDispID (
        /*[in]*/ unsigned long grfdex,
        /*[in]*/ long id,
        /*[out]*/ long * pid ) = 0;
      virtual HRESULT __stdcall raw_GetNameSpaceParent (
        /*[out]*/ IUnknown * * ppunk ) = 0;
};

struct __declspec(uuid("d27cdb72-ae6d-11cf-96b8-444553540000"))
IFlashObjectInterface : IDispatchEx
{};

struct __declspec(uuid("d27cdb71-ae6d-11cf-96b8-444553540000"))
FlashObjectInterface;
    // [ default ] interface IFlashObjectInterface

} // namespace ShockwaveFlashObjects

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////
// FlashSink - Receives flash events   

namespace Sexy
{

class FlashSink : public ShockwaveFlashObjects::_IShockwaveFlashEvents
{
public:	
	LPCONNECTIONPOINT		mConnectionPoint;	
	DWORD					mCookie;
	int						mRefCount;
	FlashWidget*			mFlashWidget;

public:
	FlashSink()
	{		
		mCookie = 0;
		mConnectionPoint = NULL;
		mRefCount = 0;
		mFlashWidget = NULL;		
	}

	virtual ~FlashSink()
	{
		mFlashWidget->mCOMCount--;
	}

	HRESULT Init(FlashWidget* theFlashWidget)
	{
		mFlashWidget = theFlashWidget;
		mFlashWidget->mCOMCount++;

		HRESULT aResult = NOERROR;
		LPCONNECTIONPOINTCONTAINER aConnectionPoint = NULL;
						
		if ((mFlashWidget->mFlashInterface->QueryInterface(IID_IConnectionPointContainer, (void**) &aConnectionPoint) == S_OK) &&
			(aConnectionPoint->FindConnectionPoint(__uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents), &mConnectionPoint) == S_OK))			
		{
			IDispatch* aDispatch = NULL;
			QueryInterface(__uuidof(IDispatch), (void**) &aDispatch);
			if (aDispatch != NULL)
			{
				aResult = mConnectionPoint->Advise((LPUNKNOWN)aDispatch, &mCookie);
				aDispatch->Release();
			}
		}
				
		if (aConnectionPoint != NULL) 
			aConnectionPoint->Release();

		return aResult;
	}

	HRESULT Shutdown()
	{
		HRESULT aResult = S_OK;

		if (mConnectionPoint)
		{
			if (mCookie)
			{
				aResult = mConnectionPoint->Unadvise(mCookie);
				mCookie = 0;
			}

			mConnectionPoint->Release();
			mConnectionPoint = NULL;
		}

		return aResult;
	}
 
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppv)
	{
		*ppv = NULL;

		if (riid == IID_IUnknown)
		{
			*ppv = (LPUNKNOWN)this;
			AddRef();
			return S_OK;
		}
		else if (riid == IID_IDispatch)
		{
			*ppv = (IDispatch*)this;
			AddRef();
			return S_OK;
		}
		else if (riid == __uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents))
		{
			*ppv = (ShockwaveFlashObjects::_IShockwaveFlashEvents*) this;
			AddRef();
			return S_OK;
		}
		else
		{   
			return E_NOTIMPL;
		}
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{  
		return ++mRefCount;
	}

	ULONG STDMETHODCALLTYPE Release()
	{  
		int aRefCount = --mRefCount;
		if (aRefCount == 0)		
			delete this;		

		return aRefCount;
	}

 // IDispatch method
  virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo)
   { return E_NOTIMPL; }

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
	{
		return E_NOTIMPL; 
	}

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames,
		UINT cNames, LCID lcid,DISPID* rgDispId)
	{
		return E_NOTIMPL; 
	}

	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
		WORD wFlags, ::DISPPARAMS __RPC_FAR *pDispParams, VARIANT __RPC_FAR *pVarResult,
		::EXCEPINFO __RPC_FAR *pExcepInfo, UINT __RPC_FAR *puArgErr)
	{
		switch(dispIdMember)
		{
		case 0x7a6:			
			break;
		case 0x96:			
			if ((pDispParams->cArgs == 2) &&
				(pDispParams->rgvarg[0].vt == VT_BSTR) &&
				(pDispParams->rgvarg[1].vt == VT_BSTR))
			{
				FSCommand(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
			}
			break;
		case DISPID_READYSTATECHANGE:					
			break;
		default:			
			return DISP_E_MEMBERNOTFOUND;
		} 
		
		return NOERROR;
	}

	HRESULT OnReadyStateChange (long newState)
	{	
		return S_OK;
	}
    
	HRESULT OnProgress(long percentDone )
	{		
		return S_OK;
	}

	HRESULT FSCommand (_bstr_t command, _bstr_t args)
	{
		if (mFlashWidget->mFlashListener != NULL)
			mFlashWidget->mFlashListener->FlashCommand(mFlashWidget->mId, (char*) command, (char*) args);
		return S_OK;
	}	
};

}

using namespace ShockwaveFlashObjects;

//////////////////////////////////////////////////////////////////////////
// ControlSite - container that holds the flash control

namespace Sexy
{

class ControlSite : 	
	public IOleInPlaceSiteWindowless, 
	public IOleClientSite	
{
public:
	int						mRefCount;
	FlashWidget*			mFlashWidget;

public:
	ControlSite()
	{		
		mRefCount = 0;		
		mFlashWidget = NULL;
	}	

	virtual ~ControlSite()
	{
		if (mFlashWidget != NULL)
			mFlashWidget->mCOMCount--;
	}

	void Init(FlashWidget* theFlashWidget)
	{
		mFlashWidget = theFlashWidget;
		mFlashWidget->mCOMCount++;
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppv)
	{
		*ppv = NULL;

		if (riid == IID_IUnknown)
		{
			*ppv = (IUnknown*) (IOleWindow*) this;
			AddRef();
			return S_OK;
		}
		else if (riid == IID_IOleWindow)
		{
			*ppv = (IOleWindow*)this;
			AddRef();
			return S_OK;
		}
		else if (riid == IID_IOleInPlaceSite)
		{
			*ppv = (IOleInPlaceSite*)this;
			AddRef();
			return S_OK;
		}
		else if (riid == IID_IOleInPlaceSiteEx)
		{
			*ppv = (IOleInPlaceSiteEx*)this;
			AddRef();
			return S_OK;
		}
		else if (riid == IID_IOleInPlaceSiteWindowless)
		{
			*ppv = (IOleInPlaceSiteWindowless*)this;
			AddRef();
			return S_OK;
		}
		else if (riid == IID_IOleClientSite)
		{
			*ppv = (IOleClientSite*)this;
			AddRef();
			return S_OK;
		}
		else if (riid == __uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents))
		{
			*ppv = (ShockwaveFlashObjects::_IShockwaveFlashEvents*) this;
			AddRef();
			return S_OK;
		}
		else
		{   
			return E_NOTIMPL;
		}
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{  
		return ++mRefCount;
	}

	ULONG STDMETHODCALLTYPE Release()
	{ 
		int aRefCount = --mRefCount;
		if (aRefCount == 0)		
			delete this;		

		return aRefCount;
	}

	//////////////////////////////////////////////////////////////////////////	

	virtual HRESULT  STDMETHODCALLTYPE SaveObject(void)
	{
		return S_OK;
	}

	virtual HRESULT  STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker,IMoniker** ppmk )
	{
		*ppmk = NULL;
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer ** theContainerP)
	{
		//return QueryInterface(__uuidof(IOleContainer), (void**) theContainerP);				
		return E_NOINTERFACE;
	}


	virtual HRESULT  STDMETHODCALLTYPE ShowObject(void)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT  STDMETHODCALLTYPE OnShowWindow(BOOL)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT  STDMETHODCALLTYPE RequestNewObjectLayout(void)
	{
		return E_NOTIMPL;
	}
		//		
	

	HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(/* [in] */ BOOL fEnterMode)
	{
	    return S_OK;
	}

	HRESULT STDMETHODCALLTYPE GetWindow(/* [out] */ HWND __RPC_FAR* theWnndow)
	{
		return E_FAIL;

	    //*theWnndow = NULL;
		//*theWnndow = gSexyAppBase->mHWnd;
		//return S_OK;
	}

	HRESULT STDMETHODCALLTYPE CanInPlaceActivate(void)
	{
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE OnInPlaceActivate(void)
	{		
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE OnUIActivate(void)
	{		
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE GetWindowContext(/* [out] */ IOleInPlaceFrame __RPC_FAR *__RPC_FAR *ppFrame, /* [out] */ IOleInPlaceUIWindow __RPC_FAR *__RPC_FAR *ppDoc, /* [out] */ LPRECT lprcPosRect, /* [out] */ LPRECT lprcClipRect, /* [out][in] */ LPOLEINPLACEFRAMEINFO lpFrameInfo)
	{
		RECT aRect = mFlashWidget->GetRect().ToRECT();

		*lprcPosRect = aRect;
		*lprcClipRect = aRect;
		
		*ppFrame = NULL;
		QueryInterface(__uuidof(IOleInPlaceFrame), (void**) ppFrame);		
		*ppDoc = NULL;

		lpFrameInfo->fMDIApp = FALSE;
		lpFrameInfo->hwndFrame = NULL;
		lpFrameInfo->haccel = NULL;
		lpFrameInfo->cAccelEntries = 0;
		
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE Scroll(/* [in] */ SIZE scrollExtant)
	{
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE OnUIDeactivate(/* [in] */ BOOL fUndoable)
	{		
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate(void)
	{	
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE DiscardUndoState(void)
	{
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE DeactivateAndUndo(void)
	{
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE OnPosRectChange(/* [in] */ LPCRECT lprcPosRect)
	{
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnInPlaceActivateEx(/* [out] */ BOOL __RPC_FAR *pfNoRedraw, /* [in] */ DWORD dwFlags)
	{		
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE OnInPlaceDeactivateEx(/* [in] */ BOOL fNoRedraw)
	{		
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE RequestUIActivate(void)
	{
		return S_FALSE;
	}


	HRESULT STDMETHODCALLTYPE CanWindowlessActivate(void)
	{
		// Allow windowless activation?
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE GetCapture(void)
	{
		// TODO capture the mouse for the object
		return S_FALSE;
	}


	HRESULT STDMETHODCALLTYPE SetCapture(/* [in] */ BOOL fCapture)
	{
		// TODO capture the mouse for the object
		return S_FALSE;
	}


	HRESULT STDMETHODCALLTYPE GetFocus(void)
	{
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE SetFocus(/* [in] */ BOOL fFocus)
	{
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE GetDC(/* [in] */ LPCRECT pRect, /* [in] */ DWORD grfFlags, /* [out] */ HDC __RPC_FAR *phDC)
	{		
		return E_INVALIDARG;		
	}


	HRESULT STDMETHODCALLTYPE ReleaseDC(/* [in] */ HDC hDC)
	{
		return E_INVALIDARG;
	}


	HRESULT STDMETHODCALLTYPE InvalidateRect(/* [in] */ LPCRECT pRect, /* [in] */ BOOL fErase)
	{	
		if (pRect == NULL)
		{
			mFlashWidget->mDirtyRect = mFlashWidget->GetRect();
			mFlashWidget->mFlashDirty = true;
		}
		else if (!mFlashWidget->mFlashDirty)
		{
			mFlashWidget->mDirtyRect = Rect(pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top);
			mFlashWidget->mFlashDirty = true;
		}
		else
		{			
			mFlashWidget->mDirtyRect = mFlashWidget->mDirtyRect.Union(Rect(pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top));
		}		
		
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE InvalidateRgn(/* [in] */ HRGN hRGN, /* [in] */ BOOL fErase)
	{	
		mFlashWidget->mDirtyRect = mFlashWidget->GetRect();
		mFlashWidget->mFlashDirty = true;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ScrollRect(/* [in] */ INT dx, /* [in] */ INT dy, /* [in] */ LPCRECT pRectScroll, /* [in] */ LPCRECT pRectClip)
	{
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE AdjustRect(/* [out][in] */ LPRECT prc)
	{
		if (prc == NULL)
		{
			return E_INVALIDARG;
		}
		
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnDefWindowMessage(/* [in] */ UINT msg, /* [in] */ WPARAM wParam, /* [in] */ LPARAM lParam, /* [out] */ LRESULT __RPC_FAR *plResult)
	{
		return S_FALSE;
	}
};

}

//////////////////////////////////////////////////////////////////////////

const CLSID CLSID_ShockwaveFlash =
  { 0xD27CDB6E, 0xAE6D, 0x11cf, {0x96, 0xB8, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00} };

typedef HRESULT (__stdcall *DllGetClassObjectFunc)(REFCLSID rclsid, REFIID riid, LPVOID * ppv); 

FlashWidget::FlashWidget(int theId, FlashListener* theFlashListener)
{
	HRESULT aResult;

	mState = STATE_IDLE;

	mCurCursor = CURSOR_POINTER;
	mCurOverrideCursor = NULL;

	mCOMCount = 0;
	mPauseCount = 0;
	mAutoPause = true;
	mHasLostFocus = !gSexyAppBase->mHasFocus;
	mFlashDirty = true;	

	mBkgColor.mAlpha = 0;
	mId = theId;
	mFlashListener = theFlashListener;	

	mFlashSink = NULL;
	
	mImage = NULL;	
	mFlashInterface = NULL;
	mOleObject = NULL;
	mWindowlessObject = NULL;

	mBkgImage = NULL;
	mFlashLibHandle = NULL;

	CoInitialize(NULL);

	mControlSite = new ControlSite();
	mControlSite->AddRef();	
	mControlSite->Init(this);
		
	mFlashLibHandle = LoadLibraryA("flash.ocx");
	if (mFlashLibHandle != NULL)
	{
		IClassFactory* aClassFactory = NULL;
		DllGetClassObjectFunc aDllGetClassObjectFunc = (DllGetClassObjectFunc) GetProcAddress(mFlashLibHandle, "DllGetClassObject");
		aResult = aDllGetClassObjectFunc(CLSID_ShockwaveFlash, IID_IClassFactory, (void**)&aClassFactory);
		aClassFactory->CreateInstance(NULL, IID_IOleObject, (void**)&mOleObject);
		aClassFactory->Release();	
	}
	else
	{
		CoCreateInstance(CLSID_ShockwaveFlash, NULL, 
			CLSCTX_INPROC_SERVER,
			IID_IOleObject,
			(void**)&mOleObject);
	}

	IOleClientSite* aClientSite = NULL;
	mControlSite->QueryInterface(__uuidof(IOleClientSite), (void**) &aClientSite);
	mOleObject->SetClientSite(aClientSite);	

	mOleObject->QueryInterface(__uuidof(IShockwaveFlash), (LPVOID*) &mFlashInterface);
	_bstr_t aTrans = "Transparent";
	mFlashInterface->put_WMode(aTrans);

	aResult = mOleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, aClientSite, 0, NULL, NULL);
	aClientSite->Release();	
		
	mOleObject->QueryInterface(__uuidof(IOleInPlaceObjectWindowless), (LPVOID*) &mWindowlessObject);
			
	mFlashSink = new FlashSink();
	mFlashSink->AddRef();	
	mFlashSink->Init(this);
}

FlashWidget::~FlashWidget()
{
	CleanupImages();
		
	if (mWindowlessObject != NULL)
		mWindowlessObject->Release();
	if (mFlashInterface != NULL)
		mFlashInterface->Release();	

	if (mFlashSink != NULL)
	{
		mFlashSink->Shutdown();
		mFlashSink->Release();
	}

	mOleObject->Close(OLECLOSE_NOSAVE);
		
	if (mOleObject != NULL)
		mOleObject->Release();	

	if (mControlSite != NULL)
		mControlSite->Release();

	// Make sure all our COM objects were actually destroyed
	DBG_ASSERTE(mCOMCount == 0);

	if (mFlashLibHandle != NULL)
		FreeLibrary(mFlashLibHandle);	
}

double FlashWidget::GetFlashVersion()
{
	CoInitialize(NULL);

	IOleObject* anOleObject = NULL;
	if (FAILED(CoCreateInstance(CLSID_ShockwaveFlash, NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IOleObject,
		(void**) &anOleObject)))
		return 0.0;	

	IShockwaveFlash* aFlashInterface = NULL;
	if (FAILED(anOleObject->QueryInterface(__uuidof(IShockwaveFlash), (LPVOID*) &aFlashInterface)))
		return 0.0;
	
	long aVersion = 0;
	aFlashInterface->FlashVersion(&aVersion);

	aFlashInterface->Release();
	anOleObject->Release();

	return aVersion / 65536.0;
}

void FlashWidget::CleanupImages()
{
	if (mImage != NULL)
	{
		delete mImage;
		mImage = NULL;
	}
}

void FlashWidget::RebuildImages()
{
	CleanupImages();

	mImage = new DDImage(gSexyAppBase->mDDInterface);
	mImage->Create(mWidth, mHeight);
	mImage->SetImageMode(false, false);	
}

void FlashWidget::CheckCursor()
{
	HCURSOR aCursor = GetCursor();
	if (aCursor == ::LoadCursor(NULL, IDC_ARROW))
	{
		mCurCursor = CURSOR_POINTER;
		gSexyAppBase->mOverrideCursor = NULL;
		gSexyAppBase->SetCursor(mCurCursor);
	}
	else if (aCursor != NULL)
	{
		gSexyAppBase->mOverrideCursor = aCursor;
		mCurCursor = CURSOR_HAND;
		gSexyAppBase->SetCursor(mCurCursor);
	}
}

void FlashWidget::DrawFlashBackground(Graphics* g)
{
	if (mBkgColor.mAlpha != 0)
	{
		g->SetColor(mBkgColor);
		g->FillRect(0, 0, mWidth, mHeight);
	}

	if (mBkgImage != NULL)
	{
		if ((mBkgImageSrcRect.mWidth != 0) && (mBkgImageSrcRect.mHeight != 0))
			g->DrawImage(mBkgImage, 0, 0, mBkgImageSrcRect);
		else
			g->DrawImage(mBkgImage, 0, 0);
	}
}

bool FlashWidget::StartAnimation(const std::string& theFileName)
{
	std::string aFullPath = GetFullPath(theFileName);
	
	_bstr_t bstr((char*) aFullPath.c_str());		
	mFlashInterface->put_Movie(bstr); // you have to change the path here	
	if ((mPauseCount == 0) && (mFlashInterface->Play() != S_OK))
		return false;	

	if ((mWidgetManager != NULL) && (mIsOver))
		MouseMove(mWidgetManager->mLastMouseX - mX, mWidgetManager->mLastMouseY - mY);

	mState = STATE_PLAYING;	
	return true;
}

void FlashWidget::SetQuality(int theQuality)
{
	static char* aQualityNames[3] = {"Low", "Medium", "High"};
	
	_bstr_t aNewStr = aQualityNames[theQuality];
	mFlashInterface->put_Quality2(aNewStr);
}

void FlashWidget::Pause()
{
	mPauseCount++;

	if (mState != STATE_STOPPED)
		mState = STATE_IDLE;

	if ((mPauseCount == 1) && (mFlashInterface != NULL) && (mState != STATE_STOPPED))
		mFlashInterface->StopPlay();
}

void FlashWidget::Unpause()
{
	mPauseCount--;
	if ((mPauseCount == 0) && (mFlashInterface != NULL) && (mState != STATE_STOPPED))
	{
		mState = STATE_PLAYING;
		mFlashInterface->Play();
	}
}

void FlashWidget::Rewind()
{
	if (mFlashInterface != NULL)
	{
		mFlashInterface->Rewind();
		mFlashInterface->Play();
	}
}

void FlashWidget::GotoFrame(int theFrameNum)
{
	if (mFlashInterface != NULL)
	{
		mFlashInterface->GotoFrame(theFrameNum);
		mFlashInterface->Play();
	}
}

void FlashWidget::Back()
{
	if (mFlashInterface != NULL)
	{
		mFlashInterface->Back();
		mFlashInterface->Play();
	}
}

void FlashWidget::Forward()
{
	if (mFlashInterface != NULL)
	{
		mFlashInterface->Forward();
		mFlashInterface->Play();
	}
}

bool FlashWidget::IsPlaying()
{
	VARIANT_BOOL aBool = 0;
	if (mFlashInterface != NULL)
		mFlashInterface->IsPlaying(&aBool);
	return aBool != 0;
}

int FlashWidget::GetCurrentFrame()
{	
	long aCurrentFrame = -1;
	if (mFlashInterface != NULL)
		mFlashInterface->CurrentFrame(&aCurrentFrame);
	return aCurrentFrame;
}

std::string FlashWidget::GetCurrentLabel(const std::string& theTimeline)
{
	BSTR aBStr = L"";
	if (mFlashInterface != NULL)
		mFlashInterface->TCurrentLabel(_bstr_t(theTimeline.c_str()), &aBStr);
	return (const char*) _bstr_t(aBStr);
}

void FlashWidget::CallFrame(const std::string& theTimeline, int theFrameNum)
{
	if (mFlashInterface != NULL)
		mFlashInterface->TCallFrame(_bstr_t(theTimeline.c_str()), theFrameNum);
}

void FlashWidget::CallLabel(const std::string& theTimeline, const std::string& theLabel)
{
	if (mFlashInterface != NULL)
		mFlashInterface->TCallLabel(_bstr_t(theTimeline.c_str()), _bstr_t(theLabel.c_str()));
}

std::string FlashWidget::GetVariable(const std::string& theName)
{
	BSTR aBStr = L"";
	if (mFlashInterface != NULL)
		mFlashInterface->GetVariable(_bstr_t(theName.c_str()), &aBStr);
	return (const char*) _bstr_t(aBStr);
}

void FlashWidget::SetVariable(const std::string& theName, const std::string& theValue)
{
	if (mFlashInterface != NULL)
		mFlashInterface->SetVariable(_bstr_t(theName.c_str()), _bstr_t(theValue.c_str()));
}

void FlashWidget::Update()
{
	if (mAutoPause)
	{
		if (!gSexyAppBase->mHasFocus)
		{
			if (!mHasLostFocus)
			{
				if (mAutoPause)
					Pause();
				mHasLostFocus = true;
			}

			return;
		}
		else
		{
			if (mHasLostFocus)
			{
				if (mAutoPause)
					Unpause();
				mHasLostFocus = false;
			}
		}
	}

	if (mState == STATE_PLAYING)
	{
		VARIANT_BOOL isPlaying = 0;
		mFlashInterface->IsPlaying(&isPlaying);	
		if (!isPlaying)
		{
			mState = STATE_STOPPED;
			if ((mFlashListener != NULL) && (mPauseCount == 0))
				mFlashListener->FlashAnimEnded(mId);
		}
	}

	if (mFlashDirty)
		MarkDirty();
}

void FlashWidget::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Widget::Resize(theX, theY, theWidth, theHeight);

	mDirtyRect = Rect(theX, theY, theWidth, theHeight);	

	RebuildImages();

	IOleInPlaceObject* anInPlaceObject = NULL;	
	mOleObject->QueryInterface(__uuidof(IOleInPlaceObject), (LPVOID*) &anInPlaceObject);			

	if (anInPlaceObject != NULL)
	{
		RECT aRect = GetRect().ToRECT();
		anInPlaceObject->SetObjectRects(&aRect, &aRect);
		anInPlaceObject->Release();
	}
}

void FlashWidget::Draw(Graphics* g)
{	
	if (mFlashDirty)
	{
		Graphics anImageG(mImage);	
		DrawFlashBackground(&anImageG);	

		LPDIRECTDRAWSURFACE aSurface = mImage->GetSurface();
		if (aSurface == NULL)
			return;

		HDC aDC = NULL;
		if (aSurface->GetDC(&aDC) != S_OK)
			return;	

		IViewObject* aViewObject = NULL;
		mFlashInterface->QueryInterface(IID_IViewObject, (LPVOID*) &aViewObject);
		if (aViewObject != NULL)
		{
			RECTL aRect = {0, 0, mWidth, mHeight};

			Point anAbsPos = GetAbsPos();

			HRGN aRgn = CreateRectRgn(mDirtyRect.mX - anAbsPos.mX, mDirtyRect.mY - anAbsPos.mY, 
				mDirtyRect.mX + mDirtyRect.mWidth - anAbsPos.mX, 
				mDirtyRect.mY + mDirtyRect.mHeight - anAbsPos.mY);
			SelectClipRgn(aDC, aRgn);
			DeleteObject(aRgn);

			aViewObject->Draw(DVASPECT_CONTENT, 1,
				NULL, NULL, NULL, aDC, &aRect, NULL, NULL,
				0);

			aViewObject->Release();
		}

		aSurface->ReleaseDC(aDC);

		mFlashDirty = false;
	}
	
	g->DrawImage(mImage, 0, 0);
}

void FlashWidget::MouseMove(int x, int y)
{	
	Widget::MouseMove(x, y);

	LRESULT aResult;
	Point anAbsPos = GetAbsPos();
	mWindowlessObject->OnWindowMessage(WM_MOUSEMOVE, 0, MAKELPARAM(x + anAbsPos.mX, y + anAbsPos.mY), &aResult);

	if (mIsOver)
		CheckCursor();
}

void FlashWidget::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
	if (theBtnNum == 0)
	{
		LRESULT aResult;
		Point anAbsPos = GetAbsPos();
		mWindowlessObject->OnWindowMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x + anAbsPos.mX, y + anAbsPos.mY), &aResult);
		CheckCursor();
	}
}

void FlashWidget::MouseUp(int x, int y, int theBtnNum, int theClickCount)
{
	if (theBtnNum == 0)
	{
		LRESULT aResult;
		Point anAbsPos = GetAbsPos();
		mWindowlessObject->OnWindowMessage(WM_LBUTTONUP, 0, MAKELPARAM(x + anAbsPos.mX, y + anAbsPos.mY), &aResult);
		CheckCursor();
	}
}

void FlashWidget::MouseDrag(int x, int y)
{
	if (mWidgetManager->IsLeftButtonDown())
	{
		LRESULT aResult;
		Point anAbsPos = GetAbsPos();
		mWindowlessObject->OnWindowMessage(WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(x + anAbsPos.mX, y + anAbsPos.mY), &aResult);
		CheckCursor();
	}
}

void FlashWidget::MouseLeave()
{
	Widget::MouseLeave();

	// To prevent Flash control from setting our cursor if it thinks 
	// our mouse is still over something
	MouseMove(-1, -1);

	gSexyAppBase->mOverrideCursor = NULL;
	gSexyAppBase->SetCursor(CURSOR_POINTER);	
}

void FlashWidget::SysColorChanged()
{
	mFlashDirty = true;
	mDirtyRect = GetRect();

	MarkDirty();
}

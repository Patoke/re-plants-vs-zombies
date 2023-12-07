#include "HTTPTransfer.h"
#include "SexyAppBase.h"
#include <process.h>
#include <winsock.h>

using namespace Sexy;

static int gCurTransferId = 1;

HTTPTransfer::HTTPTransfer()
{
	mTransferId = gCurTransferId++;
	mResult = RESULT_NOT_STARTED;
	mContentLength = 0;
	mDemoLastKnownResult = mResult;
	mThreadRunning = false;	
}

HTTPTransfer::~HTTPTransfer()
{
	Abort();
	while (mThreadRunning)
	{		
		Sleep(20);
	}
}

std::string HTTPTransfer::GetAbsURL(const std::string& theBaseURL, const std::string& theRelURL)
{
	std::string aURL;

	if (theRelURL.substr(0, 7).compare("http://") == 0)
	{
		aURL = theRelURL;
	}
	else if (theRelURL.substr(0, 1).compare("/") == 0)
	{
		int aFirstSlashPos = theBaseURL.find('/', 7);
		if (aFirstSlashPos != -1)
		{
			aURL = theBaseURL.substr(0, aFirstSlashPos) + theRelURL;
		}
		else
		{
			aURL = theBaseURL + theRelURL;
		}
	}
	else
	{
		int aLastSlashPos = theBaseURL.rfind('/');
		if (aLastSlashPos >= 7)
		{
			aURL = theBaseURL.substr(0, aLastSlashPos+1) + theRelURL;
		}
		else
		{
			aURL = theBaseURL + "/" + theRelURL;
		}
	}

	return aURL;
}

void HTTPTransfer::Fail(int theResult)
{	
	mResult = theResult;
	mExiting = true;
}

bool HTTPTransfer::SocketWait(bool checkRead, bool checkWrite)
{
	while (!mExiting)
	{
		fd_set aReadSet;
		fd_set aWriteSet;
		fd_set anExceptSet;

		FD_ZERO(&aReadSet);
		FD_ZERO(&aWriteSet);
		FD_ZERO(&anExceptSet);

		if (checkRead)
			FD_SET(mSocket, &aReadSet);

		if (checkWrite)
			FD_SET(mSocket, &aWriteSet);

		FD_SET(mSocket, &anExceptSet);

		// Check every 100ms
		int aReadTime = 100;

		TIMEVAL aTimeout;
		aTimeout.tv_sec = aReadTime/1000;
		aTimeout.tv_usec = (aReadTime%1000)*1000;

		int aVal = select(FD_SETSIZE, &aReadSet, &aWriteSet, &anExceptSet, &aTimeout);
		
		if (aVal == SOCKET_ERROR)
		{
			Fail(RESULT_SOCKET_ERROR);
			return false;
		}

		if (FD_ISSET(mSocket, &anExceptSet))
		{
			Fail(RESULT_SOCKET_ERROR);
			return false;
		}

		if (FD_ISSET(mSocket, &aReadSet))
			return true;

		if (FD_ISSET(mSocket, &aWriteSet))
			return true;	
	}

	// Return true on abort
	return true;
}

void HTTPTransfer::GetThreadProc()
{
	WSADATA aDat;
	WSAStartup(MAKEWORD(1,1),&aDat);
	
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == 0)	
		Fail(RESULT_SOCKET_ERROR);

	// Set non-blocking
	ulong anIoctlVal = 1;
	ioctlsocket(mSocket, FIONBIO, &anIoctlVal);

	if (!mExiting)
	{
		unsigned long anAddr = inet_addr(mHost.c_str());
		if (anAddr == INADDR_NONE)
		{		
			HOSTENT *aHostEnt = gethostbyname(mHost.c_str());
			if (aHostEnt != NULL) 
				memcpy(&anAddr, aHostEnt->h_addr_list[0], 4);
		}

		if (anAddr == INADDR_NONE)
		{
			Fail(RESULT_INVALID_ADDR);
		}
		else 
		{
			SOCKADDR_IN aSockAddrIn;		
			memset((char*) &aSockAddrIn, 0, sizeof(aSockAddrIn));
			aSockAddrIn.sin_family      = AF_INET;
			aSockAddrIn.sin_addr.s_addr = anAddr;
			aSockAddrIn.sin_port        = htons(mPort);
			
			if (::connect(mSocket, (sockaddr*) &aSockAddrIn, sizeof(SOCKADDR_IN)) != 0)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
					Fail(RESULT_CONNECT_FAIL);

				// Wait for socket to become writable to know we connected
				if (!SocketWait(false, true))
					Fail(RESULT_CONNECT_FAIL);
			}		

			while ((mSendStr.length() > 0) && (!mExiting))
			{
				SocketWait(false, true);
				int aResult = send(mSocket, mSendStr.c_str(), mSendStr.length(), 0);

				if (aResult > 0)
				{
					mSendStr = mSendStr.substr(aResult);
				}
				else 
				{
					// int anError = WSAGetLastError(); // unused		
					Fail(RESULT_DISCONNECTED);
				}
			}			

			bool chunked = false;
			bool gotHeader = false;
			int aPos = 0;
			int aLastPos = 0;
			int aChunkLengthLeft = 0; 
			int aContentLengthLeft = 0;
			std::string aRecvStr;

			while (!mExiting)
			{
				// Wait for more characters if we couldn't do any processing with the 
				//  current ones
				if (aLastPos == aPos)
				{
					char aBuffer[257];
					SocketWait(true, false);					
					int aResult = recv(mSocket, aBuffer, 256, 0);
					if (aResult > 0)
					{
						aRecvStr.insert(aRecvStr.end(), aBuffer, aBuffer+aResult);

						aBuffer[aResult] = 0;						
					}
					else
					{
						// int anError = WSAGetLastError();	// unused			
						break;
					}
				}				

				aLastPos = aPos;

				if (!gotHeader)
				{
					// Still reading header...
					int anEndLPos = aRecvStr.find("\r\n", aPos);
					if (anEndLPos == -1)
						continue; // Not enough for a full line yet
					std::string aLine = aRecvStr.substr(aPos, anEndLPos-aPos);
					aPos = anEndLPos+2;

					if (aLine.substr(0, 7) == "HTTP/1.")
					{
						std::string aResult = aLine.substr(9, 3);
						if (aResult == "404")
						{
							Fail(RESULT_NOT_FOUND);
						}
						else if (aResult != "200")
						{
							Fail(RESULT_HTTP_ERROR);
						}
					}

					if (aLine == "Transfer-Encoding: chunked")
					{
						chunked = true;
					}

					char* aCheckStr = (char *)"Transfer-Encoding: ";
					if (strncmp(aLine.c_str(), aCheckStr, strlen(aCheckStr)) == 0)
					{
						if (strcmp(aLine.c_str() + strlen(aCheckStr), "identity") != 0)
							chunked = true;
					}

					aCheckStr = (char *)"Content-Length: ";
					if (strncmp(aLine.c_str(), aCheckStr, strlen(aCheckStr)) == 0)
					{
						aContentLengthLeft = atoi(aLine.c_str() + strlen(aCheckStr));
						mContentLength = aContentLengthLeft;
					}

					// Header is complete when we get a blank line
					if (aLine.length() == 0)
						gotHeader = true;
				}
				else
				{
					if (chunked)
					{
						// If it is zero, we need to get the next chunk length
						if (aChunkLengthLeft == 0)
						{
							// Get a hex length
							int anEndLPos = aRecvStr.find("\r\n", aPos);
							if (anEndLPos == -1)
								continue; // Not enough for a full line yet

							std::string aLine = aRecvStr.substr(aPos, anEndLPos-aPos);
							if (!StringToInt("0x" + Trim(aLine), &aChunkLengthLeft))
								break; // End transfer on conversion error
							if (aChunkLengthLeft == 0)
								break; // Zero-size chunk marks end of chunked transfer
														
							aPos = anEndLPos+2;
						}

						// Chunk it in
						int aCopyLen = std::min(aChunkLengthLeft, (int)aRecvStr.length() - aPos);
						if (aCopyLen > 0)
						{
							mContent += aRecvStr.substr(aPos, aCopyLen);
							aPos += aCopyLen;
							aChunkLengthLeft -= aCopyLen;

							// There is always a CRLF at the end of the chunk data
							if (aChunkLengthLeft == 0)
								aPos += 2;
						}
					}
					else
					{
						if (aContentLengthLeft > 0)
						{
							int aCopyLen = std::min(aContentLengthLeft, (int)aRecvStr.length() - aPos);
							mContent += aRecvStr.substr(aPos, aCopyLen);
							aPos += aCopyLen;
							aContentLengthLeft -= aCopyLen;
							if (aContentLengthLeft == 0)
							{
								// We have reached the end at this point
								break;
							}
						}
						else
						{
							// No length specified, we will just read until we close
							int aCopyLen = aRecvStr.length() - aPos;
							mContent += aRecvStr.substr(aPos, aCopyLen);
							aPos += aCopyLen;
						}
					}
				}
			}			
		}
	}

	closesocket(mSocket);
	mSocket = 0;
	WSACleanup();

	if (mAborted)
		Fail(RESULT_ABORTED);
	else if (mResult == RESULT_NOT_COMPLETED)
		mResult = RESULT_DONE;

	mThreadRunning = false;
	mTransferPending = false;
}

void HTTPTransfer::GetThreadProcStub(void *theArg)
{
	((HTTPTransfer*) theArg)->GetThreadProc();
}

void HTTPTransfer::PrepareTransfer(const std::string& theURL)
{
	Reset();	
	
	mTransferId = gCurTransferId++;
	mURL = theURL;	
	mExiting = false;
	mAborted = false;
	mResult = RESULT_NOT_COMPLETED;
	mContent.erase();
	mContentLength = 0;

	mPort = 80;
		
	int aSSPos = mURL.find("//");
	int aPos = 0;
	
	if (aSSPos != -1)
		aPos = aSSPos + 2;

	if (aSSPos != -1)
		mProto = mURL.substr(0, aSSPos-1);

	int aSlashPos = mURL.find("/", aPos);
	if (aSlashPos != -1)
	{
		mHost = mURL.substr(aPos, aSlashPos-aPos);
		mPath = mURL.substr(aSlashPos);
	}
	else
	{
		mHost = mURL.substr(aPos);
		mPath = "/";
	}

	int aSemiPos = (int) mHost.find(':');
	if (aSemiPos != -1)
	{
		mPort = atoi(mHost.substr(aSemiPos + 1).c_str());
		mHost = mHost.substr(0, aSemiPos);
	}	
}

void HTTPTransfer::StartTransfer()
{	
	mTransferPending = true;

	// Don't really start the transfer while in demo playing mode
	if ((gSexyAppBase != NULL) && (gSexyAppBase->mPlayingDemoBuffer))
		return;

	mThreadRunning = true;
	_beginthread(GetThreadProcStub, 0, this);
}

void HTTPTransfer::GetHelper(const std::string& theURL)
{
	PrepareTransfer(theURL);

	mSendStr = 
		"GET " + mPath + " HTTP/1.0\r\n" 
		"User-Agent: Mozilla/4.0 (compatible; popcap)\r\n"
		"Host: " + mHost + "\r\n"
		"Connection: close\r\n" +
		"\r\n";

	StartTransfer();
}

void HTTPTransfer::PostHelper(const std::string& theURL, const std::string& theParams)
{
	PrepareTransfer(theURL);

	mSendStr = 
		"POST " + mPath + " HTTP/1.0\r\n" 
		"Content-Type: application/x-www-form-urlencoded\r\n" + 
		"User-Agent: Mozilla/4.0 (compatible; popcap)\r\n"
		"Host: " + mHost + "\r\n"
		"Content-Length: " + StrFormat("%d", theParams.length()) + "\r\n" +
		"Connection: close\r\n" +
		"\r\n" + theParams;

	StartTransfer();
}

void HTTPTransfer::Get(const std::string& theURL)
{
	mSpecifiedBaseURL = "";
	mSpecifiedRelURL = theURL;
	GetHelper(theURL);
}

void HTTPTransfer::Post(const std::string& theURL, const std::string& theParams)
{
	mSpecifiedBaseURL = "";
	mSpecifiedRelURL = theURL;
	PostHelper(theURL, theParams);
}

void HTTPTransfer::Get(const std::string& theBaseURL, const std::string& theRelURL)
{
	mSpecifiedBaseURL = theBaseURL;
	mSpecifiedRelURL = theRelURL;
	
	GetHelper(GetAbsURL(theBaseURL, theRelURL));
}

void HTTPTransfer::Post(const std::string& theBaseURL, const std::string& theRelURL, const std::string& theParams)
{
	mSpecifiedBaseURL = theBaseURL;
	mSpecifiedRelURL = theRelURL;
	
	PostHelper(GetAbsURL(theBaseURL, theRelURL), theParams);
}

void HTTPTransfer::SendRequestString(const std::string& theHost, const std::string& theSendString)
{
	mSpecifiedBaseURL.erase();
	mSpecifiedRelURL.erase();
	PrepareTransfer(theHost);
	mSendStr = theSendString;
	StartTransfer();
}

void HTTPTransfer::Abort()
{	
	mAborted = true;
	mExiting = true;	
}

void HTTPTransfer::Reset()
{	
	if (mThreadRunning)
	{
		Abort();
		WaitFor();
	}	

	mResult = RESULT_NOT_STARTED;
	mDemoLastKnownResult = RESULT_NOT_STARTED;
	mTransferId = gCurTransferId++;	
	mContent.erase();
	mExiting = false;
	mAborted = false;
	mURL.erase();
	mSendStr.erase();	
	mPath.erase();
}

// static int aLastThreadId = 0; // Seems unused

void HTTPTransfer::UpdateStatus()
{	
	// This will save the result data in demo recording mode and load it in (if available)
	//  in demo playback mode

	if (gSexyAppBase != NULL)
	{
		if (gSexyAppBase->mPlayingDemoBuffer)
		{
			// We only need to try to get the result if we think we are waiting for one			
			if (gSexyAppBase->PrepareDemoCommand(false))
			{
				if ((!gSexyAppBase->mDemoIsShortCmd) && (gSexyAppBase->mDemoCmdNum == DEMO_HTTP_RESULT))
				{
					int anOldBufferPos = gSexyAppBase->mDemoBuffer.mReadBitPos;

					// Since we don't require a demo result entry to be here, we must verify
					//  that this is referring to us
					int aTransferId = gSexyAppBase->mDemoBuffer.ReadLong();
					
					if (aTransferId == mTransferId)
					{
						// We finally got our result!
						mResult = gSexyAppBase->mDemoBuffer.ReadByte();
						mContent = gSexyAppBase->mDemoBuffer.ReadString();
						mDemoLastKnownResult = mResult;
						gSexyAppBase->mDemoNeedsCommand = true;

						//TODO:
						//OutputDebugString(StrFormat("Found State Change on %s (Id %d) to %d at %d\r\n", mURL.c_str(), mTransferId, mResult, gSexyAppBase->mUpdateCount).c_str());
					}
					else
					{
						// Not us, go back
						gSexyAppBase->mDemoBuffer.mReadBitPos = anOldBufferPos;
					}
				}
			}
		}
		else if ((gSexyAppBase->mRecordingDemoBuffer) && (mResult != mDemoLastKnownResult))
		{
			//TODO:
			//OutputDebugString(StrFormat("Recording State Change on %s (Id %d) to %d at %d\r\n", mURL.c_str(), mTransferId, mResult, gSexyAppBase->mUpdateCount).c_str());			

			// Write out the new result
			gSexyAppBase->WriteDemoTimingBlock();
			gSexyAppBase->mDemoBuffer.WriteNumBits(0, 1);
			gSexyAppBase->mDemoBuffer.WriteNumBits(DEMO_HTTP_RESULT, 5);
			gSexyAppBase->mDemoBuffer.WriteLong(mTransferId);
			gSexyAppBase->mDemoBuffer.WriteByte(mResult);

			// Avoid any threading issues by not allowing access to mContent while in progress
			if (mResult == RESULT_NOT_COMPLETED)
				gSexyAppBase->mDemoBuffer.WriteString("");
			else
				gSexyAppBase->mDemoBuffer.WriteString(mContent);

			mDemoLastKnownResult = mResult;
		}
	}
}

void HTTPTransfer::WaitFor()
{
	while (mTransferPending)
	{
		UpdateStatus();
		Sleep(20);
	}
}

int HTTPTransfer::GetResultCode()
{
	UpdateStatus();
	return mResult;
}

std::string HTTPTransfer::GetContent()
{
	if (mResult == RESULT_NOT_COMPLETED)
		return "";

	UpdateStatus();
	return mContent;
}

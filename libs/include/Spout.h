//
//					Spout.h - Version 1.01
// 
#ifndef __ofxSpout__
#define __ofxSpout__

#include <windows.h>
#include <gl/GL.h>
#include <set>
#include <string>

namespace Spout {
	
	// exported functions

	// -----------------------------
	// Creation and deletion
	// -----------------------------
	extern "C" _declspec(dllexport)
	bool InitSender(char *name, unsigned int width, unsigned int height, bool& bTextureShare, bool bMemoryShare = false);

	extern "C" _declspec(dllexport)
	bool InitReceiver (char *name, unsigned int& width, unsigned int& height, bool& bTextureShare, bool bMemoryShare = false);

	extern "C" _declspec(dllexport)
	bool ReleaseSender();

	extern "C" _declspec(dllexport)
	bool ReleaseReceiver(); 

	extern "C" _declspec(dllexport)
	bool CloseSpout(); 
	// -----------------------------
	

	// -----------------------------
	// Sending and receiving opengl textures
	// -----------------------------
	extern "C" _declspec(dllexport)
	bool SendTexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert = false);

	extern "C" _declspec(dllexport)
	bool ReceiveTexture(char *name, GLuint TextureID, GLuint TextureTarget, unsigned int &width, unsigned int &height);

	// -----------------------------
	// User interface
	// -----------------------------
	extern "C" _declspec(dllexport)
	bool SelectSenderDialog();

	extern "C" _declspec(dllexport)
	bool SelectSenderPanel();
	// -----------------------------


	// -----------------------------
	// Sender management
	// -----------------------------
	extern "C" _declspec(dllexport)
	bool GetSenderNames(std::set<std::string> *sendernames);

	extern "C" _declspec(dllexport)
	int GetSenderCount();

	extern "C" _declspec(dllexport)
	bool GetSenderNameInfo(int index, char* sendername, int sendernameMaxSize, unsigned int &width, unsigned int &height, HANDLE &dxShareHandle);

	extern "C" _declspec(dllexport)
	bool GetSenderNameMap(char *sendermap);

	extern "C" _declspec(dllexport)
	bool GetSenderInfo(char* sendername, unsigned int &width, unsigned int &height, HANDLE &dxShareHandle);
	// -----------------------------


	// -----------------------------
	// Utilities
	// -----------------------------
	extern "C" _declspec(dllexport)
	bool TextureShareCompatible();

	extern "C" _declspec(dllexport)
	void SpoutDebugConsole();

	// -----------------------------


}

#endif

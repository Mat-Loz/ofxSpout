// ofxSpout, a Spout helper for OpenFrameworks
// About Spout : http://spout.zeal.co/

#pragma once

#include "ofMain.h"
#include "Spout.h"

using namespace Spout;

class ofxSpout
{
public:
	// sender
	static void init(string senderName, float width, float height, bool isSender = true);
	static void initSender();
	static void sendTexture();

	// receiver
	static void initReceiver();
	static void receiveTexture();

	static void exit();

private:
	static bool _isSender;
	static bool _isSpoutInitialized;
	static bool _isSpoutDoneOnce;
	static GLuint _spoutTexture;
	static char _spoutSenderName[256];
	static float _width;
	static float _height;
	static bool _isSpoutTextureShared;
	static ofTexture _receiverTexture;
};
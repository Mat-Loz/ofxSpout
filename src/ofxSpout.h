// ofxSpout, a Spout helper for OpenFrameworks
// About Spout : http://spout.zeal.co/

#pragma once

#include "ofMain.h"
#include "Spout.h"

using namespace Spout;

class ofxSpout
{
public:
	static void init(string senderName, float width, float height);
	static void initSender();
	static void sendTexture();
	static void exit();

private:
	static bool _isSpoutInitialized;
	static bool _isSpoutDoneOnce;
	static GLuint _spoutTexture;
	static char _spoutSenderName[256];
	static float _width;
	static float _height;
	static bool _isSpoutTextureShared;
};
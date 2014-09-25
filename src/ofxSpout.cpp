#include "ofxSpout.h"

// static members
bool ofxSpout::_isSpoutInitialized;
bool ofxSpout::_isSpoutDoneOnce;
GLuint ofxSpout::_spoutTexture;
char ofxSpout::_spoutSenderName[256];
float ofxSpout::_width;
float ofxSpout::_height;
bool ofxSpout::_isSpoutTextureShared;

//--------------------------------------------------------------
void ofxSpout::init(string senderName, float width, float height)
{
#ifdef TARGET_WIN32
	_isSpoutInitialized = false;
	_isSpoutDoneOnce = false;
	_width = width;
	_height = height;
	strcpy_s(_spoutSenderName, senderName.c_str());

	// init texture
	if (_spoutTexture != NULL)
	{
		glDeleteTextures(1, &_spoutTexture);
		_spoutTexture = NULL;
	}
	glGenTextures(1, &_spoutTexture); // Generate one texture
	glBindTexture(GL_TEXTURE_2D, _spoutTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif
}

//--------------------------------------------------------------
void ofxSpout::initSender()
{
#ifdef TARGET_WIN32
	// A render window must be available for Spout initialization
	// and might not be available in "update", so do it now 
	// when there is definitely a render window.
	if (!_isSpoutDoneOnce)
	{ 
		// INITIALIZE A SENDER
		_isSpoutInitialized = InitSender(_spoutSenderName, _width, _height, _isSpoutTextureShared);
		// _isSpoutTextureShared is returned to indicate whether texture share compatible or not
		_isSpoutDoneOnce = true; // Only do this once
		return; // quit for the next round
	}
#endif
}

//--------------------------------------------------------------
void ofxSpout::sendTexture()
{
#ifdef TARGET_WIN32
	if (_isSpoutInitialized)
	{
		// Grab the screen (current read buffer) into the local spout texture
		glBindTexture(GL_TEXTURE_2D, _spoutTexture); 
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
		glBindTexture(GL_TEXTURE_2D, 0); 

		//
		// Send the texture out for all receivers to use
		//
		// Notes :
		// (1)	If a host calls SendTexture with a framebuffer object actively bound
		//		it must provide the GL handle to its EXT_framebuffer_object
		//		so that the dll can restore that binding because it makes use of its
		//		own FBO for intermediate rendering - default is 0 for no bound host FBO - see Spout.h
		// (2)	openGL/DirectX coordinates make our texture come out inverted so invert
		//		when transferring it to the shared texture. Default bInvert flag is true so you
		//		have to specify false to disable this default then the result comes out inverted.
		//
		SendTexture(_spoutTexture, GL_TEXTURE_2D, _width, _height);
	}
#endif
}

//--------------------------------------------------------------
void ofxSpout::exit()
{
#ifdef TARGET_WIN32
	ReleaseSender();
	CloseSpout();
#endif
}

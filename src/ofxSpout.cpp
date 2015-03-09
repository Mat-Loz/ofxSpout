#include "ofxSpout.h"

// static members
bool ofxSpout::_isSender;
bool ofxSpout::_isSpoutInitialized;
bool ofxSpout::_isSpoutDoneOnce;
GLuint ofxSpout::_spoutTexture;
char ofxSpout::_spoutSenderName[256];
float ofxSpout::_width;
float ofxSpout::_height;
bool ofxSpout::_isSpoutTextureShared;
ofTexture ofxSpout::_receiverTexture;


//--------------------------------------------------------------

void ofxSpout::init(string senderName, float width, float height, bool isSender)
{
	_isSender = isSender;
	_isSpoutInitialized = false;
	_isSpoutDoneOnce = false;
	_isSpoutTextureShared = false;
	_width = width;
	_height = height;
	strcpy_s(_spoutSenderName, senderName.c_str());

	if (_isSender)
	{
		// init sender texture
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
	}
	else
	{
		// init receiver texture
		_receiverTexture.allocate(_width, _height, GL_RGBA);
	}
}

//--------------------------------------------------------------

void ofxSpout::initSender()
{
	// A render window must be available for Spout initialization
	// and might not be available in "update", so do it now 
	// when there is definitely a render window.
	if (!_isSpoutDoneOnce)
	{ 
		// INITIALIZE A SENDER
		_isSpoutInitialized = InitSender(_spoutSenderName, _width, _height, _isSpoutTextureShared);
		// _isSpoutTextureShared is returned to indicate whether texture share compatible or not
		_isSpoutDoneOnce = true; // Only do this once
	}
}

//--------------------------------------------------------------

void ofxSpout::sendTexture()
{
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

		SendTexture(_spoutTexture, GL_TEXTURE_2D, _width, _height, true); // invert = true
	}
}

//--------------------------------------------------------------

void ofxSpout::initReceiver()
{
	// A render window must be available for Spout initialization
	// and might not be available in "update", so do it now 
	// when there is definitely a render window.
	if (!_isSpoutDoneOnce)
	{ 
		// INITIALIZE A RECEIVER
		unsigned int width = _width;	// pass the initial width and height (they will be adjusted if necessary)
		unsigned int height = _height;
		char tempname[256] = "";
		_isSpoutInitialized = InitReceiver(tempname, width, height, _isSpoutTextureShared);

		// Is the size of the detected sender different from the current texture size ?
		// This is detected for both texture share and memoryshare
		if (_isSpoutInitialized)
		{
			// Check to see whether it has initialized texture share or memoryshare
			if(_isSpoutTextureShared)
			{ 
				// Texture share is OK so we can look at sender names
				// Check if the name returned is different.
				if(strcmp(_spoutSenderName, tempname) != 0)
				{
					// If the sender name is different, the requested 
					// sender was not found so the active sender was used.
					// Act on this if necessary.
					strcpy_s(_spoutSenderName, 256, tempname);
				}
			}
			// else the receiver has initialized in memoryshare mode

			if (width != _width || height != _height)
			{
				// Reset the local receiving texture size
				_width = width;
				_height = height;
				// Adjust the receiving texture
				_receiverTexture.allocate(_width, _height, GL_RGBA);
				// reset render window
				ofSetWindowShape(_width, _height);
			}
			_isSpoutDoneOnce = true; // Only do this once
		}
		else
		{
			ofDrawBitmapStringHighlight("No sender detected", 20, 20);
		}
	}
}

//--------------------------------------------------------------
void ofxSpout::receiveTexture()
{
	if (_isSpoutInitialized)
	{
		unsigned int width = _width;
		unsigned int height = _height;

		// Try to receive the texture at the current size 
		//
		// NOTE : if the host calls ReceiveTexture with an FBO bound
		// that binding must be restored after the call because Spout makes use of its
		// own FBO for intermediate rendering		
		if (!ReceiveTexture(_spoutSenderName, _receiverTexture.getTextureData().textureID, _receiverTexture.getTextureData().textureTarget, width, height))
		{
			//
			// Receiver failure :
			//	1)	width and height are zero for read failure.
			//	2)	width and height are changed for sender change
			//		The local texture then has to be resized.
			//
			if (width == 0 || height == 0)
			{
				// width and height are returned zero if there has been 
				// a texture read failure which might happen if the sender
				// is closed. Spout will keep trying and if the same sender opens again
				// will use it. Otherwise the user can select another sender.
				return;
			}

			if (width != _width || height != _height)
			{
				// The sender dimensions have changed
				// Update the global width and height
				_width  = width;
				_height = height;
				// Update the local texture to receive the new dimensions
				_receiverTexture.allocate(_width, _height, GL_RGBA);
				// reset render window
				ofSetWindowShape(_width, _height);
				return; // quit for next round
			}
		}
		else
		{
			// draw the texture and fill the screen
			_receiverTexture.draw(0, 0, ofGetWidth(), ofGetHeight());
			return;
		}
	}
}

//--------------------------------------------------------------

void ofxSpout::exit()
{
	if (_isSender)
		ReleaseSender();
	else
		ReleaseReceiver();
	CloseSpout();
}

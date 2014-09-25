#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	_red = 0;
	_isIncreasing = true;

	// initialize Spout with a sender name, and a texture size
	ofxSpout::init("ofxSpout example", ofGetWidth(), ofGetHeight());	
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (_isIncreasing)
		_red++;
	else
		_red--;

	if (_red > 254)
		_isIncreasing = false;
	else if (_red < 0)
		_isIncreasing = true;
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// init sender if it's not already initialized
	ofxSpout::initSender();

	// clear background with a red color
	ofClear(_red, 0, 0);
	
	// send screen to Spout
	ofxSpout::sendTexture();

	// this text is not sent to spout, because it's drawn after the sendTexture call
	ofDrawBitmapStringHighlight("ofxSpout example", 20, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	// exit spout
	ofxSpout::exit();
}


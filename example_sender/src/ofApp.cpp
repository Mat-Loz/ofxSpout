#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("ofxSpout sender example");

	_red = 0;
	_isIncreasing = true;

	// initialize Spout with a sender name, and a texture size
	ofxSpout::init("ofxSpout sender example", ofGetWidth(), ofGetHeight(), true);	
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

	// set background with a red color
	ofSetColor(_red, 0, 0);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());

	// add a yellow circle
	ofSetColor(ofColor::yellow);
	ofCircle(ofGetWidth() / 2 + ofSignedNoise(ofGetElapsedTimef()) * ofGetWidth() / 2, ofGetHeight() / 3 + ofNoise(ofGetElapsedTimef() * 2) * ofGetHeight() / 2, 40);

	// send screen to Spout
	ofxSpout::sendTexture();

	// this text is not sent to spout, because it's drawn after the sendTexture call
	ofDrawBitmapStringHighlight("ofxSpout sender example", 20, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	// exit spout
	ofxSpout::exit();
}


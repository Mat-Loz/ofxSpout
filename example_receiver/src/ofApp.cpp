#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("ofxSpout receiver example");

	// initialize Spout as a receiver
	ofxSpout::init("", ofGetWidth(), ofGetHeight(), false);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// init receiver if it's not already initialized
	ofxSpout::initReceiver();

	// receive Spout texture
	ofxSpout::receiveTexture();

	ofDrawBitmapStringHighlight("ofxSpout receiver example", ofGetWidth() - 220, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	// exit spout
	ofxSpout::exit();
}


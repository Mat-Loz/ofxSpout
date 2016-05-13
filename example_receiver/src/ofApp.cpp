#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("ofxSpout receiver example");

	// initialize Spout as a receiver
	ofxSpout::init("", receiver, ofGetWidth(), ofGetHeight(), false);
}

void ofApp::update() {
	// init receiver if it's not already initialized
	ofxSpout::initReceiver(receiver);

	// receive Spout texture
	ofxSpout::receiveTexture(receiver);
}

//--------------------------------------------------------------
void ofApp::draw()
{

	receiver.draw(0,0);

	ofDrawBitmapStringHighlight("ofxSpout receiver example", ofGetWidth() - 220, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	// exit spout
	ofxSpout::exit();
}


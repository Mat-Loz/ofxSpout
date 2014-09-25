#pragma once

#include "ofMain.h"
#include "ofxSpout.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();

private:
	int _red;
	bool _isIncreasing;
};

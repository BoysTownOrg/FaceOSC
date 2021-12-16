#pragma once

#include "ofxFaceTracker.h"
#include "ofxOsc.h"

class FaceOsc {
public:
	void clearBundle();
	void addMessage(string address, ofVec3f data);
	void addMessage(string address, ofVec2f data);
	void addMessage(string address, float data);
	void addMessage(string address, int data);
	void sendBundle();
    
	ofxOscSender osc;
	ofxOscBundle bundle;
};

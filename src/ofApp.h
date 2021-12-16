#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "FaceOsc.h"

#include <fstream>

class ofApp : public ofBaseApp, public FaceOsc {
public:
	void loadSettings();

	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	void setVideoSource(bool useCamera);
    
    std::ofstream outputFile;
    
    ofParameter<bool> bIncludeGestures;
    ofParameter<bool> bIncludeAllVertices;
    ofParameter<bool> bNormalizeRaw;
    
    string host;
    int port;
	bool bUseCamera, bPaused;

	int camWidth, camHeight;
	int movieWidth, movieHeight;
	int sourceWidth, sourceHeight;

	ofVideoPlayer movie;
	ofBaseVideoDraws *videoSource;
    
	ofxFaceTracker tracker;
    glm::mat4 rotationMatrix;
	
	bool bDrawMesh;
    
    ofxPanel gui;
    bool bGuiVisible;
};

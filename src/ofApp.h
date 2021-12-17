#pragma once

#include "FaceOsc.h"
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

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

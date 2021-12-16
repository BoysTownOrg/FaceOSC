#include "ofApp.h"
#include "ofxXmlPoco.h"

using namespace ofxCv;
using namespace cv;

void ofApp::loadSettings() {
    // if you want to package the app by itself without an outer
    // folder, you can place the "data" folder inside the app in
    // the Resources folder (right click on the app, "show contents")
    //ofSetDataPathRoot("../Resources/data/");
    
    // setup gui
    bGuiVisible = true;
    gui.setup();
    gui.setName("FaceOSC");
    gui.setPosition(0, 0);
    gui.add(bIncludeGestures.set("gesture", true));
    gui.add(bIncludeAllVertices.set("raw", false));
    gui.add(bNormalizeRaw.set("normalize raw", false));
	
	// expects following tags to be wrapped by a main "faceosc" tag

    const auto result{ofSystemLoadDialog("load a video")};
    const auto filename = result.filePath;
    if(filename != "") {
        if(!movie.load(filename)) {
            ofLog(OF_LOG_ERROR, "Could not load movie \"%s\"", filename.c_str());
        }
        movie.play();
    }
    movie.setVolume(1.0);
    movie.setSpeed(1.0);
	bPaused = false;
	movieWidth = movie.getWidth();
	movieHeight = movie.getHeight();

    ofSetWindowShape(movieWidth, movieHeight);
    setVideoSource(false);

    tracker.setRescale(1.0);
    tracker.setIterations(5);
    tracker.setClamp(3.0);
    tracker.setTolerance(0.01);
    tracker.setAttempts(1);
	bDrawMesh = true;
	tracker.setup();
	host = "localhost";
	port = 8338;
	osc.setup(host, port);
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	loadSettings();
}

void ofApp::update() {
	if(bPaused)
		return;

	videoSource->update();
	if(videoSource->isFrameNew()) {
		tracker.update(toCv(*videoSource));
        sendFaceOsc(tracker);
		rotationMatrix = tracker.getRotationMatrix();
	}
}

void ofApp::draw() {
	ofSetColor(255);
	videoSource->draw(0, 0);

	if(tracker.getFound()) {
        ofDrawBitmapString("framerate "+ofToString((int) ofGetFrameRate())+", sending OSC to "+host+":"+ofToString(port), 10, ofGetHeight()-4);

		if(bDrawMesh) {
			ofSetLineWidth(1);
			//tracker.draw();
			tracker.getImageMesh().drawWireframe();

			ofPushView();
			ofSetupScreenOrtho(sourceWidth, sourceHeight, -1000, 1000);
			ofVec2f pos = tracker.getPosition();
			ofTranslate(pos.x, pos.y);
			applyMatrix(rotationMatrix);
			ofScale(10,10,10);
			ofDrawAxis(tracker.getScale());
			ofPopView();
		}
	} else {
        ofSetColor(255);
        ofDrawBitmapString("searching for face...", 10, ofGetHeight()-4);
	}

	if(bPaused) {
		ofSetColor(255, 0, 0);
		ofDrawBitmapString( "paused", 10, 32);
	}

    ofSetColor(255, 0, 0);
    ofDrawBitmapString("speed "+ofToString(movie.getSpeed()), ofGetWidth()-100, 20);
    
    if(bGuiVisible) {
        gui.draw();
    }
}

void ofApp::keyPressed(int key) {
	switch(key) {
		case 'r':
			tracker.reset();
			break;
		case 'm':
			bDrawMesh = !bDrawMesh;
			break;
		case 'p':
			bPaused = !bPaused;
			break;
        case 'g':
            bGuiVisible = !bGuiVisible;
            break;
		case OF_KEY_UP:
			movie.setSpeed(ofClamp(movie.getSpeed()+0.2, -16, 16));
			break;
		case OF_KEY_DOWN:
			movie.setSpeed(ofClamp(movie.getSpeed()-0.2, -16, 16));
			break;
	}
}

void ofApp::setVideoSource(bool useCamera) {
    videoSource = &movie;
    sourceWidth = movieWidth;
    sourceHeight = movieHeight;
}

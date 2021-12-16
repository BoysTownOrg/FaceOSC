#include "ofApp.h"
#include "ofxXmlPoco.h"

#include <sstream>

using namespace ofxCv;
using namespace cv;

static auto putWithTrailingComma(std::ostream &stream, const std::string &s) -> std::ostream & {
    return stream << s << ',';
}

static auto putWithTrailingComma(std::ostream &stream, float s) -> std::ostream & {
    return stream << s << ',';
}

void ofApp::loadSettings() {
    // if you want to package the app by itself without an outer
    // folder, you can place the "data" folder inside the app in
    // the Resources folder (right click on the app, "show contents")
    ofSetDataPathRoot("../Resources/data/");
    
    // setup gui
    bGuiVisible = true;
    gui.setup();
    gui.setName("FaceOSC");
    gui.setPosition(0, 0);
    gui.add(bIncludeGestures.set("gesture", true));
    gui.add(bIncludeAllVertices.set("raw", false));
    gui.add(bNormalizeRaw.set("normalize raw", false));
	
    {
        const auto result{ofSystemLoadDialog("load a video")};
        const auto filename = result.filePath;
        if(filename != "") {
            if(!movie.load(filename)) {
                ofLog(OF_LOG_ERROR, "Could not load movie \"%s\"", filename.c_str());
            }
        }
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
    
    
    const auto result{ofSystemSaveDialog("out.csv", "save results")};
    const auto filename = result.filePath;
    outputFile.open(filename);
    putWithTrailingComma(outputFile, "mouthWidth");
    putWithTrailingComma(outputFile, "mouthHeight");
    putWithTrailingComma(outputFile, "leftEyebrowHeight");
    putWithTrailingComma(outputFile, "rightEyebrowHeight");
    putWithTrailingComma(outputFile, "leftEyeOpenness");
    putWithTrailingComma(outputFile, "rightEyeOpenness");
    putWithTrailingComma(outputFile, "jawOpenness");
    putWithTrailingComma(outputFile, "nostrilFlare");
    for (auto i{0}; i < 67; ++i) {
        std::stringstream stream;
        stream << "vertices/" << i << '/';
        putWithTrailingComma(outputFile, stream.str() + 'x');
        putWithTrailingComma(outputFile, stream.str() + 'y');
    }
    outputFile << "end";
	host = "localhost";
	port = 8338;
	osc.setup(host, port);
    
    movie.setLoopState(OF_LOOP_NONE);
    movie.play();
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
        clearBundle();
        
        if(tracker.getFound()) {
            
            if (bIncludeGestures) {
                putWithTrailingComma(outputFile, tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH));
                putWithTrailingComma(outputFile, tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT));
                putWithTrailingComma(outputFile, tracker.getGesture(ofxFaceTracker::LEFT_EYEBROW_HEIGHT));
                putWithTrailingComma(outputFile, tracker.getGesture(ofxFaceTracker::RIGHT_EYEBROW_HEIGHT));
                putWithTrailingComma(outputFile, tracker.getGesture(ofxFaceTracker::LEFT_EYE_OPENNESS));
                putWithTrailingComma(outputFile, tracker.getGesture(ofxFaceTracker::RIGHT_EYE_OPENNESS));
                putWithTrailingComma(outputFile, tracker.getGesture(ofxFaceTracker::JAW_OPENNESS));
                putWithTrailingComma(outputFile, tracker.getGesture(ofxFaceTracker::NOSTRIL_FLARE));
            }
            
            for(ofVec2f p : tracker.getImagePoints()) {
                putWithTrailingComma(outputFile, p.x);
                putWithTrailingComma(outputFile, p.y);
            }
            
            putWithTrailingComma(outputFile, "end");

        } else {
        }
        outputFile << "end\n";
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

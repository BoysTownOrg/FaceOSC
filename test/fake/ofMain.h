#ifndef BOYSTOWNORG_FACEOSC_TEST_FAKE_OFMAIN_H_
#define BOYSTOWNORG_FACEOSC_TEST_FAKE_OFMAIN_H_

#include <string>
#include <vector>

using namespace std;

struct ofVec2f {
  float x;
  float y;
};

struct DialogResult {
  std::string filePath;
};

enum LogReason { OF_LOG_ERROR };

inline std::string ofToString(int) { return {}; }

namespace glm {
struct mat4 {};
} // namespace glm

enum { OF_KEY_UP, OF_KEY_DOWN };

inline float ofClamp(float, float, float) { return 0; }

class ofBaseApp {
public:
  DialogResult systemLoadDialogResult;
  std::string defaultSystemSaveDialogFilename;

  void ofSetDataPathRoot(const std::string &) {}

  DialogResult ofSystemLoadDialog(const std::string &) {
    return systemLoadDialogResult;
  }

  DialogResult ofSystemSaveDialog(const std::string &s, const std::string &) {
    defaultSystemSaveDialogFilename = s;
    return {};
  }

  void ofLog(LogReason, const std::string &, ...) {}

  void ofSetWindowShape(int, int) {}

  void ofSetVerticalSync(bool) {}

  void ofSetColor(int red, int green = 0, int blue = 0) {}

  float ofGetFrameRate() { return 0; }

  int ofGetHeight() { return 0; }

  int ofGetWidth() { return 0; }

  void ofSetLineWidth(int) {}

  void ofPushView() {}

  void ofSetupScreenOrtho(int, int, int, int) {}

  void ofTranslate(float, float) {}

  void applyMatrix(glm::mat4) {}

  void ofScale(int, int, int) {}

  void ofDrawAxis(float) {}

  void ofPopView() {}

  void ofDrawBitmapString(const std::string &, int, int) {}
};

class ofBaseVideoDraws {
public:
  void update() {}
  bool isFrameNew() { return true; }
  void draw(int, int) {}
};

enum LoopState { OF_LOOP_NONE };

class ofVideoPlayer : public ofBaseVideoDraws {
public:
  std::string loadedPath;

  bool load(const std::string &s) {
    loadedPath = s;
    return true;
  }

  void setVolume(float) {}

  void setSpeed(float) {}

  float getSpeed() { return 0; }

  int getWidth() { return 0; }

  int getHeight() { return 0; }

  void play() {}

  void setLoopState(LoopState) {}
};

struct cvType {};

inline cvType toCv(ofBaseVideoDraws &) { return {}; }

class Mesh {
public:
  void drawWireframe() {}
};

class ofxFaceTracker {
public:
  void setRescale(float) {}
  void setIterations(int) {}
  void setClamp(float) {}
  void setTolerance(float) {}
  void setAttempts(int) {}
  void setup() {}
  void update(const cvType &) {}
  bool getFound() { return true; }

  enum Feature {
    MOUTH_WIDTH,
    MOUTH_HEIGHT,
    LEFT_EYEBROW_HEIGHT,
    RIGHT_EYEBROW_HEIGHT,
    LEFT_EYE_OPENNESS,
    RIGHT_EYE_OPENNESS,
    JAW_OPENNESS,
    NOSTRIL_FLARE
  };

  float getGesture(Feature) { return 0; }

  std::vector<ofVec2f> getImagePoints() { return {}; }

  glm::mat4 getRotationMatrix() { return {}; }

  ofVec2f getPosition() { return {}; }

  float getScale() { return 0; }

  Mesh getImageMesh() { return {}; }

  void reset() {}
};

#endif

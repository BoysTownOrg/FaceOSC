#include "ofApp.h"

#include <sbash64/testcpplite/testcpplite.hpp>

#include <iostream>

static void loadsVideo(sbash64::testcpplite::TestResult &result) {
  ofApp app;
  app.systemLoadDialogResult.filePath =
      "/Users/abcdef/Documents/videos/mr-saturn.mov";
  app.loadSettings();
  assertEqual(result, "/Users/abcdef/Documents/videos/mr-saturn.mov",
              app.movie.loadedPath);
}

static void usesVideoFileStemAsDefaultOutputFileStem(
    sbash64::testcpplite::TestResult &result) {
  ofApp app;
  app.systemLoadDialogResult.filePath =
      "/Users/abcdef/Documents/videos/mr-saturn.mov";
  app.loadSettings();
  assertEqual(result, "mr-saturn.csv", app.defaultSystemSaveDialogFilename);
}

int main() {
  return sbash64::testcpplite::test(
      {{loadsVideo, "loads video"},
       {usesVideoFileStemAsDefaultOutputFileStem,
        "uses video file name to determine default output file name"}},
      std::cout);
}
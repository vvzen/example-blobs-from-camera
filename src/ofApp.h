#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		
        ofImage input_image;
        ofImage threshold_image;
        ofxCv::ContourFinder contour_finder;
        ofVideoGrabber cam;
    
        vector<ofRectangle> detected_blobs;
    
        bool show_GUI = true;
        bool find_next = true;
        ofxPanel gui;
        ofParameter<float> GUI_min_area, GUI_max_area, GUI_threshold, GUI_blur;
        ofParameter<bool> GUI_show_holes, GUI_erode, GUI_dilate;
};

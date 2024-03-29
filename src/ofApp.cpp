#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // This is the image we'll use to detect the blobs
    threshold_image.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    
    // CAMERA
    cam.setup(640, 480);
    
    // GUI
    gui.setup();
    gui.add(GUI_min_area.set("Min area", 10, 1, 900));
    gui.add(GUI_max_area.set("Max area", 1000, 1, 20000));
    gui.add(GUI_threshold.set("Threshold", 128, 0, 255));
    gui.add(GUI_blur.set("Blur", 10, 0, 100));
    gui.add(GUI_show_holes.set("Holes", false));
    gui.add(GUI_erode.set("Erode", false));
    gui.add(GUI_dilate.set("Dilate", false));
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    cam.update();
    
    if(cam.isFrameNew()) {
        
        // Save the webcam frame into the threshold image
        // and make it grayscale
        ofxCv::convertColor(cam, threshold_image, CV_RGB2GRAY);
        
        // These three filters help to reduce the noise
        ofxCv::blur(threshold_image, GUI_blur);
        if (GUI_dilate) ofxCv::dilate(threshold_image);
        if (GUI_erode) ofxCv::erode(threshold_image);
        
        // If you need to, you can invert the image
        // ofxCv::invert(threshold_image);
        
        // Apply the threshold filter
        ofxCv::threshold(threshold_image, GUI_threshold);
        threshold_image.update();
        
        // the contour finder is used to find the blobs
        contour_finder.setMinArea(GUI_min_area);
        contour_finder.setMaxArea(GUI_max_area);
        contour_finder.setThreshold(GUI_threshold);
        contour_finder.setFindHoles(GUI_show_holes);
        
        contour_finder.findContours(threshold_image);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    cam.draw(0, 0);
    threshold_image.draw(640, 0);
    
    ofDrawBitmapStringHighlight("Num of blobs: " + ofToString(contour_finder.size()), 640, 20);
    
    ofPushStyle();
    ofNoFill();
    ofColor blob_color;
    blob_color.set(255, 255, 255);
    
    // If we've found any blobs
    if (contour_finder.getBoundingRects().size() > 0){
        
        // Loop through each blob
        for (cv::Rect rect: contour_finder.getBoundingRects()){
            
            if (ofGetMousePressed()){
            
                int previous_size = detected_blobs.size();
                ofLogNotice() << "previous size: " << previous_size;
                
                // You could use these numbers in order to check if the mouse is nearby a blob
                //            ofLogNotice() << "x: " << rect.x << ", y: " << rect.y << ", width: " << rect.width << ", " << rect.height;
                
                // When we click on a blob
                if (rect.contains(cv::Point2d(ofGetMouseX(), ofGetMouseY()))){
                    
                    blob_color.set(0, 255, 0);
                    
                    ofLogNotice() << "adding blob";
                    
                    // After we find a blob, we add it to the detected_blobs
                    // You will need to press f again to find another one
                    if (find_next){
                        detected_blobs.push_back(ofRectangle(rect.x, rect.y, rect.width, rect.height));
                        find_next = false;
                    }
                }
            }
            
            // Draw the current blobs
            ofSetColor(blob_color);
            ofDrawRectangle(rect.x, rect.y, rect.width, rect.height);
        }
    }
    ofPopStyle();
    
    ofPushStyle();
    ofNoFill();
    
    // Draw all the blobs we've already detected
    int c = 0;
    ofDrawBitmapStringHighlight("Num of detected blobs: " + ofToString(detected_blobs.size()), 640, 40);
    for (ofRectangle blob: detected_blobs){
        
        ofSetColor(0, 0, 255);
        ofDrawBitmapString("blob " + ofToString(c), blob.x, blob.y);
        ofDrawRectangle(blob);
        c++;
    }
    
    ofPopStyle();
    
    // When i is pressed
    if (show_GUI) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case 'i':
            show_GUI = !show_GUI;
            break;
            
        case 'f':
            find_next = true;
            break;
    }
}

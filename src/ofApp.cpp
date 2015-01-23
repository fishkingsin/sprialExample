#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    gui.setup();
    gui.add(maxR.set("MAXR",100,1,100));
    gui.add(numStep.set("numStep",10,1,360));
    gui.add(n.set("N",100,10,10000));
    gui.add(start.set("start"));
    start.enableEvents();
    
//    ofAddListener(start, this , &ofApp::isStart);
    startTime = 0;
}
void ofApp::isStart(bool &_Start)
{
    startTime  = ofGetElapsedTimef();
}
//--------------------------------------------------------------
void ofApp::update(){
//    gui.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
        trianglate.reset();
//    int maxR = 1000;
//    int n = 1000;
    if(start)
    {
    float step = 0;
    numStep=sin((ofGetElapsedTimef()-startTime)*0.001*TWO_PI)*numStep.getMax();
    for(int i = 0 ; i < n ; i++)
    {
        step+=numStep;
        float x = sin(((step)/n)*TWO_PI)*maxR*(step/n)*0.25 + ofGetWidth()*0.5;
        float y = cos(((step)/n)*TWO_PI)*maxR*(step/n)*0.25 + ofGetHeight()*0.5;
        ofPushStyle();
        ofCircle(x, y, 10);
        ofPopStyle();
        trianglate.addPoint(ofPoint(x,y));
    }
    ofPushStyle();
    ofNoFill();
    trianglate.triangulate();
    trianglate.draw();
    ofPopStyle();
    }
        gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

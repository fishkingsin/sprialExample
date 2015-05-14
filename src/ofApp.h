#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDelaunay.h"
class Particle
{
public:
    
    ofVec2f pos;
    ofVec2f vel;
    
    int myID;
    
    int spacePartitioningIndexX;
    int spacePartitioningIndexY;
};

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    ofxPanel gui;
    ofParameter<float>maxR;
    ofParameter<float>maxStep;
    ofParameter<float>minStep;
    ofParameter<float>numStep;
    ofParameter<int >n;
    ofxDelaunay trianglate;
    float startTime;
    ofParameter<bool>start;
    ofParameter <float>theInitSzie;
    void isStart(bool &_Start);
    ofParameter <float> increasement;
    float count;
    ofMesh				lineMesh;
    float				lastUpdateTime;
    
    vector< Particle* > particles;
    
    vector< vector< vector< Particle* > > > spacePartitioningGrid;
    int					spacePartitioningResX;
    int					spacePartitioningResY;
    float				spacePartitioningGridWidth;
    float				spacePartitioningGridHeight;
};

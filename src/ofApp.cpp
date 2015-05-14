#include "ofApp.h"
bool showGUI;
//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    gui.setup();
    gui.add(maxR.set("MAXR",100,1,100));
    gui.add(numStep.set("numStep",10,1,360));
    gui.add(maxStep.set("maxStep",10,1,360));
    gui.add(minStep.set("minStep",10,1,360));
    gui.add(n.set("N",100,10,10000));
        gui.add(theInitSzie.set("theInitSzie",50,0,100));
    gui.add(increasement.set("Increasment",0.001,0.0001,1));
    gui.add(start.set("start"));

    
    start.enableEvents();
    int particleAmount = 100;
    for( int i = 0; i < particleAmount; i++ )
    {
        Particle* tmpParticle = new Particle();
        tmpParticle->pos.set( ofRandomWidth(), ofRandomHeight() );
        
        float tmpAngle = ofRandom( PI * 2.0f );
        float magnitude = 20.0f; // pixels per second
        tmpParticle->vel.set( cosf(tmpAngle) * magnitude, sinf(tmpAngle) * magnitude );
        
        tmpParticle->spacePartitioningIndexX = 0;
        tmpParticle->spacePartitioningIndexY = 0;
        
        tmpParticle->myID = i;
        
        particles.push_back( tmpParticle );
    }
//    ofAddListener(start, this , &ofApp::isStart);
    startTime = 0;
    // Initialize storage we will use to optimize particle-to-particle distance checks
    spacePartitioningResX = 20;
    spacePartitioningResY = 20;
    
    spacePartitioningGridWidth = ofGetWidth() / (float)spacePartitioningResX;
    spacePartitioningGridHeight= ofGetHeight() / (float)spacePartitioningResY;
    
    for( int y = 0; y < spacePartitioningResY; y++ )
    {
        spacePartitioningGrid.push_back( vector< vector< Particle* > >() );
        for( int x = 0; x < spacePartitioningResX; x++ )
        {
            spacePartitioningGrid.at(y).push_back( vector< Particle* >() );
        }
    }
    
}
void ofApp::isStart(bool &_Start)
{
    startTime  = ofGetElapsedTimef();
}
//--------------------------------------------------------------
void ofApp::update(){
//    gui.update();
    float currTime = ofGetElapsedTimef();
    float timeDelta = currTime - lastUpdateTime;
    lastUpdateTime = currTime;
    float step = 0;
    count+=timeDelta*increasement;
    numStep=sin((count/maxStep)*PI)*maxStep+minStep;
    float initSzie = theInitSzie;
   

    for( unsigned int i = 0; i < particles.size(); i++ )
    {
        Particle* tmpParticle = particles.at(i);
        step+=numStep;
        float x = sin(((step)/particles.size())*TWO_PI)*maxR*(step/particles.size())*0.25 + ofGetWidth()*0.5;
        float y = cos(((step)/particles.size())*TWO_PI)*maxR*(step/particles.size())*0.25 + ofGetHeight()*0.5;
        tmpParticle->pos.set(x,y);
        //        ofPushStyle();
        //        ofCircle(x, y,10);//* MAX(1,centerX - ofDist(centerX, centerY, x,y))/centerX);
        //        ofPopStyle();
        //        trianglate.addPoint(ofPoint(x,y));
    }
    // clear the space partitioning lists
    for( int y = 0; y < spacePartitioningResY; y++ )
    {
        for( int x = 0; x < spacePartitioningResX; x++ )
        {
            spacePartitioningGrid.at(y).at(x).clear();
        }
    }
    // add particles into the space partitioning grid
    for( unsigned int i = 0; i < particles.size(); i++ )
    {
        Particle* tmpParticle = particles.at(i);
        
        int tmpIndexX = tmpParticle->pos.x / spacePartitioningGridWidth;
        int tmpIndexY = tmpParticle->pos.y / spacePartitioningGridHeight;
        
        if( tmpIndexX < 0 )  tmpIndexX = 0;
        if( tmpIndexX >= spacePartitioningResX ) tmpIndexX = spacePartitioningResX-1;
        
        if( tmpIndexY < 0 )  tmpIndexY = 0;
        if( tmpIndexY >= spacePartitioningResY ) tmpIndexY = spacePartitioningResY-1;
        
        tmpParticle->spacePartitioningIndexX = tmpIndexX;
        tmpParticle->spacePartitioningIndexY = tmpIndexY;
        
        spacePartitioningGrid.at(tmpIndexY).at(tmpIndexX).push_back( tmpParticle );
    }
    
    // Now we update the line mesh, to do this we check each particle against every other particle, if they are
    // within a certain distance we draw a line between them. As this quickly becoems a pretty insane amount
    // of checks, we use our space partitioning scheme to optimize it all a little bit.
    
    lineMesh.clear();
    lineMesh.setMode( OF_PRIMITIVE_LINES );
    
    ofFloatColor scratchColor;
    scratchColor.set( 1.0f, 1.0f, 1.0f );
    
    float lineConnectionMaxDistance =  100;//ofMap( cosf( currTime / 10.0f ) , -1.0f, 1.0f, 10.0f, 100.0f); //   ofGetMouseY() / 10.0f;
    float lineConnectionMaxDistanceSquared = lineConnectionMaxDistance * lineConnectionMaxDistance;
    
    // how many slots do we need to check on each side?
    int spacePartitioningIndexDistanceX = ceil(lineConnectionMaxDistance / spacePartitioningGridWidth);
    int spacePartitioningIndexDistanceY = ceil(lineConnectionMaxDistance / spacePartitioningGridHeight);
    
    for( unsigned int particleIndex = 0; particleIndex < particles.size(); particleIndex++ )
    {
        Particle* tmpParticle = particles.at(particleIndex);
        
        // the particle knows where it is in the space partitioning grid, figure out which indices to loop between based
        // on how many slots the maximum line distance  can cover, then do a bounds check.
        int startIndexX = tmpParticle->spacePartitioningIndexX - spacePartitioningIndexDistanceX;
        if( startIndexX < 0 ) { startIndexX = 0; } if( startIndexX >= spacePartitioningResX ) { startIndexX = spacePartitioningResX-1;}
        
        int endIndexX   = tmpParticle->spacePartitioningIndexX + spacePartitioningIndexDistanceX;
        if( endIndexX < 0 ) { endIndexX = 0; } if( endIndexX >= spacePartitioningResX ) { endIndexX = spacePartitioningResX-1;}
        
        int startIndexY = tmpParticle->spacePartitioningIndexY - spacePartitioningIndexDistanceY;
        if( startIndexY < 0 ) { startIndexY = 0; } if( startIndexY >= spacePartitioningResY ) { startIndexY = spacePartitioningResY-1;}
        
        int endIndexY   = tmpParticle->spacePartitioningIndexY + spacePartitioningIndexDistanceY;
        if( endIndexY < 0 ) { endIndexY = 0; } if( endIndexY >= spacePartitioningResY ) { endIndexY = spacePartitioningResY-1;}
        
        for( int y = startIndexY; y < endIndexY; y++ )
        {
            for( int x = startIndexX; x < endIndexX; x++ )
            {
                for( unsigned int i = 0; i < spacePartitioningGrid.at(y).at(x).size(); i++ )
                {
                    Particle* tmpOtherParticle = spacePartitioningGrid.at(y).at(x).at(i);
                    if( tmpParticle->myID != tmpOtherParticle->myID )
                    {
                        ofVec2f diff = tmpParticle->pos - tmpOtherParticle->pos;
                        if( diff.lengthSquared() < lineConnectionMaxDistanceSquared )
                        {
                            scratchColor.a =  1.0f - (diff.length() / lineConnectionMaxDistance);
                            
                            lineMesh.addVertex( tmpParticle->pos );
                            lineMesh.addColor( scratchColor );
                            
                            lineMesh.addVertex( tmpOtherParticle->pos );
                            lineMesh.addColor( scratchColor );
                            
                            lineMesh.addIndex( lineMesh.getNumVertices() - 2 );
                            lineMesh.addIndex( lineMesh.getNumVertices() - 1 );
                        }
                    }
                }
            }
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
        trianglate.reset();
//    int maxR = 1000;
//    int n = 1000;
    if( start)
    {
        ofSetColor( 140, 140, 140 );
        for( int y = 0; y < spacePartitioningResY; y++ ) { ofLine( 0.0f, y * spacePartitioningGridHeight, ofGetWidth(), y * spacePartitioningGridHeight ); }
        for( int x = 0; x < spacePartitioningResY; x++ ) { ofLine( x * spacePartitioningGridWidth, 0.0f, x * spacePartitioningGridWidth, ofGetHeight() ); }
        
        ofSetColor( 100, 100, 100 );
        for( int y = 0; y < spacePartitioningResY; y++ )
        {
            for( int x = 0; x < spacePartitioningResX; x++ )
            {
                if( spacePartitioningGrid.at(y).at(x).size() > 0 )
                {
                    ofRect( x * spacePartitioningGridWidth, y * spacePartitioningGridHeight, spacePartitioningGridWidth, spacePartitioningGridHeight);
                }
            }
        }
        
    }
//    if(start)
    {
        int centerX = ofGetWidth()*0.5;
        int centerY = ofGetHeight()*0.5;
            lineMesh.draw();
//    ofPushStyle();
//    ofNoFill();
//    trianglate.triangulate();
//    trianglate.draw();
//    ofPopStyle();
        for( unsigned int i = 0; i < particles.size(); i++ )
        {
            ofCircle( particles.at(i)->pos, 3.0f );
        }
    }
      if(showGUI)
      {
          gui.draw();
      }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key)
    {
            case 'f':
            ofToggleFullscreen();
            break;
        case OF_KEY_TAB:
            showGUI = !showGUI;
            break;
    }
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

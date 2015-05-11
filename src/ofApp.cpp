#include "ofApp.h"
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60); //cap frame rate at 60 frames per second
    bool succ = true; //Boolean to load the image
    succ = image.loadImage("stars4.jpg");
    image.resize(200, 200); //resizes the image to speed up processing time
    if (!succ) {
        cerr << "loading image failed ...\n";
        
    }
    //applys a threshold so that we only create a vertex at the pixel locations where the intensity of the color is greater than our designated value.
    
    float time = ofGetElapsedTimef();	//Get time
    float intensityThreshold = 1.0;
    int w = image.getWidth(); //looped through each pixel in the image using getWidth and getHeight
    int h = image.getHeight();
    for (int x=0; x<w; ++x) {
        for (int y=0; y<h; ++y) {
            ofColor c = image.getColor(x, y);
            float intensity = c.getLightness(); //checked the intensity of each pixel's color
            if (intensity >= intensityThreshold) { //create a vertex at pixel location and colored it with the pixel's color
                float saturation = c.getSaturation(); //use the saturation of the color to change the z-coordinate.
                float z = ofMap(saturation, 0, 255, 1000, -1000); //push brighter colors in the positive z-direction
                //and whiter colors in the negative-z direction
                
                ofVec3f pos(4*x, 4*y, 0.0);// We shrunk our image by a factor of 4, so we need to multiply our pixel
                mesh.addVertex(pos);      // locations by 4 in order to have our mesh cover the openFrameworks window
                mesh.addColor(c);
                
                // It will create a ofVec3f with 3 random values
                // These will allow us to move the x, y and z coordinates of each vertex independently
                offsets.push_back(ofVec3f(ofRandom(0,1000000), ofRandom(0,1000000), ofRandom(0,1000000)));
            }
        }
    }
    
    float connectionDistance = 10; //Define the maximum distance a line can connect two points
    int numVerts = mesh.getNumVertices(); //gets the number of vertices
    for (int a=10; a<numVerts; ++a) { //for loop to get first pair of vertices
        ofVec3f verta = mesh.getVertex(a); //get the position of the ath vertex in our mesh
        for (int b=a+1; b<numVerts; ++b) { //for loop to get second pair of vertices
            ofVec3f vertb = mesh.getVertex(b); //get the position of the ath vertex in our mesh
            float distance = verta.distance(vertb); //finds the distance between the vertices
            if (distance <= connectionDistance) { //If the vertices are closer than a value
                mesh.addIndex(a);                 //set by connectionDistance, then we need to add a line between them.
                mesh.addIndex(b);
                ofEnableDepthTest(); //Stops the mesh at the back from shining through to the front
                
            }
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    int numVerts = mesh.getNumVertices();
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i); //We get the location of a vertex and store it in a variable called ver
        
        float time = ofGetElapsedTimef(); //find the time passed
        float timeScale = 15.0; //changes the speed of the moving vertices
        float displacementScale = 17; //changes how big the triangles are
        ofVec3f timeOffsets = offsets[i]; //These values allow us to move each vertex in a random x, y and z direction seemingly independently of the movement of the other vertices.
        
        
        // A typical design pattern for using Perlin noise uses a couple parameters:
        // ofSignedNoise(time*timeScale+timeOffset)*displacementScale
        //     ofSignedNoise(time) gives us noise values that change smoothly over time
        //     ofSignedNoise(time*timeScale) allows us to control the smoothness of our noise (smaller timeScale, smoother values)
        //     ofSignedNoise(time+timeOffset) allows us to use the same Perlin noise function to control multiple things and have them look as if they are moving independently
        //     ofSignedNoise(time)*displacementScale allows us to change the bounds of the noise from [-1, 1] to whatever we want
        // Combine all of those parameters together, and you've got some nice control over your noise
        
        //We move the x, y and z values of vert using ofSignedNoise() with a set of a few parameters.
        vert.x += (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
        vert.y += (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
        vert.z += (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
        mesh.setVertex(i, vert); //updates the position of our vertex
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //Creates a background gradient
    ofColor centerColor = ofColor(85, 78, 68); //define center color
    ofColor edgeColor(0, 0, 0); //define edge color
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR); //draws the gradient
    
    easyCam.begin();
    ofPushMatrix();
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    mesh.draw();
    ofPopMatrix();
    easyCam.end();
}

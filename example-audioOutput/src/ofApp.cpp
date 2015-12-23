#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(54, 54, 54);
	ofSetFrameRate(60);

    int ticksPerBuffer = 4;
    bufferSize = BUFFER_SIZE;
    inChan  = 0;
	outChan = 2;
	sampleRate = SR;
    
    phase1 = phase2 = 0;
    phaseAdder1 = phaseAdder2 = 0.0f;
    phaseAdderTarget1 = phaseAdderTarget2 = 0.0f;

    int midiMin = 21;//27.500 hz
    int midiMax = 108;//4186.0 hz
    
    filterBank.setup(bufferSize, midiMin, midiMax, outChan, BANDWITH, sampleRate, 1.0);
    filterBank.setColor(ofColor::orange);
    
    soundStream.setup(this, outChan, inChan, sampleRate, bufferSize, ticksPerBuffer);
    
}

//--------------------------------------------------------------

void ofApp::update(){


}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(225);
	ofNoFill();
	
    float chSz = bufferSize/3;
	// draw the left input channel:
	{
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(100, 15, 0);
        ofSetColor(225);
        ofDrawBitmapString("Left Channel", 4, 18);
        ofSetLineWidth(1);
        ofRect(0, 0, chSz, 200);
        ofSetColor(ofColor::orange);
        ofSetLineWidth(3);
        ofBeginShape();
        for (int i = 0; i < bufferSize; i++){
            ofVertex(i/(bufferSize/chSz), 100 - filterBank.getLeftBuffer()[i]*45);
        }
        ofEndShape(false);
        ofPopMatrix();
        ofPopStyle();
	}
	// draw the right input channel:
	{
        ofPushStyle();
		ofPushMatrix();
		ofTranslate(200+chSz, 15, 0);
		ofSetColor(225);
		ofDrawBitmapString("Right Channel", 4, 18);
		ofSetLineWidth(1);
		ofRect(0, 0, chSz, 200);
		ofSetColor(ofColor::orange);
		ofSetLineWidth(3);
        ofBeginShape();
        for (int i = 0; i < bufferSize; i++){
            ofVertex(i/(bufferSize/chSz), 100 - filterBank.getRightBuffer()[i]*45);
        }
        ofEndShape(false);
		ofPopMatrix();
        ofPopStyle();
	}
    
	//Draw FilterBank
	{
        ofPushStyle();
        ofPushMatrix();
        ofTranslate (100,250,0);
            filterBank.draw(800,400);
        ofPopMatrix();
        ofPopStyle();
	}
	ofSetColor(225);

	string reportString =  "Sampling Rate: "+ ofToString(SR) +"\nBuffer size: "+ ofToString(bufferSize);
	ofDrawBitmapString(reportString, 10, 700);


}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){

    
}


void ofApp::audioOut(float * output, int bufferSize, int nChannels){

    while (phase1 > TWO_PI){
        phase1 -= TWO_PI;
    }
    while (phase2 > TWO_PI){
        phase2 -= TWO_PI;
    }
    
    phaseAdder1 = 0.95f * phaseAdder1 + 0.05f * phaseAdderTarget1;
    phaseAdder2 = 0.95f * phaseAdder2 + 0.05f * phaseAdderTarget2;
    
    for (int i = 0; i < bufferSize; i++){
        phase1 += phaseAdder1;
        phase2 += phaseAdder2;
        float sample = sin(phase1) + sin(phase2);
        sample *= 0.5;
       output[i*nChannels    ] = sample;
       output[i*nChannels + 1] = sample;
    }
  
    //Analyze Output Buffer with ofxFilterbank
   filterBank.analyze(output);
  
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch (key){
        case 'q':
            filterBank.showAll = !filterBank.showAll;
			break;
        default:
            break;
    }
}


//--------------------------------------------------------------
void ofApp::exit(){
    soundStream.stop();
    soundStream.close();
    filterBank.exit();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------

void ofApp::mouseMoved(int x, int y ){
    
    float width = (float)ofGetWidth();
    float widhtPct = x / width;
    targetFrequency = ofClamp(4000.0f * widhtPct, 27.5, 4186.0);
    phaseAdderTarget1 = (targetFrequency / (float) sampleRate) * TWO_PI;
    phaseAdderTarget2 = (targetFrequency*1.5 / (float) sampleRate) * TWO_PI;//One Fifth UP

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


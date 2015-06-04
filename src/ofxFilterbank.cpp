/*
 * Copyright (c) 2015 Leo Zimmerman <leozimmerman@gmail.com>
 */

#include "ofxFilterbank.h"


//--------------------------------------------------------------
void ofxFilterbank::setup(int iBufferSize, int iMidiMin, int iMidiMax, int iChans, float iBw, int iSR, float iGain){

    bufferSize = iBufferSize;
    midiMaxVar = midiMax = iMidiMax;
    midiMinVar = midiMin = iMidiMin;
    channels = iChans;
    bw = iBw;
    sampleRate = iSR;
    gain = iGain;

    pitchDev = 0.0;
    maskAmnt = 0.0;
    smoothAmnt = 0.7;
    estimateMax = 0.2;

    treshold = 0.02;
    showAll = true;

    left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);


    energies.assign(midiMax, 0.0);
    smth_energies.assign(midiMax, 0.0);
    log_smth_energies.assign(midiMax, 0.0);

   fdata=(RESONDATA***)malloc(sizeof(RESONDATA**)*channels);
   for (int i=0; i<channels; i++) {
         fdata[i]=(RESONDATA**)malloc(sizeof(RESONDATA*)*midiMax);
         for (int n=midiMin; n<midiMax; n++){
            double exp = (double)(n-69+pitchDev)/12;
            double frec = pow (2., exp)*440;
            fdata[i][n]=(RESONDATA*)malloc(sizeof(RESONDATA));
            set_reson(frec, bw,fdata[i][n], sampleRate, gain);
            set_reson(frec, bw,fdata[i][n], sampleRate, gain);
        }
    }
    color.set(255);
}
//--------------------------------------------------------------
void ofxFilterbank::setPitchDev(){
    for (int i=0; i<channels; i++) {
        // fdata[i]=(RESONDATA**)malloc(sizeof(RESONDATA*)*midiMax);
         for (int n=midiMin; n<midiMax; n++){
            double exp = (double)(n-69+pitchDev)/12;
            double frec = pow (2., exp)*440;
          //  fdata[i][n]=(RESONDATA*)malloc(sizeof(RESONDATA));
            set_reson(frec, bw,fdata[i][n], sampleRate, gain);
            set_reson(frec, bw,fdata[i][n], sampleRate, gain);
        }
    }
}
//--------------------------------------------------------------
void ofxFilterbank::analyze(float * iBuffer){
    //Reset energies

    for (int n=midiMinVar; n<midiMaxVar; n++){
            energies[n] = 0.0;
    }
     //Process
    for (int i = 0; i < left.size(); i++){
        left[i] = iBuffer[i*channels    ] ;
        right[i]= iBuffer[i*channels + 1];

        for (int n=midiMinVar; n<midiMaxVar;n++){
            float lOutsample = 0.0;
            float rOutsample = 0.0;

            lOutsample = reson(left[i], fdata[0][n]);
            energies[n]  += lOutsample*lOutsample;
            if (channels>1){
                rOutsample = reson(right[i], fdata[1][n]);
                energies[n] += rOutsample*rOutsample;
            }
        }
	 }

	 //Smoothing...

    for (int n=midiMaxVar-1; n>=midiMinVar;n--){
        energies[n] /= (left.size()*channels);
        energies[n] = sqrt(energies[n]);
        smth_energies[n] *= smoothAmnt;
        smth_energies[n] += (1-smoothAmnt) * energies[n];
        ///mask
        if(smth_energies[n]>treshold  && n>midiMinVar+1){
            if (smth_energies[n]>smth_energies[n-1])
                smth_energies[n-1] -= smth_energies[n]*maskAmnt;
            else
                smth_energies[n] -= smth_energies[n-1]*maskAmnt;
    
            if (n>11 /*&& smth_energies[n]>smth_energies[n-12]*0.35*/ )///*
                smth_energies[n] -= smth_energies[n-12]*maskAmnt;
        }
    }

}
//--------------------------------------------------------------
void ofxFilterbank::exit(){
    
    for (int i=0; i<channels; i++) {
        for (int n=midiMin; n<midiMax; n++){
            if (fdata[i][n]!=NULL ) free (fdata[i][n]);
        }
        free (fdata[i]);
    }
    free (fdata);
    
}
//--------------------------------------------------------------
void ofxFilterbank::draw(int w, int h){

    ofSetColor(255);
    ofDrawBitmapString("FB Analysis", 4, 18);
    ofRect(0, 0, w, h);
    float step = (float) w / (midiMaxVar - midiMinVar);
    ofSetLineWidth(3);
    for(int n=midiMinVar; n<midiMaxVar; n++){
        log_smth_energies[n] = LIN2dB (smth_energies[n]);
      
       float ener = ofMap (log_smth_energies[n], -45.0, LIN2dB(estimateMax), 0.0, h, true);
        if(smth_energies[n] > treshold){
            ofSetColor(color);
            ofDrawBitmapString(midiToNote(n), step*(n-midiMinVar), h-ener);
            ofLine(step*(n-midiMinVar), h, step*(n-midiMinVar), h-ener );
        }
        else if (showAll){
            ofSetColor(85);
            ofLine(step*(n-midiMinVar), h, step*(n-midiMinVar), h-ener );
        }
    }

    ofSetColor(80);
    float logTresh = LIN2dB(treshold);
    float tres = ofMap(logTresh, -45.0, LIN2dB(estimateMax), 0.0, h);
    ofSetLineWidth(1);
    ofLine(0,h-tres, w, h-tres);
    ofDrawBitmapString("Treshold" , w-80, h-tres);



}
//--------------------------------------------------------------
string ofxFilterbank::midiToNote(int midi){
    string noteName;
    int mod = midi%12;
    switch (mod){
        case 0: noteName = "C";
                break;
        case 1: noteName = "C#";
                break;
        case 2: noteName = "D";
                break;
        case 3: noteName = "D#";
                break;
        case 4: noteName = "E";
                break;
        case 5: noteName = "F";
                break;
        case 6: noteName = "F#";
                break;
        case 7: noteName = "G";
                break;
        case 8: noteName = "G#";
                break;
        case 9: noteName = "A";
                break;
        case 10: noteName = "Bb";
                break;
        case 11: noteName = "B";
                break;
        default:
                break;

    }
    return (noteName);

}

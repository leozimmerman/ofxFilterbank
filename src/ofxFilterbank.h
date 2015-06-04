/*
 * Copyright (c) 2015 Leo Zimmerman <leozimmerman@gmail.com>
*/

#pragma once


#include "ofMain.h"
#define LIN2dB(x) (double)(20. * log10(x))

extern "C" {
    #include "fb_funcs.h"
}

class ofxFilterbank
{
    public:


        void setup(int iBufferSize, int iMidiMin, int iMidiMax, int iChans, float iBw, int iSR, float iGain);
        void draw(int w, int h);
        void exit();

        void setPitchDev();
        void analyze(float * iBuffer);
        string midiToNote (int midi);
        void setColor(ofColor col){color=col;}
    
        float* getEnergies(){return &energies[0];}
        float* getSmthEnergies(){return &smth_energies[0];}
        float* getLeftBuffer(){return &left[0];}
        float* getRightBuffer(){return &right[0];}
        float getPitchDev(){return pitchDev;}
    
		bool showAll;
        float smoothAmnt, treshold, estimateMax, maskAmnt, pitchDev, pitchFader;
        int midiMin, midiMax, midiMinVar, midiMaxVar;
    
    

    protected:

    private:
        RESONDATA ***fdata;
        vector<float> energies;
		vector<float> smth_energies;
		vector<float> log_smth_energies;
        ofColor color;
    
        vector <float> right;
        vector <float> left;
   
        int	sampleRate;
        int bufferSize;
        int channels;
        
        float bw;
        float gain;




};


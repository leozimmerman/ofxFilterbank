#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFilterbank.h"

#define BANDWITH  1.0
#define BUFFER_SIZE 1024
#define LIN2dB(x) (double)(20. * log10(x))
#define SR 44100


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void audioIn(float * input, int bufferSize, int nChannels);
        void audioOut(float * input, int bufferSize, int nChannels);


		ofSoundStream soundStream;
		ofxFilterbank filterBank;

		int	sampleRate;
		int bufferSize;
		int inChan;
		int outChan;    



};

#endif


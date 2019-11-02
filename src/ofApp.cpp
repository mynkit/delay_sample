/**
* @file ofApp.cpp
* @brief 処理のコア部分
* @author mynkit
* @date 2019/10
*/

#include "ofApp.h"
#include <iostream>
using namespace std;

const int BUFFERSIZE = 512; //! バッファサイズ(256推奨．大きくすると処理に余裕はでるが遅延が長くなる)
const int SAMPLERATE = 44100; //! サンプルレート(Hz)

const int MAXDELAYTIME = 1000; //! delayの間隔の最大値(これよりDELAYTIMEが上回るとエラー)．

//--------------------------------------------------------------
void ofApp::setup(){
    cout << "バッファによるレイテンシー: " << setprecision(2) << ((float) BUFFERSIZE) * 1000 / SAMPLERATE << "ms" << endl;
    
    bufferSize = BUFFERSIZE;
    sampleRate = SAMPLERATE;
    frequency = 440;
    inputBuffer.resize(bufferSize);

    myHoldIn = new holdIn(MAXDELAYTIME, sampleRate);
    myDelayOut = new delayOut(myHoldIn);
    delayOn = false;
    
    ofSoundStreamSetup(2, 1, sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}
//--------------------------------------------------------------
void ofApp::audioIn(float* buffer, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize; i++){
        inputBuffer[i] = buffer[i];
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(float* buffer, int bufferSize, int nChannels){

    float phaseDiff;

    phaseDiff = TWO_PI * frequency/sampleRate;
    
    for(int i = 0; i < bufferSize; i++){

		phase += phaseDiff;
		
		while (phase > TWO_PI) {
			phase -= TWO_PI;
		}
        
        float currentSample = inputBuffer[i];
        if (ringModulatorOn == true){
            //currentSample = 0.01 * sin(phase);
            currentSample = currentSample * sin(phase);
        }
        if (delayOn == true){
            currentSample = myDelayOut->effect(myHoldIn, currentSample);
        }
        buffer[i*nChannels+0] = currentSample;
        buffer[i*nChannels+1] = currentSample;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r'){
        ringModulatorOn = true;
    } else if (key == 'd'){
        delayOn = true;
    } else if (key == 'z'){
        delayOn = false;
        ringModulatorOn = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (delayOn == true){
        int new_delay_time = x / 10 * 10;
        float new_decay_rate = y / 1000.0;
        if(new_delay_time >= MAXDELAYTIME){new_delay_time = MAXDELAYTIME - 1;}
        if(new_decay_rate >= 0.7){new_decay_rate = 0.7;}
        myDelayOut->changeDelayTime(new_delay_time);
        myDelayOut->changeDecayRate(new_decay_rate);
    }
    if (ringModulatorOn == true){
        frequency = x / 10.0;
        if(frequency < 1){frequency = 1;}
    }
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
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
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

//--------------------------------------------------------------
void ofApp::exit(){
    ofSoundStreamClose();
}


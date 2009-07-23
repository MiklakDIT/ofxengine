/***************************************************************
* File: ofxEngineTweaks.h
* Author: nickiannone (aka ShotgunNinja)
* Status: Unknown (Probably complete)
* Notes:
*	This is where little snippets of code get tossed.
***************************************************************/
#pragma once

#include "ofxEngine.h"

//====================================================
// NOTE: To be added to "libs/openframeworks/util/ofUtils.h":
//====================================================
#include <stack>
using namespace std;

// A little hack here, to give push/pop functionality to ofSetLogLevel().
stack<int,vector<int>> logLevelStack;

int ofGetLogLevel() {
	return currentLogLevel;
}
void ofPushLogLevel(int logLevel) {
	logLevelStack.push( ofGetLogLevel() );
	ofSetLogLevel(logLevel);
}
void ofPopLogLevel() {
	// If there's nothing on the stack, just return. We have to leave currentLogLevel so ofLog() won't get messed up.
	if (!logLevelStack.empty()) return;
	int logLevel = logLevelStack.top();
	logLevelStack.pop();
	ofSetLogLevel(logLevel);
}
//====================================================
//====================================================



//====================================================
// NOTE: To be added to "libs/openframeworks/event/ofEventUtils.h":
//====================================================
template <class EventType>
static void ofClearListeners(EventType & event){
	event.clear();
}
template <class EventType>
static void ofEnableEvent(EventType & event){
	event.enable();
}
template <class EventType>
static void ofDisableEvent(EventType & event){
	event.disable();
}
template <class EventType>
static bool ofEventIsEnabled(EventType & event){
	return event.isEnabled();
}
// Some alternate names:
#define ofRemoveAllListeners(e)	ofClearListeners(e)
#define ofEventEnable(e)		ofEnableEvent(e)
#define ofEventDisable(e)		ofDisableEvent(e)
#define ofIsEventEnabled(e)		ofEventIsEnabled(e)
//===================================================
//====================================================

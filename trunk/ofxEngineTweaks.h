
#pragma once

#include "ofxEngine.h"

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

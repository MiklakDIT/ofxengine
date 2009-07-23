/***************************************************************
* File: Managers/ofxThinker.h
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#pragma once

#include "ofxThread.h"
//#include "../Abstracts/ofxThinkable.h" - REQUIRED
#include "ofxEngine.h"

//========================
// ofxThoughtKey class
//========================
class ofxThoughtKey {
public:
	friend class ofxThinkable;
	
	~ofxThoughtKey();
	
	// The event listener method.
	inline void checkThink(double timer);
	inline double getThinkTime() const;
	
protected:
	ofxThoughtKey(ofxThinkable* thinkable, ofxThinker* thinker, double nextThink);

	ofxThinkable* m_thinkable;
	ofxThinker* m_thinker;
	double m_nextThink;
};

//============================
// ofxThinker class
//============================
class ofxThinker : public ofxThread {
public:
	ofxThinker(unsigned int precision = 10);
	~ofxThinker();
	
	inline bool start();
	inline bool stop();
	inline bool isRunning() const;
	
	bool remove(ofxThoughtKey* key);
	bool clear();
	bool insert(ofxThoughtKey* key);
	bool enable();
	bool disable();
	inline bool isEnabled() const;
	bool setPrecision(unsigned int millisPerTick);
	inline unsigned int getPrecision() const;
	
protected:
	void threadedFunction();
	
	ofEvent<ofxSynapse*> m_event;
	unsigned int m_precision;
};
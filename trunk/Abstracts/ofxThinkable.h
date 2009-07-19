#pragma once

#include "ofxEngine.h"

//**********************************************************************
// NOTE: By default, negative nextThink values passed to setNextThink() will be forgotten.
// NOTE: Uncomment the next line if you want negative nextThink values to cancel all previous thoughts instead.
//#define OFX_THINKABLE_CANCEL_IF_NEGATIVE
//**********************************************************************

//====================================================
// NOTE: To be added to "libs/openframeworks/ofEventUtils.h":
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

// A Thinkable class. Thinkables are similar to the concept of tickables, but
// with a twist. For those unfamiliar with tickables, they are basically time-
// triggers. You can schedule a tickable to fire after such-and-such amount of
// time, and it will hook into a timer, which will call the tick() method of
// the tickable object when the timer reaches the time. Thinkables do that, and
// more. Where a tickable simply lets you overload the tick() method, a thinkable
// lets you call a setThink() method with a function pointer as an argument,
// and the next time the think() method is called, it will in turn call the 
// function you have set as the think function.
//========================

//========================
// ofxThinkable class
//========================
class ofxThinkable {
public:
	typedef (void(ofxThinkable::*ThinkFn)(void));

	ofxThinkable(ofxThinker* thinker = NULL, ThinkFn fn = NULL);
	virtual ~ofxThinkable();
	
	void setup(ofxThinker* thinker, ThinkFn fn, double nextThink = 0.0d);
	void setThinker(ofxThinker* thinker);
	void think();
	void setThink(ThinkFn fn);
	void setNextThink(double seconds);
	void cancelThink();
protected:
	ofxThinker* m_thinker;
	ThinkFn m_thinkfn;
	list<ofxThoughtKey*> m_thoughts;
};

//========================
// ofxThoughtKey class
//========================
class ofxThoughtKey {
public:
	ofxThoughtKey(ofxThinkable* thinkable, ofxThinker* thinker, double nextThink);
	~ofxThoughtKey();
	
	// The event listener method.
	inline void checkThink(double timer);
	inline double getThinkTime() const;
	
protected:
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

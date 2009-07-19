/***************************************************************
* File: Abstracts/ofxThinkable.h
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#pragma once

#include "ofxEngine.h"

//**********************************************************************
// NOTE: By default, negative nextThink values passed to setNextThink() will be forgotten.
// NOTE: Uncomment the next line if you want negative nextThink values to CANCEL all previous thoughts instead.
//#define OFX_THINKABLE_CANCEL_IF_NEGATIVE
//**********************************************************************

class ofxThinker;
class ofxThoughtKey;

//========================
// ofxThinkable class
//========================
class ofxThinkable {
public:
	friend class ofxThoughtKey;

	typedef (void(ofxThinkable::*ThinkFn)(void));

	ofxThinkable(ofxThinker* thinker = NULL, ThinkFn fn = NULL);
	virtual ~ofxThinkable();
	
	void setup(ofxThinker* thinker, ThinkFn fn, double nextThink = 0.0d);
	void setThinker(ofxThinker* thinker);
	void think();
	void setThink(ThinkFn fn);
	void setNextThink(double seconds);
	void cancelThink();
	
	inline ofxThinker* getThinker() const { return m_thinker; }
	
protected:
	ofxThinker* m_thinker;
	ThinkFn m_thinkfn;
	list<ofxThoughtKey*> m_thoughts;
};

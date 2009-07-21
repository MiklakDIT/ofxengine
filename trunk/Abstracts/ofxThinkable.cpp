/***************************************************************
* File: Abstracts/ofxThinkable.cpp
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#include "ofxThinkable.h"

//======================================================================
// class ofxThinkable
//======================================================================
ofxThinkable::ofxThinkable(ofxThinker* thinker = NULL, ofxThinkable::ThinkFn fn = NULL)
:m_thinkfn(fn), m_thinker(NULL)
{
	if (thinker != NULL)
		setThinker(thinker);
}

//----------------------------------------------------------------------
virtual ofxThinkable::~ofxThinkable()
{
	cancelThink();
}

//----------------------------------------------------------------------
void ofxThinkable::setup(ofxThinker* thinker, ofxThinkable::ThinkFn fn, double nextThink = 0.0d)
{
	setThinker(thinker);
	setThink(fn);
	if (nextThink >= 0)
		setNextThink(nextThink);
#ifdef OFX_THINKABLE_CANCEL_IF_NEGATIVE
	else
		cancelThink();
#endif
}

//----------------------------------------------------------------------
void ofxThinkable::setThinker(ofxThinker* thinker)
{
	cancelThink();
	m_thinker = thinker;
}

//----------------------------------------------------------------------
void ofxThinkable::think()
{
	if (this->m_thinkfn != NULL)
		(this->m_thinkfn)();
}

//----------------------------------------------------------------------
void ofxThinkable::setThink(ofxThinkable::ThinkFn fn)
{
	m_thinkfn = fn;
}

//----------------------------------------------------------------------
void ofxThinkable::setNextThink(double seconds)
{
	if (seconds < 0) {
#ifdef OFX_THINKABLE_CANCEL_IF_NEGATIVE
		cancelThink();
#endif
		return;
	}
	// Add the new thought to our stack.
	m_thoughts.push_back(new ofxThoughtKey(this,m_thinker,seconds));
}

//----------------------------------------------------------------------
void ofxThinkable::cancelThink()
{
	while (true)
	{
		ofxThoughtKey* key = m_thoughts.back();
		if (key != NULL)
			delete key;
		// NOTE: This is redundant, because the destructor of key _should_ do this for us.
		m_thoughts.remove(key);
		if (m_thoughts.empty())
			break;
	}
}

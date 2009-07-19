/***************************************************************
* File: Managers/ofxThinker.cpp
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#include "ofxThinker.h"

//======================================================================
// class ofxThoughtKey
//======================================================================
ofxThoughtKey::ofxThoughtKey(ofxThinkable* thinkable, ofxThinker* thinker, double nextThink)
:m_thinkable(thinkable), m_thinker(thinker), m_nextThink(nextThink)
{
#ifdef OFX_THINKABLE_CANCEL_IF_NEGATIVE
/**/	if (nextThink >= 0)
/**/	{
#endif
		// HACKHACK: Loop until we can lock the Thinker thread and insert the thought.
		while (true) {	
			if (m_thinker->insert(this))
				break;
		}
#ifdef OFX_THINKABLE_CANCEL_IF_NEGATIVE
/**/	}
/**/	else
/**/	{
/**/		m_thinkable->cancelThink();
/**/		delete this;	// Is this allowed in C++? I'm not sure...
/**/	}
#endif
}

//----------------------------------------------------------------------
ofxThoughtKey::~ofxThoughtKey()
{
	// Remove us from the Thinkable's internal thoughts list, just in case.
	if (m_thinkable != NULL)
		(m_thinkable->m_thoughts).remove(this);
	// HACKHACK: Loop until we can lock the Thinker thread and remove the thought.
	while (true) {
		if (m_thinker->remove(this))
			break;
	}
}

//----------------------------------------------------------------------
inline void ofxThoughtKey::checkThink(double timer)
{
	if (timer >= m_nextThink)
	{	
		m_thinkable->think();
		delete this;	// Is this allowed in C++?
	}
}

//----------------------------------------------------------------------
inline double ofxThoughtKey::getThinkTime() const
{
	return m_nextThink;
}






//======================================================================
// class ofxThinker
//======================================================================
ofxThinker::ofxThinker(unsigned int precision = 10)
:m_precision(precision)
{
	start();
}

//----------------------------------------------------------------------
ofxThinker::~ofxThinker()
{
	clear();
	stop();
}

//----------------------------------------------------------------------
inline bool ofxThinker::start()
{
	return startThread(true, false);
}

//----------------------------------------------------------------------
inline bool ofxThinker::stop()
{
	return stopThread();
}

//----------------------------------------------------------------------
inline bool ofxThinker::isRunning() const
{
	return isThreadRunning();
}

//----------------------------------------------------------------------
bool ofxThinker::remove(ofxThoughtKey* key)
{
	if (!isRunning()) {
		ofRemoveListener(&m_event, key, &ofxThoughtKey::checkThink);
		return true;
	} else {
		while (isRunning()) {
			if (lock()) {
				ofRemoveListener(&m_event, key, &ofxThoughtKey::checkThink);
				unlock();
				return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
bool ofxThinker::clear()
{
	if (!isRunning()) {
		ofClearListeners(&m_event);
		return true;
	} else {
		while (isRunning()) {
			if (lock()) {
				ofClearListeners(&m_event);
				unlock();
				return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
bool ofxThinker::insert()
{
	if (!isRunning()) {
		ofAddListener(&m_event, key, &ofxThoughtKey::checkThink);
		return true;
	} else {
		while (isRunning()) {
			if (lock()) {
				ofAddListener(&m_event, key, &ofxThoughtKey::checkThink);
				unlock();
				return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
bool ofxThinker::enable()
{
	if (!isRunning()) {
		ofEnableEvent(&m_event);
		return true;
	} else {
		while (isRunning()) {
			if (lock()) {
				ofEnableEvent(&m_event);
				unlock();
				return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
bool ofxThinker::disable()
{
	if (!isRunning()) {
		ofDisableEvent(&m_event);
		return true;
	} else {
		while (isRunning()) {
			if (lock()) {
				ofDisableEvent(&m_event);
				unlock();
				return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
inline bool ofxThinker::isEnabled() const
{
	return (ofEventIsEnabled(&m_event));
}

//----------------------------------------------------------------------
bool ofxThinker::setPrecision(unsigned int millisPerTick)
{
	if (!isRunning()) {
		m_precision = millisPerTick;
		return true;
	} else {
		while (isRunning()) {
			if (lock()) {
				m_precision = millisPerTick;
				unlock();
				return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
inline unsigned int ofxThinker::getPrecision() const
{
	return m_precision;
}

//----------------------------------------------------------------------
void ofxThinker::threadedFunction()
{
	unsigned int precision;
	while (isRunning()) {
		if (lock()) {
			ofNotifyEvent(&m_event, ofGetElapsedTimef());
			precision = m_precision;
			unlock();
		}
		ofSleepMillis(precision);
	}
}

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
	m_thoughts->push_back(new ofxThoughtKey(this,m_thinker,seconds));
}

//----------------------------------------------------------------------
void ofxThinkable::cancelThink()
{
	while (true)
	{
		ofxThoughtKey* key = m_thoughts.back();
		if (key != NULL)
			delete key;
		if (find(m_thoughts.begin(), m_thoughts.end(), key) != m_thoughts.end())
			remove(m_thoughts.begin(), m_thoughts.end(), key);
		if (m_thoughts.empty())
			break;
	}
}

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
		// HACKHACK: Loop until we can lock the thread and insert the thought.
		while (true) {	
			if (m_thinker->insert(this))
				break;
		}
#ifdef OFX_THINKABLE_CANCEL_IF_NEGATIVE
/**/	}
/**/	else
/**/	{
/**/		m_thinkable->cancelThink();
/**/		delete this;
/**/	}
#endif
}

//----------------------------------------------------------------------
ofxThoughtKey::~ofxThoughtKey()
{
	if ((m_thinkable) && (*m_thinkable)) {
		list<ofxThoughtKey*>* thoughts = &(m_thinkable->m_thoughts);
		remove(thoughts->begin(), thoughts->end(), this);
	}
	// HACKHACK: Loop until we can lock the thread and remove the thought.
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
		delete this;
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


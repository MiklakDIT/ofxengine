/***************************************************************
* File: Abstracts/ofxCacheable.cpp
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#include "ofxCacheable.h"

//==============================================================
// class ofxCacheable
//==============================================================
ofxCacheable::ofxCacheable(ofxCache* cache)
:m_cache(cache), m_cacheIDs(list<ofxCacheID>())
{
}

//--------------------------------------------------------------
ofxCacheable::~ofxCacheable()
{
	releaseCache();
}

//--------------------------------------------------------------
void ofxCacheable::precacheFile(string filename, ofxCacheLoader* customLoader = NULL)
{
	ofxCacheID id = 0;
	if (m_cache->isFileCached(filename)) {
		id = m_cache->getIDByFilename(filename);
	} else {
		id = m_cache->loadFile(filename,customLoader);
	}
	if (id != 0) {
		m_cacheIDs.push_back(id);
	} else {
		ofLog(OF_LOG_WARNING, "Couldn't precache file \"%s\"!", filename.c_str());
	}
}

//--------------------------------------------------------------
void ofxCacheable::releaseCache()
{
	list<ofxCacheID>::iterator i;
	for (i = m_cacheIDs.begin(); i != m_cacheIDs.end(); i++) {
		m_cache->unhook(this,(*i));
	}
	m_cacheIDs.clear();
}

//--------------------------------------------------------------
ofxCacheData* ofxCacheable::getCacheData(string filename)
{
	ofCacheID id = m_cache->getIDByName(filename);
	if (m_cacheIDs.find(id) != m_cacheIDs.end()) {
		return m_cache->getCacheDataByID(id);
	}
	return NULL;
}

//--------------------------------------------------------------
ofxCacheData* ofxCacheable::getCacheData(ofxCacheID id)
{
	if (m_cacheIDs.find(id) != m_cacheIDs.end()) {
		return m_cache->getCacheDataByID(id);
	}
	return NULL;
}

//--------------------------------------------------------------
int ofxCacheable::getCachedDataCount() const
{
	return (m_cacheIDs.count());
}

//--------------------------------------------------------------
bool ofxCacheable::isCacheReleased() const
{
	return (m_cacheIDs.empty());
}

//--------------------------------------------------------------
void ofxCacheable::setCache(ofxCache* cache)
{
	releaseCache();
	m_cache = cache;
}


/***************************************************************
* File: Abstracts/ofxCacheable.h
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#pragma once

#include "ofxEngine.h"

typedef unsigned int ofxCacheID;

class ofxCacheLoader;
class ofxCacheData;
class ofxCache;

//==========================================================
// class ofxCacheable
//==========================================================
class ofxCacheable {
public:
	friend class ofxCache;

	ofxCacheable(ofxCache* cache);
	virtual ~ofxCacheable();
	
	virtual void precache() = 0;	// Overload for each derived class to precache stuff!

	// A utility to cache a file. Cached files are hooked to the Cache using this Cacheable, or withdrawn from the Cache if they are already loaded.
	void precacheFile(string filename, ofxCacheLoader* customLoader = NULL);
	
	// Unhooks the cacheable from the cached data.
	void releaseCache();
	
	// A utility to retrieve the cache data by its filename. 
	ofxCacheData* getCacheData(string filename);
	
	// A utility to retrieve the cache data by its ID number.
	ofxCacheData* getCacheData(ofxCacheID id);
	
	// Gets the number of cached files that the Cacheable depends upon.
	int getCachedDataCount() const;
	// Checks to see if this cacheable is using any cached files.
	bool isCacheReleased() const;
	
protected:
	// Sets the used Cache (probably not important, as we're probably only gonna use 1 Cache for everything, but still...)
	void setCache(ofxCache* cache);

	ofxCache* m_cache;
	list<ofxCacheID>* m_cacheIDs;
};

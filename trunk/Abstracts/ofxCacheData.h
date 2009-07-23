/***************************************************************
* File: Abstracts/ofxCacheData.h
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#pragma once

#include "ofxEngine.h"

class ofxCacheable;
class ofxCacheLoader;
class ofxCache;

//==============================================================
// class ofxCacheData
//==============================================================
class ofxCacheData {
public:
	ofxCacheData(string filename, ofxCacheLoader* loader);
	~ofxCacheData();

	// Reaches into the loader which loaded the file to find its data type (which is user-specified in the loader)
	string getDataType() const;
	
	// Gets the loader which loaded the file. (Note: Loaders should remain in memory until all cache data from that loader is cleared, or memory leaks will occur!)
	ofxCacheLoader* getLoader() const;
	
	// Gets the Cache in which this resides.
	ofxCache* getCache() const;
	// Gets the ID number of this Cache Data.
	ofxCacheID getID() const;
	// Gets the originally-loaded filename.
	string getFileName() const;
	// Gets the (void*) userdata which represents the loaded file (an instance of ie. ofImage, ofSoundPlayer, ofVideoPlayer, etc.)
	void* getUserData() const;
	
protected:
	string m_filename;
	ofxCacheLoader* m_loader;
	ofxCacheID m_id;
	void* m_userdata;
};

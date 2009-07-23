/***************************************************************
* File: Abstracts/ofxCacheData.cpp
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#include "ofxCacheData.h"

//==============================================================
// class ofxCacheData
//==============================================================
ofxCacheData::ofxCacheData(string filename, ofxCacheLoader* loader)
:m_filename(filename), m_loader(loader)
{
	m_id = m_cache->takeID();
	m_userdata = m_loader->loadFile(filename);
}

//--------------------------------------------------------------
ofxCacheData::~ofxCacheData()
{
	m_loader->unloadFile(m_userdata);
}

//--------------------------------------------------------------
string ofxCacheData::getDataType() const
{
	return (m_loader->getDataType());
}

//--------------------------------------------------------------
ofxCacheLoader* ofxCacheData::getLoader() const
{
	return m_loader;
}

//--------------------------------------------------------------
ofxCache* ofxCacheData::getCache() const
{
	return (m_loader->getCache());
}

//--------------------------------------------------------------
ofxCacheID ofxCacheData::getID() const
{
	return m_id;
}

//--------------------------------------------------------------
string ofxCacheData::getFileName() const
{
	return m_filename;
}

//--------------------------------------------------------------
void* ofxCacheData::getUserData() const
{
	return m_userdata;
}

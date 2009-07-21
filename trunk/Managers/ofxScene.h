/***************************************************************
* File: Managers/ofxScene.h
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#pragma once

//#include "../Abstracts/ofxSceneNode.h"
#include "ofxEngine.h"

class ofxBaseSceneNode;
class ofxSceneRootNode;
class ofxSceneNode;

//===================================================================
// class ofxScene
//===================================================================
class ofxScene {
public:
	friend class ofxSceneNode;

	typedef map<ofxBaseSceneNode*,ofxBaseSceneNode*> ParentMap;
	typedef ParentMap::value_type ParentPair;
	
	ofxScene();
	~ofxScene();
	
	void load();
	bool isLoaded();
	void unload();
	void draw();
	
	int getNodeCount();
	
	static ofxBaseSceneNode* getParentOf(ofxBaseSceneNode* child);
	static list<ofxBaseSceneNode*>* getChildrenOf(ofxBaseSceneNode* parent);
	
	inline ofxBaseSceneNode* getRootNode();
	ofxBaseSceneNode* getNodeByID(ofxSceneID id);
	
protected:
	void addChild(ofxBaseSceneNode* child, ofxBaseSceneNode* parent);

	ofxSceneID m_nextID;
	ofxSceneRootNode* m_rootNode;
};

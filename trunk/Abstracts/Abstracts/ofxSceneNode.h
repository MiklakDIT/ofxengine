/***************************************************************
* File: Abstracts/ofxSceneNode.h
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#pragma once

#include "ofxEngine.h"

class ofxScene;

//===================================================================
// typedef ofxSceneID
//===================================================================
typedef unsigned int ofxSceneID;

//===================================================================
// class ofxBaseSceneNode (Abstract)
//===================================================================
class ofxBaseSceneNode {
public:
	ofxBaseSceneNode();
	~ofxBaseSceneNode();
	
	virtual bool isRoot() = 0;
	virtual bool isSpawned() = 0;
	
	void draw();
	inline ofxSceneID getID() const;
	inline ofxScene* getScene() const;
	// Counts all immediate children.
	int getChildCount() const;
	// Counts all children recursively.
	int getTotalCount() const;
	
protected:
	virtual void spawn(ofxScene* scene, ofxBaseSceneNode* parent = NULL) = 0;
	virtual void kill() = 0;
	virtual void beginDraw() = 0;
	virtual void endDraw() = 0;
	
	inline ofxBaseSceneNode* getParent() const;
	inline list<ofxBaseSceneNode*>* getChildren() const;
	ofxBaseSceneNode* getNodeByID(ofxSceneID id);
	
	ofxScene* m_scene;
	ofxSceneID m_id;
	ofxBaseSceneNode* m_parent;
	list<ofxBaseSceneNode*>* m_children;
};

//===================================================================
// class ofxSceneRootNode
//===================================================================
class ofxSceneRootNode : public ofxBaseSceneNode {
public:
	ofxSceneRootNode(ofxScene* scene);
	~ofxSceneRootNode();
	
	bool isRoot();
	bool isSpawned();
	
protected:
	void spawn(ofxScene* scene, ofxBaseSceneNode* parent = NULL);
	void kill();
	void beginDraw();
	void endDraw();
};

//===================================================================
// class ofxSceneNode (Abstract)
//===================================================================
class ofxSceneNode : public ofxBaseSceneNode {
public:
	friend class ofxScene;

	ofxSceneNode(ofxScene* scene, ofxBaseSceneNode* parent = NULL);
	~ofxSceneNode();
	
	bool isRoot();
	bool isSpawned();

protected:
	virtual void onSpawn() = 0;
	virtual void onKill() = 0;
	virtual void beginDraw() = 0;
	virtual void endDraw() = 0;
	
	void spawn(ofxScene* scene, ofxBaseSceneNode* parent = NULL);
	// A little housekeeping function.
	void dumpChildren();
	void kill();
};

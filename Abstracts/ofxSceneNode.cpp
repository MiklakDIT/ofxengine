/***************************************************************
* File: Abstracts/ofxSceneNode.cpp
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#include "ofxSceneNode.h"

//==================================
// class ofxBaseSceneNode
//==================================
ofxBaseSceneNode::ofxBaseSceneNode()
:m_id(0), m_scene(NULL), m_children(new list<ofxBaseSceneNode*>()), m_parent(NULL)
{}

//-----------------------------------------------------------
ofxBaseSceneNode::~ofxBaseSceneNode()
{
	kill();
}

//-----------------------------------------------------------
ofxBaseSceneNode::draw()
{
	list<ofxBaseSceneNode*>::iterator i;
	this->beginDraw();
	for (i = m_children->begin(); i != m_children->end(); i++)
		(*i)->draw();
	this->endDraw();
}

//-----------------------------------------------------------
inline ofxSceneID ofxBaseSceneNode::getID() const
{
	return m_id;
}

//-----------------------------------------------------------
inline ofxScene* ofxBaseSceneNode::getScene() const
{
	return m_scene;
}

//-----------------------------------------------------------
int ofxBaseSceneNode::getChildCount() const
{
	return (m_children->count());
}

//-----------------------------------------------------------
int ofxBaseSceneNode::getTotalCount() const
{
	int count = 0;
	list<ofxBaseSceneNode*>::iterator i;
	for (i = m_children->begin(); i != m_children->end(); i++)
		count += (*i)->getTotalCount() + 1;
	return count;
}

//-----------------------------------------------------------
inline ofxBaseSceneNode* ofxBaseSceneNode::getParent() const
{
	return m_parent;
}

//-----------------------------------------------------------
inline list<ofxBaseSceneNode*>* ofxBaseSceneNode::getChildren() const
{
	return m_children;
}

//-----------------------------------------------------------
ofxBaseSceneNode* ofxBaseSceneNode::getNodeByID(ofxSceneID id)
{
	if (id == 0)
		return m_scene->getRootNode();
	if (id >= (m_scene->m_nextID))
		return NULL;
	if (id == m_id)
		return this;
	ofxBaseSceneNode* next = NULL;
	for (i = m_children->begin(); i != m_children->end(); i++) {
		next = (*i)->getNodeByID(id);
		if (next != NULL)
			return next;
	}
	return NULL;
}


//==================================
// class ofxSceneRootNode
//==================================
ofxSceneRootNode::ofxSceneRootNode(ofxScene* scene)
{
	spawn(scene);
}

//-----------------------------------------------------------
ofxSceneRootNode::~ofxSceneRootNode()
{
	kill();
}

//-----------------------------------------------------------
bool ofxSceneRootNode::isRoot()
{
	return true;
}

//-----------------------------------------------------------
bool ofxSceneRootNode::isSpawned()
{
	return true;
}

//-----------------------------------------------------------
void ofxSceneRootNode::spawn()
{
	m_scene = scene;
	m_id = 0;
}

//-----------------------------------------------------------
void ofxSceneRootNode::kill()
{
	list<ofxBaseSceneNode*>::iterator i;
	for (i = m_children->begin(); i != m_children->end(); i++)
		delete (*i);
	m_children->clear();
	delete m_children;
	m_children = NULL;
}

//-----------------------------------------------------------
void ofxSceneRootNode::beginDraw()
{
	// Do global frame-begin stuff here!
}

//-----------------------------------------------------------
void ofxSceneRootNode::endDraw()
{
	// Do global frame-end stuff here!
}


//==================================
// class ofxSceneNode
//==================================
ofxSceneNode::ofxSceneNode(ofxScene* scene, ofxBaseSceneNode* parent = NULL)
{
	spawn(scene,parent);
}

//-----------------------------------------------------------
ofxSceneNode::~ofxSceneNode()
{
	kill();
}

//-----------------------------------------------------------
bool ofxSceneNode::isRoot()
{
	return false;
}

//-----------------------------------------------------------
bool ofxSceneNode::isSpawned()
{
	return (m_id != 0);
}

//-----------------------------------------------------------
void ofxSceneNode::spawn(ofxScene* scene, ofxBaseSceneNode* parent = NULL)
{
	if (!isSpawned()) {
		scene->addChild(this,parent);
		this->onSpawn();
	}
}

//-----------------------------------------------------------
void ofxSceneNode::dumpChildren()
{
	if (!isSpawned()) {
		list<ofxBaseSceneNode*>::iterator i;
		for (i = m_children->begin(); i != m_children->end(); i++)
			delete (*i);
		m_children->clear();
	}
}

//-----------------------------------------------------------
void ofxSceneNode::kill()
{
	if (isSpawned()) {
		this->onKill();
		m_id = 0;
		dumpChildren();
		delete m_children;
		m_children = NULL;
		m_scene = NULL;
	}
}

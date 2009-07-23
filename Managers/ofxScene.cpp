/***************************************************************
* File: Abstracts/ofxSceneNode.cpp
* Author: nickiannone (aka ShotgunNinja)
* Status: Complete
***************************************************************/
#include "ofxScene.h"

//==================================
// class ofxScene
//==================================
ofxScene::ofxScene()
:m_rootNode(NULL)
{
	load();
}

//-----------------------------------------------------------
ofxScene::~ofxScene()
{
	delete m_rootNode;
}

//-----------------------------------------------------------
void ofxScene::load()
{
	if (isLoaded())
		unload();
	m_nextID = 1;
	m_rootNode = new ofxSceneRootNode(this);
}

//-----------------------------------------------------------
bool ofxScene::isLoaded()
{
	return (m_rootNode != NULL);
}

//-----------------------------------------------------------
void ofxScene::unload()
{
	delete m_rootNode;
	m_rootNode = NULL;
}

//-----------------------------------------------------------
void ofxScene::draw()
{
	m_rootNode->draw();
}

//-----------------------------------------------------------
int ofxScene::getNodeCount()
{
	return m_rootNode->getTotalCount() + 1;
}

//-----------------------------------------------------------
static ofxBaseSceneNode* ofxScene::getParentOf(ofxBaseSceneNode* child)
{
	return (child->getParent());
}

//-----------------------------------------------------------
static list<ofxBaseSceneNode*>* ofxScene::getChildrenOf(ofxBaseSceneNode* parent)
{
	return (parent->getChildren());
}

//-----------------------------------------------------------
inline ofxBaseSceneNode* ofxScene::getRootNode()
{
	// return static_cast<ofxBaseSceneNode*>(m_rootNode);
	return (ofxBaseSceneNode*)m_rootNode;
}

//-----------------------------------------------------------
ofxBaseSceneNode* ofxScene::getNodeByID(ofxSceneID id)
{
	if (id == 0) return getRootNode();
	if (id >= m_nextID) return NULL;
	return (getRootNode())->getNodeByID(id);
}

//-----------------------------------------------------------
void ofxScene::addChild(ofxBaseSceneNode* child, ofxBaseSceneNode* parent)
{
	if ((parent == NULL) || (parent->getScene() != this))
		return;
	child->m_scene = this;
	child->m_id = m_nextID;
	m_nextID++;
	child->m_parent = parent;
	(getChildrenOf(parent))->push_back(child);
}

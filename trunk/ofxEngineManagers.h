/***************************************************************
* File: ofxEngineManagers.h
* Author: nickiannone (aka ShotgunNinja)
* Status: Incomplete
* Notes:
*	This file simply includes all of the header files from the "Managers/" folder.
*	Modify this when new files get added!
***************************************************************/
#pragma once

#include "ofxEngine.h"

// Managerial and Workhorse Classes (Level 2):

// Scene:		Operates upon Scene Nodes and derived classes; the Scene holds the Root Node and captures the draw() event from the App.
// Thinker:		Operates upon Thinkables and derived classes; the Thinker contains the thread which fires the think events to the Thinkables.
// Cache:
// Logic Web:	
// Animator:
// ????

#include "Managers/ofxScene.h"		// FINISHED
#include "Managers/ofxThinker.h"	// FINISHED


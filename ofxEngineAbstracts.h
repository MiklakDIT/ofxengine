#pragma once

#include "ofxEngine.h"

// Abstract Class Definitions (Level 1)

// Scene Node:	Allows an object to be incorporated into the Scene Graph, which handles the draw() function.
// Cacheable:	Allows an object to load and utilize Cache Data in the Cache.
// Cache Data:	A wrapper for classes which load, save, and utilize data from files (ofImage, ofxXmlSettings, ofxShader, etc.)
// Cloneable:	Allows an object to easily clone itself, and provides utilities for keeping track of clone donors and inheritors.
// Thinkable:	Allows an object to schedule a modifiable, time-triggered, self-handled event. Similar to "Tickables", but the method can be changed during runtime.
// Logical:		Allows an object to communicate with other Logicals by means of Logic objects fired between Logic Gates.
// Animated:	Allows an object to utilize sprite/animation sequences and listen to Animator events.
// Function:	Allows an object to be evaluated as a function, with some added features.
// Entity:		Derives from SceneNode, Cacheable, Cloneable, Thinkable, and Logical to make a basic entity class.

#include "Abstracts/ofxSceneNode.h"	// FINISHED
#include "Abstracts/ofxCacheable.h"	// FINISHED
#include "Abstracts/ofxCacheData.h"	// FINISHED
#include "Abstracts/ofxCloneable.h"	// FINISHED
#include "Abstracts/ofxThinkable.h"	// FINISHED
#include "Abstracts/ofxLogical.h"	// TODO
#include "Abstracts/ofxAnimated.h"	// TODO
#include "Abstracts/ofxFunction.h"	// FINISHED
#include "Abstracts/ofxEntity.h"	// TODO

#pragma once

#include "ofxEngine.h"

// A Cloneable class. Cloneables let you copy or "clone" an object through a
// virtual method, and lets you map the original and the clone to a clone map.
// The Clone Map is nothing but a std::map with a slight modification to its
// destructor method to help prevent memory leakage. It is mapped with the 
// Inheritor as the key and the Donor as the value, so that we avoid having to
// use a multimap for one-to-many relationships when we only need a one-to-one
// relationship between donors and inheritors. In other words, since each inheritor
// can only have one donor, we map donors to the value so that copies can safely
// exist in the container, and we still only have one pair per map with the key
// being the inheritor. (Read up on database normalization if you still don't get it.)

class ofxCloneable;

class ofxCloneMap : public map<ofxCloneable*,ofxCloneable*> {
public:
	friend class ofxCloneable;

	// We're keeping the same inner workings of the std::multimap, but adding a default destructor for custom functionality.
	~ofxCloneMap();
};
typedef ofxCloneMap::iterator ofxCloneIterator;
typedef ofxCloneMap::value_type ofxCloneKinship;
class ofxCloneable {
public:
	friend class ofxCloneMap;
	
	ofxCloneable() {}
	// NOTE: You MUST call the ofxCloneable destructor from ANY AND ALL derived destructor(s)!
	virtual ~ofxCloneable();
	
	void decease(ofxCloneMap* map);
	void deceaseAll();
	
	ofxCloneable* clone(ofxCloneMap* map);
	bool isWithin(ofxCloneMap* map) const;
	bool isRegisteredTo(ofxCloneMap* map) const;
	bool isDonorDeceased(ofxCloneMap* map) const;
	bool hasDonor(ofxCloneMap* map) const;
	ofxCloneable* getDonor(ofxCloneMap* map);
	ofxCloneIterator findDonor(ofxCloneMap* map);
	
	bool hasInheritor(ofxCloneMap* map) const;
	list<ofxCloneable*>* getInheritors(ofxCloneMap* map) const;
	ofxCloneIterator findNextInheritor(ofxCloneMap* map);
	ofxCloneIterator findNextInheritor(ofxCloneMap* map, ofxCloneIterator i);
	static ofxCloneable* getInheritorAt(ofxCloneIterator i);
	static ofxCloneable* getDonorAt(ofxCloneIterator i);
protected:
	// Our overloadable onClone() method. This is protected so it can't be called outside of the base-class' clone() method.
	virtual ofxCloneable* onClone() = 0;

	// An internal list of pointers to all clone maps that this clone is registered with.
	// NOTE: Deleting the clone map will _NOT_ delete all of its members!
	// NOTE: Deleting the clone map _WILL_ remove it from any contained ofxCloneable's internal list!
	list<ofxCloneMap*> m_cloneMaps;
};

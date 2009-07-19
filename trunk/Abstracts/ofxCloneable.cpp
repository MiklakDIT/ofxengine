
#include "ofxCloneable.h"

//===============
// ofxCloneMap
//===============
ofxCloneMap::~ofxCloneMap() {
	// Wow, it's SO weird to be writing an iterator loop from INSIDE an STL container for once...
	for (iterator i = begin(); i != end(); i++) {
		// Remove the map from the n-th donor's internal list:
		if ((i->second) != NULL) {
			list<ofxCloneMap*>* donorMaps = &((i->second)->m_cloneMaps);
			remove(donorMaps->begin(), donorMaps->end(), this);
		}
		// Remove the map from the n-th inheritor's internal list:
		if ((i->first) != NULL) {
			// Because the inheritor is a key, and keys are const, we have to cast off const-ness.
			// NOTE: Some compilers and platforms _MIGHT_ have issues with const_cast for modifications.
			ofxCloneable* inheritor = const_cast<ofxCloneable*>(i->first);
			list<ofxCloneMap*>* inheritorMaps = &(inheritor->m_cloneMaps);
			remove(inheritorMaps->begin(), inheritorMaps->end(), this);
		}
	}
	// At this point, all real ofxCloneables within this map shall no longer point to this map.
	// Therefore, all real ofxCloneables that used to point to this map _SHOULD_ no longer point to this map.
	// This means that there are no internally-made pointers to this map, and we can deallocate properly.
}

//==============
// ofxCloneable
//==============
virtual ofxCloneable::~ofxCloneable()
{
	deceaseAll();
}

//---------------------------------
void ofxCloneable::decease(ofxCloneMap* map)
{
	if (!isWithin(map)) return;	// We're already not in the map.
	ofxCloneIterator i;
	for (i = map->begin(); i != map->end(); i++) {
		if (getDonorAt(i) == this) {
			(i->second) = NULL;	// Sets the donor to NULL. We're dead now.
		}
	}
	for (i = map->begin(); i != map->end(); i++) {
		if (getInheritorAt(i) == this) {
			ofxCloneable* donor = getDonorAt(i);
			map->erase(i);		// Erases the entire clone record.
			bool anymore = false;
			for (ofxCloneIterator j = map->begin(); j != map->end(); j++) {
				if (getDonorAt(j) == donor) {
					anymore = true;
					break;
				}
			}
			if (!anymore)		// If that was the last inheritor for that donor, wipe the map from his internal list.
				remove((donor->m_cloneMaps).begin(); (donor->m_cloneMaps).end(); map);
			break;				// We can only be in this once!
		}
	}
}

//------------------------------------------
void ofxCloneable::deceaseAll()
{
	list<ofxCloneMap*>* maplist = new list<ofxCloneMap*>(m_cloneMaps);
	list<ofxCloneMap*>::iterator i;
	for (i = maplist->begin(); i != maplist->end(); i++) {
		decease((*i));
	}
	maplist->clear();
	delete maplist;
}

//-------------------------------------------
ofxCloneable* ofxCloneable::clone(ofxCloneMap* map)
{
	// Here, we call our virtualized onClone() method to create and fill the clone.
	ofxCloneable* inheritor = this->onClone();
	// Now, we add the newly created and filled clone (or "inheritor") and it's "donor" into the clone-map.
	map->insert(ofxCloneKinship(inheritor,this));
	// If we don't find the map in our donor's list, we add it.
	if (!isRegisteredTo(map)) {
		m_cloneMaps.push_back(map);
	}
	// Then, because we know our cloned object doesn't have anything in its clone-map registry:
	(inheritor->m_cloneMaps).push_back(map);
	// We keep lists within each cloneable so it can know which clone-maps it is a member of.
	return inheritor;
}

//---------------------------------------------
bool ofxCloneable::isWithin(ofxCloneMap* map) const
{
	// We search the clone map to see if we're in it.
	return (hasDonor(map) || hasInheritor(map));
}

//---------------------------------------------
bool ofxCloneable::isRegisteredTo(ofxCloneMap* map) const
{
	// We search our internal list for the clone-map.
	return (find(m_cloneMaps.begin(), m_cloneMaps.end(), map) != m_cloneMaps.end());
}

//---------------------------------------------
bool ofxCloneable::isDonorDeceased(ofxCloneMap* map) const {
	if (!hasDonor(map)) return false;	// If he's not _IN_ there, then he can't be dead, right?
	ofxCloneable* donor = getDonor(map);
	if ((donor == NULL)) return true;
	return false;
}

//---------------------------------------------
bool ofxCloneable::hasDonor(ofxCloneMap* map) const {
	if (map == NULL) return false;	// A NULL map can't contain our donor!
	if (findDonor(map) != map->end()) return true;	// If we can find ourselves registered as an inheritor, then we have to have a donor.
	return false;
}

//---------------------------------------------
ofxCloneable* ofxCloneable::getDonor(ofxCloneMap* map) const {
	ofxCloneIterator i = findDonor(map);
	if (i == map->end()) return NULL;
	return (getDonorAt(i));
}

//---------------------------------------------
ofxCloneIterator ofxCloneable::findDonor(ofxCloneMap* map) {
	ofxCloneIterator i;
	for (i = map->begin(); i != map->end(); i++) {
		if (getInheritorAt(i)) == this)
			return i;
	}
	return map->end();
}

//----------------------------------------------
bool ofxCloneable::hasInheritor(ofxCloneMap* map) const {
	if (map == NULL) return false;
	if (findNextInheritor(map) != map->end()) return true;
	return false;
}

//----------------------------------------------
list<ofxCloneable*>* ofxCloneable::getInheritors(ofxCloneMap* map) const {
	if (map == NULL) return NULL;
	if (!isWithin()) return NULL;
	list<ofxCloneable*>* inheritors = new list<ofxCloneable*>();
	ofxCloneIterator i;
	for (i = map->begin(); i != map->end(); i = findNextInheritor(map, i)) {
		inheritors->push_back(getInheritorAt(i));
	}
	return inheritors;
}

//----------------------------------------------
ofxCloneIterator ofxCloneable::findNextInheritor(ofxCloneMap* map) {
	return findNextInheritor(map, map->begin());
}

//----------------------------------------------
ofxCloneIterator ofxCloneable::findNextInheritor(ofxCloneMap* map, ofxCloneIterator i) {
	ofxCloneIterator j;
	for (j = i; j != map->end(); j++) {
		if ((getDonorAt(j)) == this)
			return j;
	}
	return map->end();
}

//----------------------------------------------
static ofxCloneable* ofxCloneable::getInheritorAt(ofxCloneIterator i) {
	return const_cast<ofxCloneable*>(i->first);	// We have to cast away const-ness because the key of a map is always const.
}

//----------------------------------------------
static ofxCloneable* ofxCloneable::getDonorAt(ofxCloneIterator i) {
	return (i->second);
}

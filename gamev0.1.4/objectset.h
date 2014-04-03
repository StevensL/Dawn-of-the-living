#ifndef OBJECTSET_H
#define OBJECTSET_H

#ifndef NULL
//#define NULL 0L
#endif
/**************************
	ObjectSet:  a set of objects to be handled

	This set exists so that world, and possibly other classes representing subsets of the world,
		can inherit from it.

	Its member function ProcessCollisions should test for collisions between all members of the set.

	The reason for doing this in ObjectSet:  we may in the future have one room or one area in
		which to test for collisions, such that there can be no collisions between its member objects
		and any non-member object.  So we're keeping our worlds modular.

	ObjectSet also tells its objects to Prepare, Draw, and Animate themselves.

	Will briggs 11-2007
**********************/

using namespace lcgl;

#include <vector>

class Object;
class Player;
class ObjectSet: public std::vector<Object*> 
{
protected:
	int CountObjectTypes (const type_info& classID); //how many "classID" objects are there?

	void Draw (const Player& p);
	void animate(float deltaTime) ;
	void ProcessCollisions ();
	void Prepare();
public:
	void ProcessCollisions(Object *obj) ;
	bool contains (const Object* what) const;
};

#endif
#include <vector>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "player.h"
#include "objectset.h"

using namespace std;

bool ObjectSet::contains (const Object* what) const
{
	 int which=0;
	for (;which < (signed)size(); ++which) //using loop w/ iterators didn't work; don't know why
		if ((*this)[which] == what)
			return true;

	return false;
}

int ObjectSet::CountObjectTypes (const type_info& classID)
{
	int count = 0;

	for (vector<Object*>::iterator i = begin(); i != end(); ++i)
	{
		//type_info x (typeid (*i));
		const char* name = typeid(**i).name();
		const char* n2   = classID.name();
		if (typeid(**i) 
			== classID)
			count++;
	}

	return count;
}

void ObjectSet::Draw (const Player& p)
{
	for (vector<Object*>::iterator i = begin(); i != end(); ++i)
		(*i)->Draw (p);
}


void ObjectSet::animate(float deltaTime) 
{
	for (vector<Object*>::iterator i = begin(); i != end(); ++i)
		(*i)->animate (deltaTime);
}

void ObjectSet::ProcessCollisions(Object *obj) 
{
	for (vector<Object*>::iterator i = begin(); i != end(); ++i)
		//and skip if *i == obj
		(*i)->ProcessCollisions (obj);
}

void ObjectSet::ProcessCollisions () 
{
	for (vector<Object*>::iterator i = begin(); i != end()-1; ++i)
		for (vector<Object*>::iterator j = i+1; j != end(); ++j) 
		{
		   (*i)->ProcessCollisions (*j);
		   (*j)->ProcessCollisions (*i);
		}

}

void ObjectSet::Prepare()
{
	for (int i = 0; i < (signed)size(); ++i) (*this)[i]->Prepare();
	//Why not use iterators?  Because Prepare might add something to the list, whic
	// trashes the iterator!  Easiest fix for now is to use int instead.
	/*
	for (vector<Object*>::iterator i = begin(); i != end(); ++i)
		(*i)->Prepare ();
		*/
}
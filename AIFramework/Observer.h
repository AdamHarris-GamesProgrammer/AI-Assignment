#pragma once
#include "Entity.h"
#include "Event.h"

class Observer
{
public:
	virtual ~Observer() {}
	virtual void OnNotify(const Entity& entity, Event event) = 0;
	virtual void OnNotify(Event event) = 0;


};

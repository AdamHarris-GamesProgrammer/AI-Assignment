#pragma once
#include "Event.h"

class Observer
{
public:
	virtual ~Observer() {}
	virtual void OnNotify(Event event) = 0;


};

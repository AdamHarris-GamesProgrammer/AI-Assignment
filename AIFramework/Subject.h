#pragma once
#include "Observer.h"
#include "Event.h"
#include <vector>
#include <algorithm>

class Subject {
public:
	void AddObserver(Observer* observer) {
		_observers.push_back(observer);
		_numOfObservers++;
	}

	void RemoveObserver(Observer* observer) {
		_observers.erase(std::remove(_observers.begin(), _observers.end(), observer));
		_numOfObservers--;
	}

protected:
	//Allows events to be sent for example (PICKUP_SPAWNED)
	void Notify(Event event) {
		for (int i = 0; i < _numOfObservers; i++)
		{
			_observers[i]->OnNotify(event);
		}
	}

private:
	int _numOfObservers;
	std::vector<Observer*> _observers;
};

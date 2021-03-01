#ifndef STATE_H
#define STATE_H

class Vehicle;

class State {
public:
	State(Vehicle* owner) {
		pOwner = owner;
	}

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void Update(float dt) = 0;

protected:
	Vehicle* pOwner = nullptr;

};

#endif

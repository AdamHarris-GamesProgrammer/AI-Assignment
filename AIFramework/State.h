#ifndef STATE_H
#define STATE_H

class Vehicle;

class State {
public:
	State(Vehicle* owner) {
		_pOwner = owner;
	}

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void Update(float dt) = 0;
	virtual void DrawUI() {}

protected:
	Vehicle* _pOwner = nullptr;

};

#endif

#ifndef STATE_H
#define STATE_H

class Vehicle;

//A state base class
class State {
public:
	//Constructor which sets the states owner
	State(Vehicle* owner) {
		_pOwner = owner;
	}

	//Called when we enter a state
	virtual void OnEnter() = 0;

	//Called when we exit a state
	virtual void OnExit() = 0;

	//Called every frame
	virtual void Update(float dt) = 0;

	//Draws the relevant UI for the state
	virtual void DrawUI() {}

protected:
	Vehicle* _pOwner = nullptr;

};

#endif

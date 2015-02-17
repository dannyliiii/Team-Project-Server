#pragma once

#include <stdlib.h>
#include "MyGame.h"
using namespace std;

enum States{
	destroyed,
	moveFoward,
	avoidObstcle,
	avoidOthers
};

class SpaceshipFSM{
public:
	SpaceshipFSM();
	~SpaceshipFSM();
	
	void updateState();
	void behaveState();
	inline States getCurrentState(){ return currentState; }

private:
	States currentState;
	void behaveMoveFoward();
	void behaveAvoidObstacle();
	void behaveAvoidOthers();
};
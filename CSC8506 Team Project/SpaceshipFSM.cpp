#include "SpaceshipFSM.h"

using namespace std;

SpaceshipFSM::SpaceshipFSM(){
	currentState = moveFoward;
}

SpaceshipFSM::~SpaceshipFSM(){
	
}

void SpaceshipFSM::updateState(){

	
}

void SpaceshipFSM::behaveState(){
	switch(currentState){
	case moveFoward:
		behaveMoveFoward();
		break;
	case avoidObstcle:
		behaveAvoidObstacle();
		break;
	case avoidOthers:
		behaveAvoidOthers();
		break;
	default:
		break;
	}
}
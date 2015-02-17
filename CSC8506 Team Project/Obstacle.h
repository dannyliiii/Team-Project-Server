//This class will represent each of the obstacles a track will contain
#pragma once
#include "../nclgl/SceneNode.h"
#include "PhysicsNode.h"
#include "GameEntity.h"
#include <vector>

enum OBSTACLE_TYPES{
		JUMPABLE,	//Elements that can be jumped
		ARC,		//Ars that you need to go through
		EXPLOSIVE,	//Stuff that explodes on toach perhaps?
		FALLING	//Stuff that falls from sky (perhaps meteors?)
};

enum ATMOSPHERE{
	DESERT,
	QUADS,
	FIRE,
	JUNGLE
};

class Obstacle{
public:
	Obstacle(int type, int atmosphere);
	Obstacle(int type, int atmosphere, Vector3 position);
	~Obstacle(void);
	void onCollision (GameEntity* player); //What happens when player collisions with this obstacle

	//Getters & Setters
	int getType() { return obstacle_type; }
	int getNGameEntities(){ return game_entities.size(); }
	Vector3 getPosition() { return position; }
	GameEntity* getEntityAt(int index) { return game_entities[index]; }
	

private:
	unsigned int obstacle_type;
	void GenerateJumpable (int atmosphere);
	void GenerateArc(int atmosphere);
	void GenerateExplosive (int atmosphere);
	void GenerateFalling (int atmosphere);
	
	float health; //some health in case this element can explode aftera  few collissions perhaps?
	std::vector <SceneNode*> scene_nodes;
	std::vector <PhysicsNode*> physics_nodes;
	std::vector <GameEntity*> game_entities;
	Mesh* obstacleMesh;
	Mesh* otherMesh; //Helper mesh to build complex figures
	Vector3 position;
	Vector3 debugPosition;

};
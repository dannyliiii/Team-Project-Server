#pragma once

#include <vector>
#include <iostream>
#include <queue>

#include "GameEntity.h"
#include "OBJMesh.h"
#include "SceneNode.h"
#include "PlayerEntity.h"
#include "Obstacle.h"

class Track;

class PlayerManager
{
public:

	PlayerManager(Track& track);

	~PlayerManager();

	

	void Update();
	void UpdatePlayers(float dt);

	Vector3 leaderPosition(){
		return leaderpos;
	}

	class Player : public SceneNode
	{
	public:

		Player(unsigned int id):
			id(id),
			speed(0.f), 
			distance(0.f) { }

		void Kill()
		{
			std::cout << "Player " << id << " died!" << std::endl;
		}

		unsigned int id;

		float speed;

		float distance;
	};

	// Player in front controls when the track advances.
	// All players have a display of "when" this will happen (i.e. distance of front player to trigger point).
	// When we hit the trigger point, players at the back are killed or whatever.
	// Track advances!

private:

	Track& track;

	typedef std::vector<GameEntity*> PlayersT;
	PlayersT players;

	OBJMesh cube;
	int debug_skybox;
	int difficulty;

	PlayerEntity* SpaceshipPlayer;
	SceneNode* playerScene;
	PhysicsNode* playerPhysics;

	vector<Obstacle*> obstacles;
	vector<Obstacle*> traps;
	Vector3 leaderpos;
};

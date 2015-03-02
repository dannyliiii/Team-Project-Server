#include "PlayerManager.h"

#include "Track.h"
#include "InertialMatrixHelper.h"
#include "Renderer.h"
#include "TriggerNode.h"

#include <cassert>
float TriggerDistance = 5.0f;
PlayerManager::PlayerManager(Track& track):
	track(track),
	cube(MESHDIR"spaceRanger.obj")
{
	Renderer& renderer = Renderer::GetRenderer();

	{
		playerScene = new SceneNode(&cube);
		playerPhysics = new PhysicsNode();
		
		playerPhysics->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(1.0f / 10.0f, 1, 1, 1));
		playerPhysics->SetInverseMass(1.0f / 10.0f);
		playerPhysics->SetLinearVelocity(Vector3(0, 0, 0));
		playerPhysics->SetAngularVelocity(Vector3(0, 0, 0));
		//playerPhysics->SetPosition(Vector3(0, 0, 0));
		playerPhysics->SetCollisionVolume(new CollisionCapsule(Vector3(100, 600, 300), 250.0f, Vector3(1000, 2000, 1000)));
		
		SpaceshipPlayer = new PlayerEntity(playerScene, playerPhysics);
		SpaceshipPlayer->ConnectToSystems();
		
		//SpaceshipPlayer->GetPhysicsNode().SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), 90.0f));
		SpaceshipPlayer->GetRenderNode().SetModelScale(Vector3(0.15, 0.15, 0.10));
		SpaceshipPlayer->GetPhysicsNode().SetUseGravity(false);
		SpaceshipPlayer->GetPhysicsNode().SetRestable(false);
		SpaceshipPlayer->GetPhysicsNode().SetInverseMass(0.1f);
		//SpaceshipPlayer->GetRenderNode().SetMaterial(shipMaterial);
		float bound = track.GetLowerBound();
		SpaceshipPlayer->GetPhysicsNode().SetPosition(track.GetPositionAt(bound));
		players.push_back(SpaceshipPlayer);
		
	}

	//{
	//	Player* player = new Player(0u);
	//	//player->speed = 1.0f;
	//	player->speed = 1.0f; // INCREASED SPEED FOR TESTING
	//	player->SetMesh(&cube);
	//	player->SetModelScale(Vector3(1.f));

	//	players.push_back(new GameEntity(player, nullptr));
	//	players.back()->ConnectToSystems();
	//}
	//{
	//	Player* player = new Player(1u);
	//	//player->speed = 0.86f;
	//	player->speed = 1.72f;
	//	player->SetMesh(&cube);
	//	player->SetModelScale(Vector3(1.f));

	//	players.push_back(new GameEntity(player, nullptr));
	//	players.back()->ConnectToSystems();
	//}
	//{
	//	Player* player = new Player(2u);
	//	//player->speed = 0.80f;
	//	player->speed = 1.6f;
	//	player->SetMesh(&cube);
	//	player->SetModelScale(Vector3(1.f));

	//	players.push_back(new GameEntity(player, nullptr));
	//	players.back()->ConnectToSystems();
	//}
	//{
	//	Player* player = new Player(3u);
	//	//player->speed = 0.84f;
	//	player->speed = 1.68f;
	//	player->SetMesh(&cube);
	//	player->SetModelScale(Vector3(1.f));

	//	players.push_back(new GameEntity(player, nullptr));
	//	players.back()->ConnectToSystems();
	//}
	//{
	//	Player* player = new Player(4u);
	//	//player->speed = 0.76f;
	//	player->speed = 1.52f;
	//	player->SetMesh(&cube);
	//	player->SetModelScale(Vector3(1.f));

	//	players.push_back(new GameEntity(player, nullptr));
	//	players.back()->ConnectToSystems();
	//}
	

	
	debug_skybox = 0;
	//load elements for each of the obstacles
	Obstacle::createCube();
	Obstacle::createCylinder();
	Obstacle::createQuad();
	Obstacle::createSphere();
	Obstacle::LoadTextures();

}

PlayerManager::~PlayerManager()
{
	for (PlayersT::iterator i = players.begin(); i != players.end(); ++i)
	{
		//(*i)->DisconnectFromSystems();
		//delete *i;
	}
}

void PlayerManager::Update()
{
	
	for (int i = 0; i < obstacles.size(); i++){
		Renderer::DrawDebugCross(DEBUGDRAW_PERSPECTIVE, obstacles[i]->getPosition(), Vector3(10, 10, 10), Vector3(1, 0, 0));
	}
	float lower = track.GetLowerBound();
	float upper = track.GetUpperBound();

	assert(upper - lower > TriggerDistance);

	float trigger = upper - TriggerDistance;

	bool advance = false;
	float leaderPosition = 0.f;

	for (PlayersT::iterator i = players.begin(); i != players.end(); ++i)
	{
		Player* player = static_cast<Player*>(&(*i)->GetRenderNode());

		if (player->distance >= trigger)
			advance = true;

		leaderPosition = max(leaderPosition, player->distance);
	}

	if (advance)
	{
		// Players must be beyond the last section...
		float min = lower + track.GetLengthOfFirstSection() + 1.5f;

		for (PlayersT::iterator i = players.begin(); i != players.end(); ++i)
		{
			Player* player = static_cast<Player*>(&(*i)->GetRenderNode());

			if (player->distance < min)
			{
				player->Kill();

				// for now put them back at the leader's position!
				// should probably be an average, but definitely safe position...
				// maybe half way between min and leader?
				player->distance = leaderPosition;
			}
		}

		float upperboundbefor = track.GetUpperBound();
		track.Advance();
		//here I will sample the terrain to see what type of skybox to change too
		//for now I'll use the temp value debug_skybox
		debug_skybox++;
		if (debug_skybox >= 5){
			debug_skybox = 1;
		}
		Renderer::GetRenderer().GetSkybox()->SetCubeMapValue(debug_skybox);
		
		
		float upperboundafter = track.GetUpperBound();

		float diff = upperboundafter - upperboundbefor;
		cout << "Chunck size: " << diff << endl;

		int random = rand() % 15 + 1;
		cout << "Number obstacles in this chunk: " << random << endl;
		float distance = track.GetUpperBound() - diff;
		
		cout << "Position of the first obstacle: " << distance << endl;
		float obstacle_diff = diff / random;
		

		for (int i = 0; i < random; i++){
			Obstacle *o = new Obstacle(rand() % 4, (debug_skybox - 1), track.GetPositionAt(distance), track.GetRotationAt(distance));
			
			o->setDistance(distance);
			obstacles.push_back(o);
			distance += obstacle_diff;
			cout << "New distace: " << distance << endl;
		}
		
	}	
	for (unsigned int i = 0; i < obstacles.size(); i++){
		
		if (obstacles[i]->getDistance() < lower){
			obstacles[i]->Disconnect();
			obstacles.erase(obstacles.begin() + i);
		}
	}



	//leaderpos = track.GetPositionAt(leaderPosition);
}

void PlayerManager::UpdatePlayers(float msec)
{
	SpaceshipPlayer->Update(msec);

	Vector3 splinePosition = SpaceshipPlayer->GetSplinePosition();
	Quaternion shipOrientation = SpaceshipPlayer->GetOrientation();

	Matrix4 transform = track.GetTransformAt(splinePosition.z);

	float playerBaseHeight = 5.f;

	Matrix4 translation = Matrix4::Translation(
		Vector3(splinePosition.x, playerBaseHeight + splinePosition.y, 0.f));

	Matrix4 result = transform * translation;

	Vector3 worldPosition = result.GetPositionVector();
	Quaternion worldOrientation = Quaternion::FromMatrix(result);

	Quaternion correction = Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), -90.0f);

	SpaceshipPlayer->GetPhysicsNode().SetPosition(worldPosition);
	SpaceshipPlayer->GetPhysicsNode().SetOrientation(correction * worldOrientation * shipOrientation);

	leaderpos = SpaceshipPlayer->GetPhysicsNode().GetPosition();

	/*for (PlayersT::iterator i = players.begin(); i != players.end(); ++i)
	{
		Player* player = static_cast<Player*>(&(*i)->GetRenderNode());

		player->distance += player->speed * (msec / 1000.f);
		
		Matrix4 transform = track.GetTransformAt(player->distance);
		
		Matrix4 translation = Matrix4::Translation(Vector3(0.f, 5.f, 0.f));

		player->SetTransform(transform * translation);
	}*/
}

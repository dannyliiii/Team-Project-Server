#include "Obstacle.h"
#include "InertialMatrixHelper.h"
#include "OBJMesh.h"
#include "ParticleSceneNode.h"

Obstacle::Obstacle (int type, int atmosphere){
	debugPosition = Vector3(800,800,800);
	otherMesh = NULL;
	obstacle_type = type;
	health = 100.0f;
	switch(obstacle_type){
	case JUMPABLE:
		GenerateJumpable(atmosphere);
		break;
	case ARC:
		GenerateArc(atmosphere);
		break;
	case EXPLOSIVE:
		GenerateExplosive(atmosphere);
		break;
	case FALLING:
		GenerateFalling(atmosphere);
		break;
	}
}

Obstacle::Obstacle (int type, int atmosphere, Vector3 position){
	debugPosition = position;
	otherMesh = NULL;
	obstacle_type = type;
	health = 100.0f;
	switch(obstacle_type){
	case JUMPABLE:
		GenerateJumpable(atmosphere);
		break;
	case ARC:
		GenerateArc(atmosphere);
		break;
	case EXPLOSIVE:
		GenerateExplosive(atmosphere);
		break;
	case FALLING:
		GenerateFalling(atmosphere);
		break;
	}
}

Obstacle::~Obstacle(void){
	delete obstacleMesh;
	if(otherMesh)
		delete otherMesh;
}





void Obstacle::GenerateArc(int atmosphere){
	position = debugPosition;
	//Since an arc will be composed of many meshes, there will be a scene and physics node for each composing element.
	SceneNode* root = new SceneNode(); // root sceneNode of the obstacle, it is an empty Node
	vector <PhysicsNode*> physicsNodes;
	vector <GameEntity*> gameEntities;
	int nElements = 0;
	
	Vector3 location(-20, 0, 0);
	
	switch(atmosphere){
	case QUADS:
		nElements = 3;
		obstacleMesh = new OBJMesh(MESHDIR"cube.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"whiteTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		for(int i = 0; i < nElements; i++) {
			root->AddChild(new SceneNode(obstacleMesh));
			physicsNodes.push_back(new PhysicsNode());
		}
		physicsNodes[0]->SetPosition(position + Vector3(0, 30, 0));
		physicsNodes[0]->SetCollisionVolume(new CollisionCapsule(physicsNodes[0]->GetPosition() - Vector3(25,-5,0), 5.0f, physicsNodes[0]->GetPosition() + Vector3(25,5,0)));
		root->getChildrenAtIndex(0)->SetModelScale(Vector3(25,5,5));
		physicsNodes[1]->SetPosition(position + Vector3(-20, 0, 0));
		physicsNodes[1]->SetCollisionVolume(new CollisionCapsule(physicsNodes[1]->GetPosition(), 5.0f, physicsNodes[1]->GetPosition() + Vector3(0,30,0)));
		root->getChildrenAtIndex(1)->SetModelScale(Vector3(5,15,5));
		physicsNodes[2]->SetPosition(position + Vector3(20,0, 0));
		physicsNodes[2]->SetCollisionVolume(new CollisionCapsule(physicsNodes[2]->GetPosition(), 5.0f, physicsNodes[2]->GetPosition() + Vector3(0, 30,0)));
		root->getChildrenAtIndex(2)->SetModelScale(Vector3(5,15,5));
		break;
	case DESERT:
		nElements = 3;
		obstacleMesh = new OBJMesh(MESHDIR"cube.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"ruinTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		otherMesh = new OBJMesh(MESHDIR"newCone.obj");
		otherMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"ruinTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		for(int i = 0; i < nElements; i++) {
			if(i < 2){
				root->AddChild(new SceneNode(otherMesh));
				root->getChildrenAtIndex(i)->SetModelScale(Vector3(5,15,5));
			}else {
				root->AddChild(new SceneNode(obstacleMesh));
				root->getChildrenAtIndex(i)->SetModelScale(Vector3(25,3,3));
			}
			physicsNodes.push_back(new PhysicsNode());
		}
		physicsNodes[0]->SetPosition(position + Vector3(-20, 0, 0));
		physicsNodes[0]->SetCollisionVolume(new CollisionCapsule(physicsNodes[0]->GetPosition(), 5.0f, physicsNodes[0]->GetPosition() + Vector3(0, 30,0)));
		physicsNodes[1]->SetPosition(position + Vector3( 20, 0, 0));
		physicsNodes[1]->SetCollisionVolume(new CollisionCapsule(physicsNodes[1]->GetPosition(), 5.0f, physicsNodes[1]->GetPosition() + Vector3(0, 30,0)));
		physicsNodes[2]->SetPosition(position + Vector3(0,30, 0));
		physicsNodes[2]->SetCollisionVolume(new CollisionCapsule(physicsNodes[2]->GetPosition() - Vector3(25,-3,0), 5.0f, physicsNodes[2]->GetPosition() + Vector3(25,3,0)));
		break;
	case FIRE:
		nElements = 4;
		obstacleMesh = Mesh::GenerateQuad();
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"volcano_texture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		for(unsigned int i = 0; i < nElements/2; i++){
			root->AddChild(new SceneNode(obstacleMesh));
			root->getChildrenAtIndex(i)->SetModelScale(Vector3(5,5,1));
			physicsNodes.push_back(new PhysicsNode());
			physicsNodes[i]->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-90,Vector3(1,0,0))));
			physicsNodes[i]->SetPosition(position + location);
			location += Vector3(40, 0,0);
		}
		location = Vector3(-20,0,0);
		for(unsigned int i = nElements/2; i < nElements; i++){
			root->AddChild(new ParticleSceneNode(position + location, LAVA_OBSTACLE));
			physicsNodes.push_back(new PhysicsNode());
			physicsNodes[i]->SetPosition(position + location);
			physicsNodes[i]->SetCollisionVolume(new CollisionCapsule(physicsNodes[i]->GetPosition(), 10.0f, physicsNodes[i]->GetPosition() + Vector3(0, 60,0)));
			location += Vector3(40,0,0);
		}
		break;
	case JUNGLE:
		nElements = 3;
		obstacleMesh = new OBJMesh(MESHDIR"newCone.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"jungle.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		for(int i = 0; i < nElements; i++) {
			root->AddChild(new SceneNode(obstacleMesh));
			if(i <2)
				root->getChildrenAtIndex(i)->SetModelScale(Vector3(5,15,5));
			else
				root->getChildrenAtIndex(i)->SetModelScale(Vector3(5, 25, 5));
			physicsNodes.push_back(new PhysicsNode());
		}
		physicsNodes[0]->SetPosition(position + Vector3(-20, 0, 0));
		physicsNodes[0]->SetCollisionVolume(new CollisionCapsule(physicsNodes[0]->GetPosition(), 5.0f, physicsNodes[0]->GetPosition() + Vector3(0, 30, 0)));
		physicsNodes[1]->SetPosition(position + Vector3( 20, 0, 0));
		physicsNodes[1]->SetCollisionVolume(new CollisionCapsule(physicsNodes[1]->GetPosition(), 5.0f, physicsNodes[1]->GetPosition() + Vector3(0, 30, 0)));
		physicsNodes[2]->SetPosition(position + Vector3(-25,30, 0));
		physicsNodes[2]->SetCollisionVolume(new CollisionCapsule(physicsNodes[2]->GetPosition(), 5.0f, physicsNodes[2]->GetPosition() + Vector3(50, 0, 0)));
		physicsNodes[2]->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(90.0f, Vector3(0,0,1))));
		break;
	}
	for(unsigned int i = 0; i < physicsNodes.size(); i++){
			physicsNodes[i]->setMoveable(false);
	}
	
	scene_nodes.push_back(root);
	for(int i = 0; i < nElements; i++){
		physics_nodes.push_back(physicsNodes[i]);
		gameEntities.push_back(new GameEntity(root->getChildrenAtIndex(i), physicsNodes[i]));
	}
	for(int i = 0; i < nElements; i++){
		gameEntities[i]->ConnectToSystems();
		game_entities.push_back(gameEntities[i]);
	}
	
}


void Obstacle::GenerateFalling(int atmosphere){
	position = debugPosition;
	Vector3 location(-20, 0, 0);
	float size = 0.0f;
	//Differently to the arc, where there was a root scene node for all the arc, here there are separate entities that will fall and have different behaviour
	
	int elements = 5; // this is supposed to be random, but for testing purposes, it will remain as 3
	

	switch(atmosphere){
	case QUADS:
		obstacleMesh = new OBJMesh(MESHDIR"cube.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"whiteTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		size = 10.0f;
		break;
	case DESERT:
		obstacleMesh = new OBJMesh(MESHDIR"sphere.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"ruinTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		size = 10.0f;
		break;
	case FIRE:
		obstacleMesh = new OBJMesh(MESHDIR"sphere.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"volcano_texture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		size = 10.0f;
		break;
	case JUNGLE:
		obstacleMesh = new OBJMesh(MESHDIR"sphere.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"safari_ball.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		size = 10.0f;
		break;

	}

	for (int i = 0; i < elements; i++){
		GameEntity* g = new GameEntity(new SceneNode(obstacleMesh), new PhysicsNode());
		g->ConnectToSystems();

		SceneNode &sn = g->GetRenderNode();
		sn.SetModelScale(Vector3(size, size, size));
		sn.SetBoundingRadius(size);

		PhysicsNode &pn = g->GetPhysicsNode();
		pn.SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(1.0f / 10.0f, size, size, size));
		pn.SetInverseMass(1.0f / 10.0f);
		pn.SetPosition(position + location);
		pn.SetCollisionVolume(new CollisionSphere(pn.GetPosition(), size));
		pn.SetRest(false);
		pn.SetLinearVelocity(Vector3(-0.5, -0.5, 0));
		pn.AddForce(pn.GetPosition(), Vector3(0, -1, 0));
		location += Vector3(30, -30, 0);
		physics_nodes.push_back(&pn);
		scene_nodes.push_back(&sn);
		game_entities.push_back(g);
	}
}



void Obstacle::GenerateExplosive(int atmosphere){
	position = debugPosition;
	PhysicsNode* pn = new PhysicsNode();
	SceneNode* sn = new SceneNode();
	pn->SetPosition(position);
	pn->setMoveable(false);

	switch(atmosphere){
	case QUADS:
		obstacleMesh = new OBJMesh(MESHDIR"cube.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"whiteTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sn->SetMesh(obstacleMesh);
		sn->SetModelScale(Vector3(5,5,5));
		pn->SetCollisionVolume(new CollisionSphere(position + Vector3(0, 5,0), 2.5f));
		break;
	case DESERT:
		obstacleMesh = new OBJMesh(MESHDIR"sphere.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"camouflage_texture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sn->SetMesh(obstacleMesh);
		sn->SetModelScale(Vector3(10, 2, 10));
		pn->SetCollisionVolume(new CollisionSphere(position, 10));
		break;
	case FIRE:
		obstacleMesh = new OBJMesh(MESHDIR"sphere.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"volcano_texture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sn->SetMesh(obstacleMesh);
		sn->SetModelScale(Vector3(5,5,5));
		pn->SetCollisionVolume(new CollisionSphere(position, 5.0f));
		break;
	case JUNGLE:
		obstacleMesh = new OBJMesh(MESHDIR"sphere.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"hexagonal-camo.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sn->SetMesh(obstacleMesh);
		sn->SetModelScale(Vector3(10, 2, 10));
		pn->SetCollisionVolume(new CollisionSphere(position, 10));
		break;
	}

	GameEntity* ge = new GameEntity(sn, pn);
	ge->ConnectToSystems();
	
	scene_nodes.push_back(sn);
	physics_nodes.push_back(pn);
	game_entities.push_back(ge);


}

void Obstacle::GenerateJumpable(int atmosphere){
	position = debugPosition;
	PhysicsNode* pn = new PhysicsNode();
	SceneNode* sn = new SceneNode();
	
	sn->SetBoundingRadius(40);
	
	pn->SetPosition(position);
	pn->setMoveable(false);
	pn->SetCollisionVolume(new CollisionCapsule(pn->GetPosition() - Vector3(40,0,0), 40, pn->GetPosition() + Vector3(40,0,0)));
	pn->SetRest(true);
		

	switch(atmosphere){
	case QUADS:
		obstacleMesh = new OBJMesh(MESHDIR"cube.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"whiteTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sn->SetMesh(obstacleMesh);
		sn->SetModelScale(Vector3(40, 5, 5));	
		break;
	case DESERT:
		obstacleMesh = new OBJMesh(MESHDIR"cube.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"ruinTexture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		obstacleMesh->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"ruin_bump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sn->SetMesh(obstacleMesh);
		sn->SetModelScale(Vector3(40,5,5));
		break;
	case FIRE:
		obstacleMesh = Mesh::GenerateQuad();
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"volcano_texture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sn->SetModelScale(Vector3(20,20,1));
		pn->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-90,Vector3(1,0,0))));
		sn->SetMesh(obstacleMesh);
		break;
	case JUNGLE:
		obstacleMesh = new OBJMesh(MESHDIR"newCone.obj");
		obstacleMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"jungle.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sn->SetMesh(obstacleMesh);
		sn->SetModelScale(Vector3(10,30,10));
		pn->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(90, Vector3(0,0,1))));
		break;
	}


	GameEntity* ge = new GameEntity(sn, pn);
	ge->ConnectToSystems();
	
	scene_nodes.push_back(sn);
	physics_nodes.push_back(pn);
	game_entities.push_back(ge);

}

void Obstacle::onCollision(GameEntity* player){
	Vector3 position;
	GameEntity* ge = NULL;
	switch(obstacle_type){
	case JUMPABLE:
		//decrease players health
		//dicrease obstacles health
		break;
	case ARC:
		//decrease players health
		//lets say arcs are undestructible
		break;
	case EXPLOSIVE:
		position = game_entities[0]->GetPhysicsNode().GetPosition();
		game_entities[0]->DisconnectFromSystems();
		ge = new GameEntity(new ParticleSceneNode(position, EXPLOSION), new PhysicsNode());
		ge->ConnectToSystems();
		game_entities.push_back(ge);
		//decrease the players health
		break;
	case FALLING:
		//just decrease the health of the player
		//and explode
		for(int i = 0; i < game_entities.size(); i++){
			position = game_entities[i]->GetPhysicsNode().GetPosition();
			game_entities[i]->DisconnectFromSystems();
			ge = new GameEntity(new ParticleSceneNode(position, EXPLOSION), new PhysicsNode());
			ge->ConnectToSystems();
			game_entities.push_back(ge);
		}
		break;
	}
}
#include "MyGame.h"
#include "InertialMatrixHelper.h"
#include "Spring.h"
#include "OBJMesh.h"
/*
Creates a really simple scene for our game - A cube robot standing on
a floor. As the module progresses you'll see how to get the robot moving
around in a physically accurate manner, and how to stop it falling
through the floor as gravity is added to the scene.

You can completely change all of this if you want, it's your game!

*/


MyGame::MyGame():
	cube(nullptr), quad(nullptr), sphere(nullptr) {

	activedPlayers = new int[MAX_PLAYER_NUMBER];

	for (int i = 0; i < MAX_PLAYER_NUMBER; i++){
		activedPlayers[i] = 0;
	}

	time = 0;
	gameState = waiting;
	
}

MyGame::~MyGame(void)	{
	/*
	We're done with our assets now, so we can delete them
	*/
	delete cube;
	delete quad;
	delete sphere;
	//delete theCube;
}

/*
Here's the base 'skeleton' of your game update loop! You will presumably
want your games to have some sort of internal logic to them, and this
logic will be added to this function.
*/
void MyGame::UpdateGame(float msec) {

	switch (gameState){
	case waiting:
		for (int i = 0; i < MAX_PLAYER_NUMBER; i++){
			if (sendPacket.activedPlayers[i] == 1 && activedPlayers[i] == 0){
				GameEntity* e = BuildSphereEntity(50.0, Vector3(0, 0, 0), Vector3(0, 0, 0));
				Vector3 vel(0, 0, -10);
				e->GetPhysicsNode().SetLinearVelocity(vel);
				e->GetPhysicsNode().SetPosition(Vector3(rand() % 1000, 1000, 0));
				e->GetPhysicsNode().SetAngularVelocity(vel*0.005f);
				e->GetPhysicsNode().SetInverseMass(1.0f / 1.0f);
				e->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f / 10.0f, 100.0f));
				allEntities.push_back(e);
				players.insert(pair<int, GameEntity*>(i, e));
				activedPlayers[i] = 1;
				cout << "Create player " << i << endl;
			}
			else if (sendPacket.activedPlayers[i] == 0 && activedPlayers[i] == 1){
				delete players.find(i)->second;
				players.erase(i);
				activedPlayers[i] = 0;
				cout << "Remove player " << i << endl;
			}
			else{
				continue;
			}
		}
		break;
	case started:
		break;
	case finished:
		break;
	default:
		break;
	}
	
	//for (vector<GameEntity*>::iterator i = allEntities.begin(); i != allEntities.end(); ++i) {
	//	(*i)->Update(msec);
	//}

	//press 1, launch projectile
	/*if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
		Matrix4 t = gameCamera->BuildViewMatrix();
		Vector3 vel = Vector3(-t.values[2], -t.values[6], -t.values[10]);
		GameEntity* e = BuildSphereEntity(5.0f, gameCamera->GetPosition(), vel);
		e->GetPhysicsNode().SetLinearVelocity(vel);
		e->GetPhysicsNode().SetPosition(gameCamera->GetPosition());
		e->GetPhysicsNode().SetAngularVelocity(vel*0.005f);
		e->GetPhysicsNode().SetInverseMass(1.0f / 1.0f);
		e->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f / 10.0f, 100.0f));
		allEntities.push_back(e);
	}*/

	//////////////////////////////////////Spaceship Movements

	//move forward
	//if (Window::GetKeyboard()->KeyHeld(KEYBOARD_UP)) {
	//	theCube->GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, -3));
	//}
	//else if (!Window::GetKeyboard()->KeyHeld(KEYBOARD_UP)) {
	//	theCube->GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, 0));
	//}

	////move back
	//if (Window::GetKeyboard()->KeyHeld(KEYBOARD_DOWN)) {
	//	theCube->GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, 3));
	//}

	////move left
	//if (Window::GetKeyboard()->KeyHeld(KEYBOARD_LEFT) && roll > -10.0f) {
	//	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_UP) && roll > -10.0f) {
	//		roll -= 1.0f;
	//		theCube->GetPhysicsNode().SetLinearVelocity(Vector3(-3, 0, -3));
	//		Quaternion rotation3 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), -1.0f);
	//		Quaternion currentRotation3 = theCube->GetPhysicsNode().GetOrientation();
	//		theCube->GetPhysicsNode().SetOrientation(currentRotation3 * rotation3);
	//	}
	//	else if (Window::GetKeyboard()->KeyHeld(KEYBOARD_DOWN) && roll > -10.0f) {
	//		roll -= 1.0f;
	//		theCube->GetPhysicsNode().SetLinearVelocity(Vector3(-3, 0, 0));
	//		Quaternion rotation4 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), -1.0f);
	//		Quaternion currentRotation4 = theCube->GetPhysicsNode().GetOrientation();
	//		theCube->GetPhysicsNode().SetOrientation(currentRotation4 * rotation4);
	//	}
	//	else if (!Window::GetKeyboard()->KeyHeld(KEYBOARD_LEFT)) {
	//		Quaternion rotation4 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 0.0f);
	//		Quaternion currentRotation4 = theCube->GetPhysicsNode().GetOrientation();
	//		theCube->GetPhysicsNode().SetOrientation(currentRotation4 * rotation4);
	//	}
	//	else {
	//		roll -= 1.0f;
	//		theCube->GetPhysicsNode().SetLinearVelocity(Vector3(-3, 0, 0));
	//		Quaternion rotation = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), -1.0f);
	//		Quaternion currentRotation = theCube->GetPhysicsNode().GetOrientation();
	//		theCube->GetPhysicsNode().SetOrientation(currentRotation * rotation);
	//	}
	//}
	//

	////move right
	//if (Window::GetKeyboard()->KeyHeld(KEYBOARD_RIGHT) && roll < 10.0f) {
	//	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_UP) && roll < 10.0f) {
	//		roll += 1.0f;
	//		theCube->GetPhysicsNode().SetLinearVelocity(Vector3(3, 0, -3));
	//		Quaternion rotation4 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 1.0f);
	//		Quaternion currentRotation4 = theCube->GetPhysicsNode().GetOrientation();
	//		theCube->GetPhysicsNode().SetOrientation(currentRotation4 * rotation4);
	//	}
	//	else if (Window::GetKeyboard()->KeyHeld(KEYBOARD_DOWN) && roll < 10.0f) {
	//		roll += 1.0f;
	//		theCube->GetPhysicsNode().SetLinearVelocity(Vector3(3, 0, 0));
	//		Quaternion rotation4 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 1.0f);
	//		Quaternion currentRotation4 = theCube->GetPhysicsNode().GetOrientation();
	//		theCube->GetPhysicsNode().SetOrientation(currentRotation4 * rotation4);
	//	}
	//	else {
	//		roll += 1.f;
	//		theCube->GetPhysicsNode().SetLinearVelocity(Vector3(3, 0, 0));
	//		Quaternion rotation2 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 1.0f);
	//		Quaternion currentRotation2 = theCube->GetPhysicsNode().GetOrientation();
	//		theCube->GetPhysicsNode().SetOrientation(currentRotation2 * rotation2);
	//	}
	//}


	////Barrel Roll
	//if (Window::GetKeyboard()->KeyHeld(KEYBOARD_B)) {
	//	theCube->GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, -3));
	//	Quaternion rotation = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 10.0f);
	//	Quaternion currentRotation = theCube->GetPhysicsNode().GetOrientation();
	//	theCube->GetPhysicsNode().SetOrientation(currentRotation * rotation);
	//}

	////jump
	//if (Window::GetKeyboard()->KeyHeld(KEYBOARD_P) && yaw > -10.0f) {
	//	yaw -= 1.0f;
	//	theCube->GetPhysicsNode().SetLinearVelocity(Vector3(0, 3, -3));
	//	Quaternion rotation = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 1), -5.0f);
	//	Quaternion currentRotation = theCube->GetPhysicsNode().GetOrientation();
	//	theCube->GetPhysicsNode().SetOrientation(currentRotation * rotation);
	//	theCube->GetPhysicsNode().SetUseGravity(TRUE);
	//	if (yaw > -10.0f) {
	//		Quaternion rotation = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 1), 5.0f);
	//		Quaternion currentRotation = theCube->GetPhysicsNode().GetOrientation();
	//		theCube->GetPhysicsNode().SetOrientation(currentRotation * rotation);
	//		theCube->GetPhysicsNode().SetUseGravity(TRUE);
	//	}
	//}

	////supersonic speed 
	//if (Window::GetKeyboard()->KeyHeld(KEYBOARD_L)) {
	//	theCube->GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, -15));
	//}

	//if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Y)) {
	//	track.Advance();
	//	std::cout << "Track Extents: " << track.GetLowerBound() << " " << track.GetUpperBound() << std::endl;
	//}

}

/*
Makes a cube. Every game has a crate in it somewhere!
*/
GameEntity* MyGame::BuildCubeEntity(float size) {
	//GameEntity*g = new GameEntity(new SceneNode(cube), new PhysicsNode());
	GameEntity*g = new GameEntity(new PhysicsNode());

	g->ConnectToSystems();

	//SceneNode &test = g->GetRenderNode();

	/*test.SetModelScale(Vector3(size, size, size));
	test.SetBoundingRadius(size);*/
	PhysicsNode& p = g->GetPhysicsNode();
	p.SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(1.0f/10.0f, size, size, size));
	p.SetInverseMass(1.0f / 10.0f);
	p.SetLinearVelocity(Vector3(0, 0, 0));
	p.SetAngularVelocity(Vector3(0, 0, 0));
	p.SetPosition(Vector3(0, 0, 0));
	p.SetCollisionVolume(new CollisionCapsule(Vector3(100, 600, 300), 50.0f, Vector3(1000, 2000, 1000)));
	return g;
}
/*
Makes a sphere.
*/
GameEntity* MyGame::BuildSphereEntity(float radius, Vector3 pos, Vector3 vel) {
	/*SceneNode* s = new SceneNode(sphere);
	s->SetMaterial(checkboardMaterial);

	s->SetModelScale(Vector3(radius, radius, radius));
	s->SetBoundingRadius(radius);
	s->SetColour(Vector4(0.2f, 0.2f, 0.5f, 1.0f));*/
	PhysicsNode*p = new PhysicsNode();
	p->SetPosition(pos);
	p->SetLinearVelocity(vel);
	p->SetAngularVelocity(Vector3(0, 0, 0));
	p->SetInverseInertia(InertialMatrixHelper::createSphereInvInertial(1.0f, radius));
	p->SetInverseMass(1.0f);
	p->SetCollisionVolume(new CollisionSphere(pos, radius));
	GameEntity*g = new GameEntity(p);
	g->ConnectToSystems();
	//PhysicsSystem::GetPhysicsSystem().AddDebugDraw(p->GetCollisionVolume());
	return g;
}

/*
Makes a flat quad, initially oriented such that we can use it as a simple
floor.
*/
GameEntity* MyGame::BuildQuadEntity(float size) {
	//SceneNode* s = new SceneNode(quad);
	//s->SetMaterial(checkSmileyMaterial);

	//s->SetModelScale(Vector3(size, size, size));
	////Oh if only we had a set texture function...we could make our brick floor again WINK WINK
	//s->SetBoundingRadius(size);

	PhysicsNode*p = new PhysicsNode(Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 90.0f), Vector3());
	p->SetUseGravity(false);
	p->SetInverseMass(0);
	p->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
	p->SetCollisionVolume(new CollisionPlane(Vector3(0,0,0), Vector3(0, 1, 0), 0));
	p->SetLinearVelocity(Vector3(0,0,0));
	GameEntity*g = new GameEntity(p);
	g->ConnectToSystems();
	return g;
}

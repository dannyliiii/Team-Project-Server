#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(void){
	health = 100.0f;
	lives = 3;
	is_Player = true;
	roll = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
}

PlayerEntity::PlayerEntity(SceneNode* sn, PhysicsNode* pn):
GameEntity(sn, pn)
{
	health = 100.0f;
	lives = 3;
	is_Player = true;
	roll = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
}

PlayerEntity::PlayerEntity(Sound* snd, SceneNode* s, SoundNode* sn):GameEntity(snd, s, sn){
	health = 100.0f;
	lives = 3;
	is_Player = true;
	roll = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
}

void PlayerEntity::Update(float msec){
	//////////////////////////////////////Spaceship Movements

	Vector3 frameVelocity = Vector3(0.f);

	//move forward
	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_UP)) {
		frameVelocity = Vector3(0, 0, -3.0f);
		GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, -3.0f));
	}
	else if (!Window::GetKeyboard()->KeyHeld(KEYBOARD_UP)) {
		frameVelocity = Vector3(0, 0, 0);
		GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, 0));
	}

	//move back
	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_DOWN)) {
		frameVelocity = Vector3(0, 0, 3.0f);
		GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, 3.0f));
	}

	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_M)) {
		frameVelocity = Vector3(0, -3.0, 0);
		GetPhysicsNode().SetLinearVelocity(Vector3(0, -3.0, 0));
	}

	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_N)) {
		frameVelocity = Vector3(0, 3, 0);
		GetPhysicsNode().SetLinearVelocity(Vector3(0, 3, 0));
	}

	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_LEFT)) {
		if (Window::GetKeyboard()->KeyHeld(KEYBOARD_UP)) {

			frameVelocity = Vector3(-3, 0, -3);
			GetPhysicsNode().SetLinearVelocity(Vector3(-3, 0, -3));
			if (roll > -10.0f) {
				roll -= 1.0f;
				Quaternion rotation3 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), -1.0f);
				shipOrientation = rotation3;
				Quaternion currentRotation3 = GetPhysicsNode().GetOrientation();
				GetPhysicsNode().SetOrientation(currentRotation3 * rotation3);
			}
		}
		else if (Window::GetKeyboard()->KeyHeld(KEYBOARD_DOWN)) {
			frameVelocity = Vector3(-3, 0, 3);
			GetPhysicsNode().SetLinearVelocity(Vector3(-3, 0, 3));
			if (roll > -10.0f) {
				roll -= 1.0f;
				Quaternion rotation3 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), -1.0f);
				shipOrientation = rotation3;
				Quaternion currentRotation3 = GetPhysicsNode().GetOrientation();
				GetPhysicsNode().SetOrientation(currentRotation3 * rotation3);
			}
		}
		else {
			frameVelocity = Vector3(-3, 0, 0);
			GetPhysicsNode().SetLinearVelocity(Vector3(-3, 0, 0));
			if (roll > -10.0f) {
				roll -= 1.0f;
				Quaternion rotation3 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), -1.0f);
				shipOrientation = rotation3;
				Quaternion currentRotation3 = GetPhysicsNode().GetOrientation();
				GetPhysicsNode().SetOrientation(currentRotation3 * rotation3);
			}

		}
	}

	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_RIGHT)) {
		if (Window::GetKeyboard()->KeyHeld(KEYBOARD_UP)) {
			frameVelocity = Vector3(3, 0, -3);
			GetPhysicsNode().SetLinearVelocity(Vector3(3, 0, -3));
			if (roll < 10.0f) {
				roll += 1.0f;
				Quaternion rotation4 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 1.0f);
				shipOrientation = rotation4;
				Quaternion currentRotation4 = GetPhysicsNode().GetOrientation();
				GetPhysicsNode().SetOrientation(currentRotation4 * rotation4);
			}
		}
		else if (Window::GetKeyboard()->KeyHeld(KEYBOARD_DOWN)) {
			frameVelocity = Vector3(3, 0, 3);
			GetPhysicsNode().SetLinearVelocity(Vector3(3, 0, 3));
			if (roll < 10.0f) {
				roll += 1.0f;
				Quaternion rotation3 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 1.0f);
				shipOrientation = rotation3;
				Quaternion currentRotation3 = GetPhysicsNode().GetOrientation();
				GetPhysicsNode().SetOrientation(currentRotation3 * rotation3);
			}
		}
		else {
			frameVelocity = Vector3(3, 0, 0);
			GetPhysicsNode().SetLinearVelocity(Vector3(3, 0, 0));
			if (roll < 10.0f) {
				roll += 1.0f;
				Quaternion rotation3 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 1.0f);
				shipOrientation = rotation3;
				Quaternion currentRotation3 = GetPhysicsNode().GetOrientation();
				GetPhysicsNode().SetOrientation(currentRotation3 * rotation3);
			}
		}
	}


	if (!Window::GetKeyboard()->KeyHeld(KEYBOARD_LEFT) && !Window::GetKeyboard()->KeyHeld(KEYBOARD_RIGHT)) {
		Quaternion rotation4;
		if (roll < 0) {
			rotation4 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 1.0f);
			roll += 1.f;
		}
		else if (roll > 0) {
			rotation4 = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), -1.0f);
			roll -= 1.f;
		}
		shipOrientation = rotation4;
		GetPhysicsNode().SetOrientation(GetPhysicsNode().GetOrientation() * rotation4);
	}

	//Barrel Roll
	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_B)) {
		frameVelocity = Vector3(0, 0, -3);
		GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, -3));
		Quaternion rotation = Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 10.0f);
		shipOrientation = rotation;
		Quaternion currentRotation = GetPhysicsNode().GetOrientation();
		GetPhysicsNode().SetOrientation(currentRotation * rotation);
	}

	//jump
	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_P) && yaw > -10.0f) {
		yaw -= 1.0f;
		frameVelocity = Vector3(0, 3, -3);
		GetPhysicsNode().SetLinearVelocity(Vector3(0, 3, -3));
		Quaternion rotation = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 1), -5.0f);
		shipOrientation = rotation;
		Quaternion currentRotation = GetPhysicsNode().GetOrientation();
		GetPhysicsNode().SetOrientation(currentRotation * rotation);
		GetPhysicsNode().SetUseGravity(TRUE);
		if (yaw > -10.0f) {
			Quaternion rotation = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 1), 5.0f);
			shipOrientation = rotation;
			Quaternion currentRotation = GetPhysicsNode().GetOrientation();
			GetPhysicsNode().SetOrientation(currentRotation * rotation);
			GetPhysicsNode().SetUseGravity(TRUE);
		}
	}

	//supersonic speed 
	if (Window::GetKeyboard()->KeyHeld(KEYBOARD_L)) {
		frameVelocity = Vector3(0, 0, -15);
		GetPhysicsNode().SetLinearVelocity(Vector3(0, 0, -15));
	}

	GetPhysicsNode().SetLinearVelocity(Vector3(0.f));

	Vector3 splineVelocity = Vector3(frameVelocity.x, frameVelocity.y, -frameVelocity.z);

	Vector3 velocityScale = Vector3(0.001f);
	splineVelocity *= velocityScale;

	splinePosition = splinePosition + splineVelocity * msec;

	if (splinePosition.z < 0.f)
		splinePosition.z = 0.f;

}
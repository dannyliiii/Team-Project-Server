#include "PhysicsNode.h"

const Vector3 PhysicsNode::gravity = Vector3(0.0f, -0.01f, 0.0f);

PhysicsNode::PhysicsNode(void) : vol(NULL) {
	target = NULL;
	useGravity = true;
	m_angularVelocity = Vector3(0, 0, 0);
	m_linearVelocity = Vector3(0, 0, 0);
	m_invMass = 0;
	m_force = Vector3(0, 0, 0);
	m_torque = Vector3(0, 0, 0);
	m_position = Vector3(0, 0, 0);
	m_invInertia = Matrix4();
	m_atRest = false;
	m_restable = true;
	moveable = true;
}

PhysicsNode::PhysicsNode(Quaternion orientation, Vector3 position) : vol(NULL) {
	m_orientation = orientation;
	m_position = position;
	useGravity = true;
	m_angularVelocity = Vector3(0, 0, 0);
	m_linearVelocity = Vector3(0, 0, 0);
	m_invMass = 0;
	m_force = Vector3(0, 0, 0);
	m_torque = Vector3(0, 0, 0);
	m_invInertia = Matrix4();
	m_atRest = false;
	m_restable = true;
	moveable = true;
}

PhysicsNode::~PhysicsNode(void)	{

}

//You will perform your per-object physics integration, here!
//I've added in a bit that will set the transform of the
//graphical representation of this object, too.
void	PhysicsNode::Update(float msec) {
	//FUN GOES HERE
	if(moveable){
		//check if should be resting and is allowed to rest
		if (Vector3::Dot(m_linearVelocity, m_linearVelocity) < 0.000000005f && m_restable) {
			m_linearVelocity = Vector3(0, 0, 0);
			m_angularVelocity = Vector3(0, 0, 0);
			m_atRest = true;
		}
		else {
			m_atRest = false;
		}

	

		if (!m_atRest) {
			Vector3 acc = m_force*m_invMass + (useGravity ? gravity : Vector3(0, 0, 0));
			m_linearVelocity = m_linearVelocity + acc*msec;


			m_linearVelocity = m_linearVelocity*LINEAR_VELOCITY_DAMP;

			m_position = m_position + m_linearVelocity*msec;
	
			vol->SetPos(m_position);
		

			Vector3 angAcc = m_invInertia*m_torque;
			m_angularVelocity = m_angularVelocity + angAcc*msec;
			m_angularVelocity = m_angularVelocity*ANGULAR_VELOCITY_DAMP;
			m_orientation = m_orientation + m_orientation*(m_angularVelocity*(msec / 2.0f));
			m_orientation.Normalise();

			m_force = Vector3(0, 0, 0);
			m_torque = Vector3(0, 0, 0);

		}
	}

	if (target) {
		target->SetTransform(BuildTransform());
	}

	//if(vol){
	//	CollisionCapsule& cc = *(CollisionCapsule*)vol;
	//	Renderer::DrawDebugCross(DEBUGDRAW_PERSPECTIVE, cc.GetPos(), Vector3(20,20,20), Vector3(1,0,0));
	//	Renderer::DrawDebugCross(DEBUGDRAW_PERSPECTIVE, cc.GetEp(), Vector3(20,20,20), Vector3(0,0,1));
	//}
	
}

void PhysicsNode::AddForce(Vector3 point, Vector3 force) {
	m_force = m_force + force;
	m_torque = m_torque + Vector3::Cross(point - m_position, force);
}

/*
This function simply turns the orientation and position
of our physics node into a transformation matrix, suitable
for plugging into our Renderer!

It is cleaner to work with matrices when it comes to rendering,
as it is what shaders expect, and allow us to keep all of our
transforms together in a single construct. But when it comes to
physics processing and 'game-side' logic, it is much neater to
have seperate orientations and positions.

*/
Matrix4		PhysicsNode::BuildTransform() {
	Matrix4 m = m_orientation.ToMatrix();

	m.SetPositionVector(m_position);

	return m;
}
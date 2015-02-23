#include "PhysicsSystem.h"
#include "CollisionHelper.h"

PhysicsSystem* PhysicsSystem::instance = 0;
int PhysicsSystem::numUpdates = 0;
int PhysicsSystem::numUpdatesTotal = 0;
int PhysicsSystem::numNodes = 0;
int PhysicsSystem::numNodesRest = 0;
int PhysicsSystem::numCollisions = 0;
int PhysicsSystem::numUpdatesPerSec = 0;

PhysicsSystem::PhysicsSystem(void)	{
	//hm = new HeightMap(TEXTUREDIR"terrain.raw");
}

PhysicsSystem::~PhysicsSystem(void)	{
	for (unsigned int i = 0; i < allSprings.size(); i++) {
		delete allSprings[i];
	}
}

void	PhysicsSystem::Update(float msec) {
	numUpdates++;
	numUpdatesTotal++;
	std::lock_guard<std::mutex> lock(node_lock);
	numNodes = allNodes.size();
	BroadPhaseCollisions();
	NarrowPhaseCollisions();

	for (vector<Constraint*>::iterator i = allSprings.begin(); i != allSprings.end(); ++i) {
		(*i)->Update(msec);
	}
	for (vector<PhysicsNode*>::iterator i = allNodes.begin(); i != allNodes.end(); ++i) {
		(*i)->Update(msec);
	}

}

void	PhysicsSystem::BroadPhaseCollisions() {

}

void	PhysicsSystem::NarrowPhaseCollisions() {
	for (unsigned int i = 0; i < allNodes.size(); i++) {
		//left -> terrain collision
		if (allNodes[i]->GetCollisionVolume() && allNodes[i]->GetCollisionVolume()->GetType() == COLLISION_SPHERE) 
			//CollisionHelper::HeightMapCollision(hm->GetVertices(), hm->GetNormals(),*allNodes[i]);

		//for all left children
		for (unsigned int j = 0; j < allNodes[i]->children.size(); ++j) {
			//against each right node
			for (unsigned int k = i + 1; k < allNodes.size(); ++k) {
				NarrowPhaseHelper(allNodes[i]->children[j], allNodes[k]);
				//against each right child
				for (unsigned int l = 0; l < allNodes[k]->children.size(); ++l) {
					NarrowPhaseHelper(allNodes[k]->children[l], allNodes[i]->children[j]);
				}
			}
		}

		//left -> right collision
		for (unsigned int j = i + 1; j < allNodes.size(); j++) {
			NarrowPhaseHelper(allNodes[i], allNodes[j]);
			//left -> right children collision
			for (unsigned int k = 0; k < allNodes[j]->children.size(); ++k) {
				NarrowPhaseHelper(allNodes[i], allNodes[j]->children[k]);
			}
		}


	}
}

/*Helper function to prevent NarrowPhaseCollisions turning into one huge ugly bastard function*/
void PhysicsSystem::NarrowPhaseHelper(PhysicsNode* left, PhysicsNode* right) {
	CollisionVolume* fv = left->GetCollisionVolume();
	CollisionVolume* sv = right->GetCollisionVolume();
	

	if (!fv || !sv) return;
	CollisionData data;

	if (fv->GetType() == COLLISION_SPHERE) {
		if (sv->GetType() == COLLISION_SPHERE) {
			if (CollisionHelper::SphereSphereCollision(*left, *right, &data)) {
				CollisionHelper::AddCollisionImpulse(*left, *right, data);
			}
		}
		if (sv->GetType() == COLLISION_CAPSULE) {
			if (CollisionHelper::SphereCapsuleCollision(*left, *right, &data)) {
				CollisionHelper::AddCollisionImpulse(*left, *right, data);
			}
		}
	}
	else if (fv->GetType() == COLLISION_PLANE) {
		if (sv->GetType() == COLLISION_SPHERE) {
			if (CollisionHelper::PlaneSphereCollision(*left, *right, &data)) {
				CollisionHelper::AddCollisionImpulse(*left, *right, data);
			}
		}
	}
	else if (fv->GetType() == COLLISION_CAPSULE) {
		if (sv->GetType() == COLLISION_SPHERE) {
			if (CollisionHelper::SphereCapsuleCollision(*right, *left, &data)) {
				CollisionHelper::AddCollisionImpulse(*right, *left, data);
			}
		}
	}
}

//mutex needed as threading was playing up without
void	PhysicsSystem::AddNode(PhysicsNode* n) {
	std::lock_guard<std::mutex> lock(node_lock);
	allNodes.push_back(n);
}

void	PhysicsSystem::RemoveNode(PhysicsNode* n) {
	std::lock_guard<std::mutex> lock(node_lock);
	for (vector<PhysicsNode*>::iterator i = allNodes.begin(); i != allNodes.end(); ++i) {
		if ((*i) == n) {
			allNodes.erase(i);
			return;
		}
	}
}

void	PhysicsSystem::AddConstraint(Constraint* s) {
	allSprings.push_back(s);
}

void	PhysicsSystem::RemoveConstraint(Constraint* c) {
	for (vector<Constraint*>::iterator i = allSprings.begin(); i != allSprings.end(); ++i) {
		if ((*i) == c) {
			allSprings.erase(i);
			return;
		}
	}
}

void	PhysicsSystem::AddDebugDraw(DebugDrawer* d) {
	allDebug.push_back(d);
}

void	PhysicsSystem::RemoveDebugDraw(DebugDrawer* d) {
	for (vector<DebugDrawer*>::iterator i = allDebug.begin(); i != allDebug.end(); ++i) {
		if ((*i) == d) {
			allDebug.erase(i);
			return;
		}
	}
}

void    PhysicsSystem::DrawDebug() {
	for (vector<DebugDrawer*>::iterator i = allDebug.begin(); i != allDebug.end(); ++i) {
		(*i)->DebugDraw();
	}
}
#pragma once
#include "../nclgl/SceneNode.h"
#include "ParticleEmitter.h"

enum PARTICLE_TYPE{
	LAVA_OBSTACLE,
	EXPLOSION
};

class ParticleSceneNode: public SceneNode{
public:
	ParticleSceneNode(Vector3 position, int type);
	~ParticleSceneNode(void);
	virtual void Draw(const OGLRenderer &r);
	virtual void Update(float msec);

private:
	ParticleEmitter* pe;
	unsigned int m_type;

};
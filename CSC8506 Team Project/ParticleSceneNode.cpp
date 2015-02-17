#include "ParticleSceneNode.h"

ParticleSceneNode::ParticleSceneNode(Vector3 position, int type){
	isParent = false;
	m_type = type;
	pe = new ParticleEmitter();
	pe->SetPosition(position);
	SetShader(new Shader(SHADERDIR"VertexParticle.glsl", 
		SHADERDIR"FragmentParticle.glsl", SHADERDIR"GeometryParticle.glsl"));
	if(!shader->LinkProgram())
		return;

	
}

ParticleSceneNode::~ParticleSceneNode(void){
	delete pe;
}

void ParticleSceneNode::Update(float msec){
	pe->Update(msec);	
}

void ParticleSceneNode::Draw(const OGLRenderer &r){
	glEnable(GL_DEPTH_TEST);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "particleSize"), pe->GetParticleSize());
	switch(m_type){
	case LAVA_OBSTACLE:
		pe->SetParticleSize(0.5f);
		pe->SetParticleVariance(0.2f);
		pe->SetLaunchParticles(256.0f);
		pe->SetParticleLifetime(3000.0f);
		pe->SetParticleSpeed(0.02f);
		pe->SetDirection(Vector3(0,1,0));
		break;
	case EXPLOSION:
		pe->SetParticleSize(2.0f);
		pe->SetParticleVariance(4.0f);
		pe->SetLaunchParticles(32.0f);
		pe->SetParticleLifetime(500.0f);
		pe->SetParticleSpeed(0.02f);
		pe->SetDirection(Vector3(0,1,0));
		break;
	}
	
	pe->Draw();
	glDisable(GL_DEPTH_TEST);
	
}
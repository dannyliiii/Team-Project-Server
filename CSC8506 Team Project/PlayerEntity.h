#include "GameEntity.h"

class PlayerEntity : public GameEntity{
public:
	PlayerEntity(void);
	PlayerEntity(SceneNode* sn, PhysicsNode* pn);
	PlayerEntity(Sound* snd, SceneNode* s, SoundNode* sn);
	
	~PlayerEntity(void){}

	float getHealth() { return health; }
	void increaseHealth(float val = 10.0f) { health += val; }
	void decreaseHealth(float val = 10.0f) { health -= val; }

	int getLives() { return lives; }
	void increaseLives(int val = 1){ lives += val; }
	void decreaseLives(int val = 1){ lives -= val; }

	virtual void Update(float msec);
	
	Vector3 GetSplinePosition() const { return splinePosition; }
	Quaternion GetOrientation() const { return shipOrientation; }

private:
	float health;
	int lives;
	float roll;
	float yaw;
	float pitch;

	Vector3 splinePosition;
	Quaternion shipOrientation;
};
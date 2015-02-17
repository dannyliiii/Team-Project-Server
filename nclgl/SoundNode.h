#pragma once
#include "SceneNode.h"
#include "Sound.h"
#include "SoundSystem.h"

enum SoundPriority {
	SOUNDPRIORTY_LOW,
	SOUNDPRIORITY_MEDIUM,
	SOUNDPRIORITY_HIGH,
	SOUNDPRIORITY_ALWAYS
};

#define NUM_STREAM_BUFFERS 3	//Part 2

struct OALSource;

class SoundNode : public SceneNode	{
public:
	SoundNode(void);
	SoundNode(Sound* s);
	~SoundNode(void);

	void			Reset();

	void			SetSound(Sound *s);
	Sound*			GetSound()					 {return sound;}

	void			SetPriority(SoundPriority p) {priority = p;}
	SoundPriority	GetPriority()				 {return priority;}

	void			SetVolume(float volume)		 {volume = min(1.0f, max(0.0f, volume));}
	float			GetVolume()					 {return volume;}

	void			SetLooping(bool state)		 {isLooping = state;}
	bool			GetLooping()				 {return isLooping;}

	void			SetRadius(float value)		 {radius = max(0.0f, value);}
	float			GetRadius()					 {return radius;}

	float			GetPitch()					 {return pitch;}
	void			SetPitch(float value)		 {pitch = value;}

	bool			GetIsGlobal()				 {return isGlobal;}
	void			SetIsGlobal(bool value)		 {isGlobal = value;}

	double			GetTimeLeft()				 {return timeLeft;}

	OALSource*		GetSource()					 {return oalSource;}

	static bool		CompareNodesByPriority(SoundNode *a, SoundNode* b);

	void			AttachSource(OALSource* s);
	void			DetachSource();
	
	virtual void	Update(float msec);

	void	SetTarget(SceneNode *s) { target = s; }
	SceneNode*	target;

protected:
	Sound*			sound;
	OALSource*		oalSource;
	SoundPriority	priority;
	float			volume;
	float			radius;
	float			pitch;
	bool			isLooping;
	bool			isGlobal;
	double			timeLeft;

	double			streamPos;							//Part 2
	ALuint			streamBuffers[NUM_STREAM_BUFFERS];	//Part 2
};
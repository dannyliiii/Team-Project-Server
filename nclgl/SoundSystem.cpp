#include "SoundSystem.h"

SoundSystem* SoundSystem::instance = NULL;

SoundSystem::SoundSystem(unsigned int channels) {
	listener = new SceneNode();
	masterVolume = 1.0f;

	cout << "Creating SoundSystem!" << endl;
	cout << "Found the following devices: " << alcGetString(NULL,ALC_DEVICE_SPECIFIER) << endl;	//outputs all OAL devices

	device = alcOpenDevice(NULL);	//Open the 'best' device

	if(!device) {
		cout << "Failed to create SoundSystem! (No valid device!)" << endl;
	}

	cout << "SoundSystem created with device: " << alcGetString(device,ALC_DEVICE_SPECIFIER) << endl;	//Outputs used OAL device!

	context = alcCreateContext(device,NULL);
	alcMakeContextCurrent(context);

	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

	for(unsigned int i = 0; i < channels; ++i) {
		ALuint source;

		alGenSources(1,&source);
		ALenum error = alGetError();

		if(error == AL_NO_ERROR)	{
			sources.push_back(new OALSource(source));
		}
		else{
			break;
		}
	}

	cout << "SoundSystem has " << sources.size() << " channels available!"  << endl;
}

SoundSystem::~SoundSystem(void)	{
	for(vector<SoundNode*>::iterator i = temporaryEmitters.begin(); i != temporaryEmitters.end(); ++i) {
		delete (*i);
	}

	for(vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i) {
		alDeleteSources(1, &(*i)->source);
		delete (*i);
	}

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void SoundSystem::Update(float msec) {
	UpdateListener();
	UpdateTemporaryEmitters(msec);

	for ( vector < SoundNode * >:: iterator i = emitters . begin ();
		i != emitters . end (); ++ i ) {
		frameEmitters . push_back ((* i ));
		(* i ) -> Update ( msec );
	}

	CullNodes();	//First off, remove nodes that are too far away

	if(frameEmitters.size() > sources.size()) {
		std::sort(frameEmitters.begin(), frameEmitters.end(), SoundNode::CompareNodesByPriority);	//Then sort by priority

		DetachSources(frameEmitters.begin() + (sources.size()+1), frameEmitters.end());		//Detach sources from nodes that won't be covered this frame
		AttachSources(frameEmitters.begin(), frameEmitters.begin() + (sources.size()));	//And attach sources to nodes that WILL be covered this frame
	}
	else{
		AttachSources(frameEmitters.begin(), frameEmitters.end());//And attach sources to nodes that WILL be covered this frame
	}

	frameEmitters.clear();	//We're done for the frame! empty the emitters list
}

void SoundSystem::CullNodes() {
	for(vector<SoundNode*>::iterator i = emitters.begin(); i != emitters.end();) {

		float length;

		if((*i)->GetIsGlobal()) {
			length = 0.0f;
		}
		else{
			length = (listener->GetWorldTransform().GetPositionVector() - 
				(*i)->GetWorldTransform().GetPositionVector()).Length();
		}
		
		if(length > (*i)->GetRadius() || !(*i)->GetSound() || (*i)->GetTimeLeft() < 0) {
			(*i)->DetachSource();	//Important!
			i = emitters.erase(i);
		}
		else{
			++i;
		}
	}
}

void	SoundSystem::DetachSources(vector<SoundNode*>::iterator from, vector<SoundNode*>::iterator to) {
	for(vector<SoundNode*>::iterator i = from; i != to; ++i) {
		(*i)->DetachSource();
	}
}

void	SoundSystem::AttachSources(vector<SoundNode*>::iterator from, vector<SoundNode*>::iterator to) {
	for(vector<SoundNode*>::iterator i = from; i != to; ++i) {
		if(!(*i)->GetSource()) {	//Don't attach a new source if we already have one!
			(*i)->AttachSource(GetSource());
		}
	}
}

OALSource*	SoundSystem::GetSource() {
	for(vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i) {
		OALSource*s = *i;
		if(!s->inUse) {
			return s;
		}
	}
	return NULL;
}

void	SoundSystem::SetMasterVolume(float value)	{
	value = max(0.0f, value);
	value = min(1.0f, value);
	masterVolume = value;
	alListenerf(AL_GAIN,masterVolume);
}

void	SoundSystem::UpdateListener() {
	if(listener)	{
		Matrix4 worldMat = listener->GetWorldTransform();
		Vector3 worldPos = worldMat.GetPositionVector();

		Vector3 dirup[2];
		//forward
		dirup[0].x = -worldMat.values[2];
		dirup[0].y = -worldMat.values[6];
		dirup[0].z = -worldMat.values[10];
		//Up
		dirup[1].x = worldMat.values[1];
		dirup[1].y = worldMat.values[5];
		dirup[1].z = worldMat.values[9];

		alListenerfv(AL_POSITION,(float*)&worldPos);
		alListenerfv(AL_ORIENTATION,(float*)&dirup);
	}
}

void SoundSystem::PlaySound(Sound* s, Vector3 position) {
	SoundNode* n = new SoundNode();
	n->SetLooping(false);
	n->SetTransform(Matrix4::Translation(position));
	n->SetRadius(10000);
	n->SetSound(s);
	temporaryEmitters.push_back(n);
}

void SoundSystem::PlaySound(Sound * s, SoundPriority p) {
	SoundNode * n = new SoundNode();
	 n -> SetLooping(false);
	 n -> SetSound(s);
	 n -> SetIsGlobal(true);
	 n -> SetPriority(p);
	 temporaryEmitters.push_back(n);
	
}



void	SoundSystem::UpdateTemporaryEmitters(float msec) {
	for(vector<SoundNode*>::iterator i = temporaryEmitters.begin(); i != temporaryEmitters.end(); ) {
		if((*i)->GetTimeLeft() < 0.0f && !(*i)->GetLooping()) {
			delete (*i);
			i = temporaryEmitters.erase(i);
		}
		else{
			frameEmitters.push_back((*i));
			(*i)->Update(msec);
			++i;
		}
	}
}
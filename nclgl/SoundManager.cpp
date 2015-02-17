#include "SoundManager.h"
//#include "OggSound.h"

map<string, Sound*> SoundManager::sounds;

void SoundManager::AddSound(string name){
	if (!GetSound(name)){
		Sound *s = new Sound();

		string extension = name.substr(name.length() - 3, 3);

		if (extension == "wav") {
			s->LoadFromWAV(name);

			alGenBuffers(1, &s->buffer);

			alBufferData(s->buffer, s->GetOALFormat(), s->GetData(),
				s->GetSize(), (ALsizei)s->GetFrequency());
		}
		else if (extension == "ogg"){
			/*OggSound* ogg = new OggSound();
			ogg->LoadFromOgg(name);

			s = ogg;*/
		}
		else{
			s = NULL;
			cout << "Invalid extension '" << extension << "'!" << endl;
		}
		sounds.insert(make_pair(name, s));
	}
}

Sound* SoundManager::GetSound(string name){
	map<string, Sound*>::iterator s = sounds.find(name);
	return (s != sounds.end() ? s->second : NULL);
	return 0;
}

void SoundManager::DeleteSounds(){
	for (map<string, Sound*>::iterator i = sounds.begin();
		i != sounds.end(); i++){
		delete i->second;
	}
}
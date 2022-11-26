#include "SoundMgr.h"

SoundMgr::SoundMgr()
{
	soundName.push_back("sound/titleSound.wav"); // titleSound
	soundName.push_back("sound/playSound.wav"); // playSound
	soundName.push_back("sound/chop.wav");
	soundName.push_back("sound/death.wav");
	soundName.push_back("sound/out_of_time.wav");

	SoundBuffer soundBuffer;
	Sound sound;
	for (int i = 0; i < soundName.size(); i++)
	{
		soundBuffer.loadFromFile(soundName[i]);
		soundBuffers.push_back(soundBuffer);
	}

	for (int i = 0; i < soundBuffers.size(); i++)
	{
		sound.setBuffer(soundBuffers[i]);
		sounds.push_back(sound);
	}	
}

SoundMgr::~SoundMgr()
{
	for (auto& k : soundBuffers)
	{
		delete &k;
	}
	soundBuffers.clear();

	for (auto& k : sounds)
	{
		delete& k;
	}
	sounds.clear();

	for (auto& k : soundName)
	{
		delete& k;
	}
	soundName.clear();
}

void SoundMgr::SoundPlay(SoundChoice soundchoice)
{
	switch (soundchoice)
	{
	case SoundChoice::TitleSound:
		sounds[0].play();
		break;

	case SoundChoice::PlaySound:
		sounds[1].play();
		break;

	case SoundChoice::ChopSound:
		sounds[1].play();
		break;

	case SoundChoice::DeathSound:
		sounds[2].play();
		break;

	case SoundChoice::TimeOutSound:
		sounds[3].play();
		break;
	}
	this->soundChoice = soundchoice;
}

SoundChoice SoundMgr::GetSoundChoice()
{
	return soundChoice;
}
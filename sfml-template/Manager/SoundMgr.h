#pragma once
#include <SFML/Audio.hpp>
#include <list>
using namespace std;
using namespace sf;

enum class SoundChoice
{
	TitleSound,
	PlaySound,
	ChopSound,
	DeathSound,
	TimeOutSound,
};

class SoundMgr 
{
private:
	vector<SoundBuffer> soundBuffers;
	vector<Sound> sounds;
	vector<string> soundName;

	SoundChoice soundChoice;
public:
	SoundMgr();
	virtual ~SoundMgr();
	
	void SoundPlay(SoundChoice soundchoice);
	SoundChoice GetSoundChoice();
};


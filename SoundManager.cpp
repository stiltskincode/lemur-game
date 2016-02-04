#include "SoundManager.h"

Sound SoundManager::s;
std::string SoundManager::sNames[NUM_SOURCES];

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::init()
{
	s.init();
}

void SoundManager::addSound(std::string fileName,ALuint sId)
{
	s.load(fileName,sId);
	sNames[sId] = "NoName"+sId;
}

void SoundManager::addSound(std::string fileName,std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].size() == 0)
		{
			s.load(fileName,i);
			sNames[i]=sName;
			break;
		}
	}
}

void SoundManager::removeSound(ALuint sId)
{
	s.remove(sId);
	sNames[sId].clear();
}

void SoundManager::removeSound(std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			s.remove(i);
			sNames[i].clear();
			break;
		}
	}
}

void SoundManager::removeAll()
{
	s.removeAll();
	for(int i=0;i<NUM_SOURCES;i++)
		sNames[i].clear();
}

void SoundManager::replaceSound(std::string sName, std::string sNewName, std::string sNewFileName)
{
	removeSound(sName);
	addSound(sNewFileName,sNewName);
}

void SoundManager::replaceSound(std::string sName, std::string sNewFileName)
{
	removeSound(sName);
	addSound(sNewFileName,sName);
}

void SoundManager::replaceSound(ALuint sId, std::string sNewFileName)
{
	removeSound(sId);
	addSound(sNewFileName,sId);
}

void SoundManager::replaceSound(ALuint sId, std::string sNewName, std::string sNewFileName)
{
	removeSound(sId);
	addSound(sNewFileName,sNewName);
}

void SoundManager::play(ALuint sId)
{
	s.play(sId);
}

void SoundManager::play(std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			s.play(i);
			break;
		}
	}
}

void SoundManager::playAll()
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.play(i);
}

bool SoundManager::isPlaying(std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			return s.isPlaying(i);
		}
	}
	return false;
}

bool SoundManager::isPlaying(ALuint sId)
{
	return s.isPlaying(sId);
}

void SoundManager::pause(ALuint sId)
{
	s.pause(sId);
}

void SoundManager::pause(std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			s.pause(i);
			break;
		}
	}
}

void SoundManager::pauseAll()
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.pause(i);
}

void SoundManager::stop(ALuint sId)
{
	s.stop(sId);
}

void SoundManager::stop(std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			s.stop(i);
			break;
		}
	}
}

void SoundManager::stopAll()
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.stop(i);
}

void SoundManager::setLoop(bool value, ALuint sId)
{
	s.setLoop(value, sId);
}

void SoundManager::setLoop(bool value, std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			s.setLoop(value, i);
			break;
		}
	}
}

void SoundManager::setLoopAll(bool value)
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.setLoop(value, i);
}

void SoundManager::setVolume(ALfloat value, ALuint sId)
{
	s.setVolume(value, sId);
}

void SoundManager::setVolume(ALfloat value, std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			s.setVolume(value, i);
			break;
		}
	}
}

void SoundManager::setVolumeAll(ALfloat value)
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.setVolume(value, i);
}

void SoundManager::setListenerValues(ALfloat vPos[3],ALfloat vFront[3], ALfloat vUp[3])
{
	s.setListenerValues(vPos,vUp,vFront);
}

void SoundManager::setEmitterValues(ALfloat vPos[3],ALfloat vFront[3] ,ALuint sId)
{
	s.setEmitterValues(vPos,vFront,sId);
}

void SoundManager::setEmitterValues(ALfloat vPos[3],ALfloat vFront[3], std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			s.setEmitterValues(vPos,vFront,i);
			break;
		}
	}
}





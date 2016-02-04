#include "SoundSet.h"

SoundSet::SoundSet()
{
}

SoundSet::~SoundSet()
{
}

void SoundSet::init()
{
	s.init();
}

void SoundSet::addSound(std::string fileName,ALuint sId)
{
	s.load(fileName,sId);
	sNames[sId] = "NoName"+sId;
}

void SoundSet::addSound(std::string fileName,std::string sName)
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

void SoundSet::removeSound(ALuint sId)
{
	s.remove(sId);
	sNames[sId].clear();
}

void SoundSet::removeSound(std::string sName)
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

void SoundSet::removeAll()
{
	s.removeAll();
	for(int i=0;i<NUM_SOURCES;i++)
		sNames[i].clear();
}

void SoundSet::replaceSound(std::string sName, std::string sNewName, std::string sNewFileName)
{
	removeSound(sName);
	addSound(sNewFileName,sNewName);
}

void SoundSet::replaceSound(std::string sName, std::string sNewFileName)
{
	removeSound(sName);
	addSound(sNewFileName,sName);
}

void SoundSet::replaceSound(ALuint sId, std::string sNewFileName)
{
	removeSound(sId);
	addSound(sNewFileName,sId);
}

void SoundSet::replaceSound(ALuint sId, std::string sNewName, std::string sNewFileName)
{
	removeSound(sId);
	addSound(sNewFileName,sNewName);
}

void SoundSet::play(ALuint sId)
{
	s.play(sId);
}

void SoundSet::play(std::string sName)
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

void SoundSet::playAll()
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.play(i);
}

bool SoundSet::isPlaying(std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			return s.isPlaying(i);
			break;
		}
	}
}

bool SoundSet::isPlaying(ALuint sId)
{
	return s.isPlaying(sId);
}

void SoundSet::pause(ALuint sId)
{
	s.pause(sId);
}

void SoundSet::pause(std::string sName)
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

void SoundSet::pauseAll()
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.pause(i);
}

void SoundSet::stop(ALuint sId)
{
	s.stop(sId);
}

void SoundSet::stop(std::string sName)
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

void SoundSet::stopAll()
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.stop(i);
}

void SoundSet::setLoop(bool value, ALuint sId)
{
	s.setLoop(value, sId);
}

void SoundSet::setLoop(bool value, std::string sName)
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

void SoundSet::setLoopAll(bool value)
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.setLoop(value, i);
}

void SoundSet::setVolume(ALfloat value, ALuint sId)
{
	s.setVolume(value, sId);
}

void SoundSet::setVolume(ALfloat value, std::string sName)
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

void SoundSet::setVolumeAll(ALfloat value)
{
	for(int i=0;i<NUM_SOURCES;i++)
		s.setVolume(value, i);
}

ALfloat SoundSet::getVolume(std::string sName)
{
	for(int i=0;i<NUM_SOURCES;i++)
	{
		if(sNames[i].compare(sName.c_str()) == 0)
		{
			return s.getVolume(i);
		}
	}
	return 0;
}

ALfloat SoundSet::getVolume(ALuint sId)
{
	return s.getVolume(sId);
}

void SoundSet::setListenerValues(ALfloat vPos[3],ALfloat vFront[3], ALfloat vUp[3])
{
	s.setListenerValues(vPos,vUp,vFront);
}

void SoundSet::setEmitterValues(ALfloat vPos[3],ALfloat vFront[3] ,ALuint sId)
{
	s.setEmitterValues(vPos,vFront,sId);
}

void SoundSet::setEmitterValues(ALfloat vPos[3],ALfloat vFront[3], std::string sName)
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





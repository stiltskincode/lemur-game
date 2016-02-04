#include "Sound.h"
#include <algorithm>

Sound::Sound()
{
	endian = 0;
}

Sound::~Sound()
{
	alDeleteBuffers(NUM_BUFFERS, buffers);
	alDeleteSources(NUM_SOURCES, sources);
	alutExit();
}

void Sound::init()
{
	alutInit(NULL, 0);

	ss.str("");
	ss << getMaxSources();
	message = "Wygenerowano " + ss.str() + " Ÿróde³ i bufferów";
	writeInf(message);
	alGenBuffers(getMaxSources(), buffers);
	alGenSources(getMaxSources(), sources);

}

bool Sound::load(std::string fileName, ALuint sID)
{
	//int ifGenerated = 0;
	std::string ext;
	//for(int i=0;i<NUM_SOURCES;i++)
	//	if(sources[i] != NULL) ifGenerated++;

	//if(ifGenerated == 0)
	//{
	//
	//}

	if(alGetError() != AL_NO_ERROR)
	{
		writeErr(alutGetErrorString(alutGetError()));
		return AL_FALSE;
	}

	// Zmienia ciag znaków na ma³e litery.
	std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);

	ext=fileName.substr(fileName.length()-4,4);


	if(ext.compare(".wav") == 0)
	{
		file = fopen(fileName.c_str(), "rb");
		if(!file)
		{
			message = "PLIK WAV: Nie mozna odnaleœæ pliku " + fileName + " lub plik jest aktualnie u¿ywany!";
			writeErr(message);
			return AL_FALSE;
		}

		alutLoadWAVFile((ALbyte*)fileName.c_str(), &format, &data, &size, &freq, &loop);
		alBufferData(buffers[sID], format, data, size, freq);
		alutUnloadWAV(format, data, size, freq);

		message = "PLIK WAV: Plik " + fileName + " za³adowany z powodzeniem.";
		writeInf(message.c_str());

		fclose(file);
	}
	else if(ext.compare(".ogg") == 0)
	{
		oggFile = fopen(fileName.c_str(), "rb");
		if(!oggFile)
		{
			message = "PLIK OGG: Nie mozna odnaleœæ pliku " + fileName + " lub plik jest aktualnie u¿ywany!";
			writeErr(message);
			return AL_FALSE;
		}

		int result;

		if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
		{
			message = "PLIK OGG: Plik " + fileName + " wydaje siê byæ uszkodzony lub nie jest to plik Ogg!";
			writeErr(message);
			return AL_FALSE;
		}
		vorbisInfo = ov_info(&oggStream, -1);
		vorbisComment = ov_comment(&oggStream, -1);

		if(vorbisInfo->channels == 1)
			oggFormat = AL_FORMAT_MONO16;
		else
		    oggFormat = AL_FORMAT_STEREO16;

		do
		{
			bytes = ov_read(&oggStream, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

			if (bytes < 0)
			{
				ov_clear(&oggStream);
				message = "PLIK OGG: B³¹d dekodowania pliku " + fileName;
				writeErr(message);
				return AL_FALSE;
			}
			oggBuffer[sID].insert(oggBuffer[sID].end(), array, array + bytes);
		}
		while (bytes > 0);
			ov_clear(&oggStream);

		alBufferData(buffers[sID], oggFormat, &oggBuffer[sID][0], static_cast<ALsizei>(oggBuffer[sID].size()), vorbisInfo->rate);
		fclose(oggFile);

		message = "PLIK OGG: Plik " + fileName + " za³adowany z powodzeniem.";
		writeInf(message.c_str());
	}
	else
	{
		file = fopen(fileName.c_str(), "rb");
		if(!file)
		{
			message = "Nie mozna odnaleœæ pliku " + fileName + " lub plik jest aktualnie u¿ywany!";
			writeErr(message);
			return AL_FALSE;
		}
		else
		{
			message = "Podane rozszerzenie *" + ext + " jest nie obslugiwane!";
			writeWar(message);
		}
	}

	//if(ifGenerated == 0)


	if(alGetError() != AL_NO_ERROR)
	{
		writeErr(alutGetErrorString(alutGetError()));
		return AL_FALSE;
	}

	ALfloat sPos[] = { 0.0f, -10.0f, 0.0f };
	ALfloat sVel[] = { 0.0f, 0.0f, 0.0f };

	alSourcei (sources[sID], AL_BUFFER, buffers[sID]);

	if(alGetError() != AL_NO_ERROR)
	{
		writeErr(alutGetErrorString(alutGetError()));
		return AL_FALSE;
	}

	alSourcefv (sources[sID], AL_POSITION, sPos);
	alSourcefv (sources[sID], AL_VELOCITY, sVel);
    alSource3f(sources[sID], AL_DIRECTION, 0.0, 0.0, 0.0);
    alSourcef (sources[sID], AL_ROLLOFF_FACTOR, 0.0);
	alSourcei (sources[sID], AL_LOOPING,  AL_FALSE);

	if(alGetError() != AL_NO_ERROR)
	{
		writeErr(alutGetErrorString(alutGetError()));
		return AL_FALSE;
	}

	return AL_TRUE;
}

void Sound::remove(ALuint sID)
{
	alDeleteSources(1,&sources[sID]);
	if(oggBuffer[sID].size() != 0) oggBuffer[sID].clear();
	alGenSources(1, &sources[sID]);
	alDeleteBuffers(1,&buffers[sID]);
	alGenBuffers(1,&buffers[sID]);
	std::cout << "usunieto: " << sID << std::endl;
}

void Sound::removeAll()
{
	alDeleteSources(NUM_SOURCES,sources);
	alDeleteBuffers(NUM_BUFFERS,buffers);
}

void Sound::play(ALuint sID)
{
	//if(!isPlaying(sID))
		alSourcePlay(sources[sID]);
}

void Sound::pause(ALuint sID)
{
	if(isPlaying(sID))
		alSourcePause(sources[sID]);
}

void Sound::stop(ALuint sID)
{
	if(isPlaying(sID))
		alSourceStop(sources[sID]);
}

bool Sound::isPlaying(ALuint sID)
{
	ALenum state;

	alGetSourcei(sources[sID], AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}

void Sound::setListenerValues(ALfloat vPos[3],ALfloat vTop[3], ALfloat vFront[3])
{
	float 			velocity[3];
	static float 	lastpos[3] = { 0,0,0 };

	velocity[0] = (vPos[0]-lastpos[0]) * (1000 / 50);
	velocity[1] = (vPos[1]-lastpos[1]) * (1000 / 50);
	velocity[2] = (vPos[2]-lastpos[2]) * (1000 / 50);

	lastpos[0] = vPos[0];
	lastpos[1] = vPos[1];
	lastpos[2] = vPos[2];

	ALfloat vOri[] = { vFront[0], vFront[1], vFront[2],vTop[0], vTop[1],vTop[2] };

	alListenerfv (AL_POSITION, vPos);
	//alListenerfv (AL_VELOCITY, velocity);
	alListenerfv (AL_ORIENTATION, vOri);
}

void Sound::setEmitterValues(ALfloat vPos[3],ALfloat vFront[3], ALuint sID)
{
	float 			velocity[3]; 				// Predkosc
	static float 	lastpos[3] = { 0,0,0 }; 	// Przesuniecie

	velocity[0] = (vPos[0]-lastpos[0]) * (1000 / 50);
	velocity[1] = (vPos[1]-lastpos[1]) * (1000 / 50); 	// Przeliczanie predkosci
	velocity[2] = (vPos[2]-lastpos[2]) * (1000 / 50);

	// store pos for next time
	lastpos[0] = vPos[0];
	lastpos[1] = vPos[1]; // Przeliczanie przesuniecia
	lastpos[2] = vPos[2];

	alSourcefv (sources[sID], AL_POSITION, vPos);
	//alSourcefv (sources[sID], AL_VELOCITY, velocity);
    alSourcefv (sources[sID], AL_DIRECTION, vFront);
}

void Sound::setVolume(ALfloat value, ALuint sID)
{
	alSourcef (sources[sID], AL_GAIN, value);
}

ALfloat Sound::getVolume(ALuint sID)
{
	ALfloat value=0.0f;
	//alGetSourcef (sources[sID], AL_GAIN, value);
	return value;
}

void Sound::setLoop(bool value, ALuint sID)
{
	if(value==true)
		alSourcei (sources[sID], AL_LOOPING,  AL_TRUE);
	if(value==false)
		alSourcei (sources[sID], AL_LOOPING,  AL_FALSE);
}

ALuint Sound::getMaxSources()
{
	ALuint Source;
	std::vector<ALuint> Sources;

	while (alGetError() == AL_NO_ERROR)
	{
		Source = 0;
		alGenSources(1, &Source);
		if (Source != 0)
			Sources.push_back(Source);
	}

	int nSources = Sources.size();

	for(std::vector<ALuint>::iterator it = Sources.begin(); it != Sources.end(); it++)
	{
		Source = (*it);
		alDeleteSources(1, &Source);
	}
	return nSources;
}


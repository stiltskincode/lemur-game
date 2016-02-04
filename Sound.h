#ifndef SOUND_H_
#define SOUND_H_

#include <al.h>
#include <alc.h>
#include <AL/alut.h>
#include <OGG/ogg.h>
#include <Vorbis/vorbisfile.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#define WRITE_ALL_LOGS
#include "LogWriter.h"

// Zdefiniowana stala okreslajaca liczbe bufferow, ktore beda wymagane
#define NUM_BUFFERS 256

// Zdefiniowana stala okreslajaca liczbe zrodel, ktore beda wymagane
#define NUM_SOURCES 256

#define BUFFER_SIZE (4096 * 4)


class Sound
{
	public:
		Sound();
		virtual ~Sound();

		// Inizjalizuje ALUT
		void init();

		// Dodawanie i usuwanie wskaznikow oraz bufferow dla dzwieku
		bool load(std::string fileName, ALuint sID);
		void remove(ALuint sID);
		void removeAll();

		// Metody sterujace
		void play(ALuint sID);
		void pause(ALuint sID);
		void stop(ALuint sID);

		// Zwraca stan odgrywania dzwieku
		bool isPlaying(ALuint sID);

		// Ustawia tryb zapetlenia dzwieku
		void setLoop(bool value, ALuint sID);

		// Ustawienia koordynat dla "sluchacza" i "emitera"
		void setListenerValues(ALfloat vPos[3],ALfloat vFront[3], ALfloat vUp[3]);
		void setEmitterValues(ALfloat vPos[3],ALfloat vFront[3], ALuint sID);

		// Ustawienia glosnosci
		void setVolume(ALfloat value, ALuint sID);

		// Zwraca poziom g³oœnoœci.
		ALfloat getVolume(ALuint sID);

	private:

		ALuint getMaxSources();

		std::string message;

		// Zmienne pomocniecze ustalaj¹ce maksymaln¹ liczbe Ÿróde³ i bufferów.
		ALuint numBuffers;
		ALuint numSources;
		std::stringstream ss;

		// Zmienna pomocnicza okreœlaj¹ca instenienie danego pliku.
		FILE* file;

		// Deklaracja vektora buferow dla dzwiekow, buffery przechowuja dane o dzwieku
		ALuint buffers[NUM_BUFFERS];
		// Deklaracja vektora wskaznikow dla dzwiekow, zrodla sa wskaznikiem do emitowanego dzwieku
		ALuint sources[NUM_SOURCES];

		// Pola przechowywujacwe informacje o dzwieku WAV
		ALenum format;
		ALsizei size;
		ALvoid* data;
		ALsizei freq;
		ALboolean loop;

		// Pola przechowywujace informacje o dzwieku OGG
		ALenum 			oggFormat;
		FILE*           oggFile;
		OggVorbis_File  oggStream;
		vorbis_info*    vorbisInfo;
		vorbis_comment* vorbisComment;

        // Buffer dla dzwieku Ogg
		std::vector<char> oggBuffer[NUM_BUFFERS];

		std::vector<char> wavBuffer;

        // 0 for Little-Endian, 1 for Big-Endian
        int endian;

        int bitStream;
        long bytes;

        // Local fixed size array
        char array[BUFFER_SIZE];
};

#endif /*SOUND_H_*/

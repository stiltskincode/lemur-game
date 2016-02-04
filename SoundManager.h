#ifndef SOUNDSET_H_
#define SOUNDSET_H_

#include "Sound.h"

	///////////////////////////////////////////
	//	  KLASA DO ZARZ�DZANIA D�WI�KIEM    //
	///////////////////////////////////////////

class SoundManager
{
	public:
		virtual ~SoundManager();

		static void init();

		// �aduje d�wi�k do pami�ci i dodaje go do listy.
		static void addSound(std::string fileName, ALuint sId);
		static void addSound(std::string fileName, std::string sName);

		// Usuwa d�wi�k z pami�ci i listy oraz czy�ci buffer.
		static void removeSound(ALuint sId);
		static void removeSound(std::string sName);
		static void removeAll();

		// Podmienia za�adowany d�wiek na inny, wczytany z nowego pliku.
		static void replaceSound(std::string sName, std::string sNewName, std::string sNewFileName);
		static void replaceSound(std::string sName, std::string sNewFileName);
		static void replaceSound(ALuint sId, std::string sNewFileName);
		static void replaceSound(ALuint sId, std::string sNewName ,std::string sNewFileName);

		// Odgrywa d�wiek.
		static void play(std::string sName);
		static void play(ALuint sId);
		static void playAll();

		// Zwraca stan odgrywania d�wieku.
		static bool isPlaying(std::string sName);
		static bool isPlaying(ALuint sId);

		// Zatrzymuje d�wiek.
		static void stop(std::string sName);
		static void stop(ALuint sId);
		static void stopAll();

		// Pauza d�wi�ku.
		static void pause(std::string sName);
		static void pause(ALuint sId);
		static void pauseAll();

		// Ustawia atrybut zap�tlenia d�wi�ku.
		static void setLoop(bool value, std::string sName);
		static void setLoop(bool value, ALuint sId);
		static void setLoopAll(bool value);

		// Ustawia g�o�no�� d�wi�ku (warto�ci 0.0f - 1.0f).
		static void setVolume(ALfloat value, std::string sName);
		static void setVolume(ALfloat value, ALuint sId);
		static void setVolumeAll(ALfloat value);

		// Zwraca poziom g�o�no�ci.
		static ALfloat getVolume(std::string sName);
		static ALfloat getVolume(ALuint sId);

		// Ustawia koordynaty "s�uchacza" w przestrzeni 3D.
		static void setListenerValues(ALfloat vPos[3],ALfloat vFront[3], ALfloat vUp[3]);

		// Ustawia koordynaty "emitera" w przestrzeni 3D.
		static void setEmitterValues(ALfloat vPos[3],ALfloat vFront[3], ALuint sId);
		static void setEmitterValues(ALfloat vPos[3],ALfloat vFront[3], std::string sName);

	private:
		SoundManager(); // Konstruktor Prywatny
		// Tablica zawieraj�ca identywikatory d�wi�k�w.
		static std::string sNames[NUM_SOURCES];
		// Obiekt klasy Sound, g��wnej klasy odtwarzaj�cej d�wi�k Wav i Ogg.
		static Sound s;
};

#endif /*SOUNDSET_H_*/

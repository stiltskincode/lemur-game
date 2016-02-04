#ifndef SOUNDSET_H_
#define SOUNDSET_H_

#include "Sound.h"

	///////////////////////////////////////////
	//	  KLASA DO ZARZ•DZANIA DèWI KIEM    //
	///////////////////////////////////////////

class SoundSet
{
	public:
		SoundSet();
		virtual ~SoundSet();
		
		void init();
		
		// £aduje düwiÍk do pamiÍci i dodaje go do listy.
		void addSound(std::string fileName, ALuint sId);
		void addSound(std::string fileName, std::string sName);
		
		// Usuwa døwiÍk z pamiÍci i listy oraz czyúci buffer.
		void removeSound(ALuint sId);
		void removeSound(std::string sName);
		void removeAll();
		
		// Podmienia za≥adowany düwiek na inny, wczytany z nowego pliku.
		void replaceSound(std::string sName, std::string sNewName, std::string sNewFileName);
		void replaceSound(std::string sName, std::string sNewFileName);
		void replaceSound(ALuint sId, std::string sNewFileName);
		void replaceSound(ALuint sId, std::string sNewName ,std::string sNewFileName);
		
		// Odgrywa düwiek.
		void play(std::string sName);
		void play(ALuint sId);
		void playAll();
		
		// Zwraca stan odgrywania düwieku.
		bool isPlaying(std::string sName);
		bool isPlaying(ALuint sId);
		
		// Zatrzymuje düwiek.
		void stop(std::string sName);
		void stop(ALuint sId);
		void stopAll();
		
		// Pauza düwiÍku.
		void pause(std::string sName);
		void pause(ALuint sId);
		void pauseAll();

		// Ustawia atrybut zapÍtlenia düwiÍku.
		void setLoop(bool value, std::string sName);
		void setLoop(bool value, ALuint sId);
		void setLoopAll(bool value);
		
		// Ustawia g≥oúnoúÊ düwiÍku (wartoúci 0.0f - 1.0f).
		void setVolume(ALfloat value, std::string sName);
		void setVolume(ALfloat value, ALuint sId);
		void setVolumeAll(ALfloat value);
		
		// Zwraca poziom g≥oúnoúci.
		ALfloat getVolume(std::string sName);
		ALfloat getVolume(ALuint sId);
		
		// Ustawia koordynaty "s≥uchacza" w przestrzeni 3D.
		void setListenerValues(ALfloat vPos[3],ALfloat vFront[3], ALfloat vUp[3]);
		
		// Ustawia koordynaty "emitera" w przestrzeni 3D. 
		void setEmitterValues(ALfloat vPos[3],ALfloat vFront[3], ALuint sId);
		void setEmitterValues(ALfloat vPos[3],ALfloat vFront[3], std::string sName);
		
	private:
		// Tablica zawierajπca identywikatory düwiÍkÛw.
		std::string sNames[NUM_SOURCES];
		// Obiekt klasy Sound, g≥Ûwnej klasy odtwarzajπcej düwiÍk Wav i Ogg.
		Sound s;
};

#endif /*SOUNDSET_H_*/

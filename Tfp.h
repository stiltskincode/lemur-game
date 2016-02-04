#ifndef TFP_H_
#define TFP_H_

#define WRITE_ALL_LOGS

#include <fstream>
#include <iostream>
#include <stdio.h>

#include "LogWriter.h" 

#define NUM_NAMES 255

class Tfp
{
	public:
		Tfp();
		
		// Otwiera archiwum.
		Tfp(std::string archFileName);
		
		// Ustawia folder, gdzie maj¹ zostaæ wypakowane pliki oraz otwiera archiwum.		
		Tfp(std::string archFileName, std::string path);
		
		virtual ~Tfp();
		
		// Tworzy nowe archuwum TFP.
		bool compress(std::string archFileName, std::string *fileNames, unsigned int num);
		
		// Ustawia folder, gdzie maj¹ zostaæ wypakowane pliki.
		void setExtractPath(std::string path);
		
		// Pobiera lokalizacje, gdzie zosta³y wypakowane pliki.
		std::string getExtractPath();
		
		// Open - otwiera archuwum | Close - zamyka archuwum.
		bool open(std::string archFileName);
		bool close();
		
		// Wypakowuje pliki na dysk
		bool decompress(unsigned int id);		
		bool decompress(std::string fileName);		
		
		// Zwraca wskaŸnik przez referencje do pliku umieszczonego w archiwum. 
		std::fstream &getFile(std::string fileName);
		std::fstream &getFile(unsigned int id);
			
		// Zwraca wielkoœæ pliku.
		unsigned int getFileSize(std::string fileName);
		unsigned int getFileSize(unsigned int id);
		
		unsigned int getFileId(std::string fileName);
		
		void deteleDecompressedFiles();
		
		std::string *getFileNamesFromTfp(std::string fileName);
		
	private:
		
		std::string names[NUM_NAMES];
		std::string tempNames[NUM_NAMES];
		std::string path;
		unsigned int fileSizes[NUM_NAMES];
		unsigned int numFiles;
		std::fstream file;
		unsigned int headerSize;
};

#endif /*TFP_H_*/

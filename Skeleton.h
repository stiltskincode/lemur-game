/**********************************************************************
 Skeleton
 
 Klasa reprezentuje hierarchie kosci, czyli szkielet. Idea tej klasy
 jest przechowywanie wszystkich macierzy w buforach tak, aby mozliwe
 bylo przekazanie ich do shader'a. Wewnacz klasy przyjeto nastepujace
 skroty przy nazywaniu pol:
 tbuf - Transformation Matrix Buffer
 cbuf - Combined Transformation Matrix Buffer 
 obuf - Offset Matrix Buffer
 Przedrostek 'w' przed nazwa pola oznacza 'wrapper', czyli obiekt
 opakowujacy. W przypadku tej klasy obiektami opakowujacymi sa klasy
 Mat4. Kazdy obiekt tablicy wtbuf reprezentuje 16 liczb typu float
 bufora tbuf. Dzieki takiemu zabiegowi, mozliwe jest przeprowadzanie
 operacji macierzowych na zwyklych tablicach float tak, jak gdyby
 byly to obiekty klasy Mat4. Bufory opakowujace, to:
 wtbuf - Transformation Matrix Buffer Wrapper
 wcbuf - Combined Transformation Matrix Buffer Wrapper
 wobuf - Offset Matrix Buffer Wrapper
**********************************************************************/

#ifndef SKELETON_H_
#define SKELETON_H_

#define WRITE_ALL_LOGS

#include <string>
#include <fstream>
#include <vector>
#include "Mathematics.h"
#include "LogWriter.h"

#include <iostream>

class Skeleton
{
	public:
		
		struct Bone
		{
			unsigned int id;
			std::string name;
			std::vector<Bone> children;
		};

		Skeleton();
		virtual ~Skeleton();
		bool load(std::string fileName);
		bool load(std::fstream &file);
		Bone* find(std::string name);
		Bone* find(unsigned int id);
		void update(Bone *bone, Mat4 &m);
		void update();
		void update(std::string name);
		void update(unsigned int id);
		void update(Bone *bone);
		void updateAttributeMatrices();

	//private:
		float *tbuf;		//Bufor macierzy transformacji.
		float *cbuf;		//Bufor macierzy transformacji lacznych. 
		float *obuf;		//Bufor macierz offsetowych.
		float *abuf;		//Bufor macierzy atrybutow (offset*transform)
		Mat4  *sbuf;			//Bufor macierzy zrodlowych, niezmodyfikowanych.
		unsigned int num;	//Ilosc elementow (kosci, macierzy).
		std::vector<Bone> bones;	//Hierarchia kosci.
		
		//Bufory opakowujace (wrappers).
		Mat4 **wtbuf;
		Mat4 **wcbuf;
		Mat4 **wobuf;
		Mat4 **wabuf;
		Bone **wbones;

		void readBone(Bone &bone, std::fstream &file);
		void createBonesWrapper(vector<Bone> &bones);
		//Bone* findBone(std::string name, Bone *bone);
		//Bone* findBone(std::string name, Bone *bone);
};

#endif /*SKELETON_H_*/

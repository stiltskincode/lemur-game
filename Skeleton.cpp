#include "Skeleton.h"

#include <iostream>

using namespace std;

unsigned int index=0;

Skeleton::Skeleton()
{
	num=0;
	tbuf=cbuf=obuf=abuf=NULL;
	sbuf=NULL;
	wtbuf=wcbuf=wobuf=wabuf=NULL;
	wbones=NULL;
}

Skeleton::~Skeleton()
{
	//Usuniecie buforow glownych.
	if(tbuf) delete[] tbuf;
	if(cbuf) delete[] cbuf;
	if(obuf) delete[] obuf;
	if(abuf) delete[] abuf;
	if(sbuf) delete[] sbuf;
	
	//Usuniecie buforow opakowujacych.
	for(unsigned int i=0; i<num; i++)
	{
		wtbuf[i]=NULL;
		wcbuf[i]=NULL;
		wobuf[i]=NULL;
		wabuf[i]=NULL;
		wbones[i]=NULL;
	}
	
	//Usuniecie buforow opakowujacych (tablice wskaznikow).
	if(wtbuf) delete[] wtbuf;
	if(wcbuf) delete[] wcbuf;
	if(wobuf) delete[] wobuf;
	if(wabuf) delete[] wabuf;
	if(wbones) delete[] wbones;
}

/**********************************************************************
 Wczytuje hierarchie kosci z pliku o podanej nazwie. Metoda ta
 jedynie tworzy obiekt pliku, ktory jest potem przekazywany do
 metody: load(std::fstream &file), ktora zajmuje sie wlasciwym
 odczytaniem hierarchii. 
**********************************************************************/
bool Skeleton::load(std::string fileName)
{
	fstream file(fileName.c_str(),ios::binary|ios::in);
	
	//Sprawdzenie, czy plik zostal otwarty.
	if(!file.is_open())
	{
		writeErr("Nie mozna otworzyc pliku: "+fileName);
		return false;
	}
	
	return load(file);
}

/**********************************************************************
 Wczytuje hierarchie kosci z pliku reprezentowanego przez dany jako
 argument obiekt typu fstream. Poza stworzeniem i alokacja buforow,
 rozpoczyna ona ciag rekurencyjnych wywolan metody readBone.
**********************************************************************/
bool Skeleton::load(std::fstream &file)
{
	unsigned int n=0;
	
	file.read((char*)&num,sizeof(int));	//Odczytanie ilosci elementow.

	//Alokacja pamieci na bufory podstawowe.
	tbuf = new float[num*16];
	cbuf = new float[num*16];
	obuf = new float[num*16];
	abuf = new float[num*16];
	sbuf = new Mat4[num];
	
	//Alokacja pamieci na bufory opakowujace.
	wtbuf = new Mat4*[num];
	wcbuf = new Mat4*[num];
	wobuf = new Mat4*[num];
	wabuf = new Mat4*[num];
	wbones = new Bone*[num];
	
	//Inicjalizacja buforow opakowujacych.
	for(unsigned int i=0; i<num; i++)
	{
		wtbuf[i]=new Mat4(&tbuf[16*i]);
		wcbuf[i]=new Mat4(&cbuf[16*i]);
		wobuf[i]=new Mat4(&obuf[16*i]);
		wabuf[i]=new Mat4(&abuf[16*i]);
	}

	file.read((char*)&n,sizeof(int));	//Odczytanie ilosci kosci glownych.
	for(unsigned int i=0; i<n; i++)
	{
		bones.push_back(Bone());
		readBone(bones.back(),file);
	}

	//Stworzenie bufora opakowujacego dla kosci.
	createBonesWrapper(bones);
	
	//Odczytanie macierzy transformacji i macierzy offsetowych.
	file.read((char*)tbuf,sizeof(float)*num*16);
	file.read((char*)obuf,sizeof(float)*num*16);

	update();

	//Inicjalizacja bufora macierzy polaczonych transformacji.
	//for(unsigned int i=0; i<num; i++) *wcbuf[i]=*wtbuf[i];
	for(unsigned int i=0; i<num; i++)
	{
		*wcbuf[i]=*wtbuf[i];
		//*wobuf[i]=*wcbuf[i];
		//wobuf[i]->inverse();
	}

	index=0;	//!!!

	for(unsigned int i=0; i<num; i++)
	{
		sbuf[i]=*wtbuf[i];
	}

	return true;
}

/**********************************************************************
 Wyszukuje kosc, na podstawie podanej nazwy.
**********************************************************************/
Skeleton::Bone* Skeleton::find(std::string name)
{
	for(unsigned int i=0; i<num; i++)
	{
		if(wbones[i]->name==name) return wbones[i];
	}
	return NULL;
}

/**********************************************************************
 Wyszukuje kosc, na podstawie podanego id.
**********************************************************************/
Skeleton::Bone* Skeleton::find(unsigned int id)
{
	for(unsigned int i=0; i<num; i++)
	{
		if(wbones[i]->id==id) return wbones[i];
	}
	return NULL;
}

void Skeleton::update(Bone *bone, Mat4 &m)
{
	(*wcbuf[bone->id])=(*wtbuf[bone->id])*m;
	for(unsigned int i=0; i<bone->children.size(); i++)
	{
		update(&bone->children[i],(*wcbuf[bone->id]));
	}
}

void Skeleton::update()
{
	for(unsigned int i=0; i<bones.size(); i++)
	{
		*wcbuf[bones[i].id] = *wtbuf[bones[i].id];
		update(&bones[i]);
	}
}

void Skeleton::update(std::string name)
{
	Bone *bone=find(name);
	if(!bone) return;
	update(bone);
}

void Skeleton::update(unsigned int id)
{
	Bone *bone=find(id);
	if(!bone) return;
	update(bone);
}

void Skeleton::update(Bone *bone)
{
	if(!bone) return;

	for(unsigned int i=0; i<bone->children.size(); i++)
	{
		unsigned int id = bone->children[i].id;
		(*wcbuf[id])=(*wtbuf[id])*(*wcbuf[bone->id]);
		update(&bone->children[i]);
	}
}

void Skeleton::updateAttributeMatrices()
{
	for(unsigned int i=0; i<num; i++)
		(*wabuf[i])=(*wobuf[i])*(*wcbuf[i]);
}

//Bone& update(unsigned int id);

/**********************************************************************
 METODA PRYWATNA
 Tworzy bufor opakowujacy dla kosci.
**********************************************************************/
void Skeleton::createBonesWrapper(vector<Bone> &bones)
{
	for(unsigned int i=0; i<bones.size(); i++)
	{
		wbones[index++]=&bones[i];
		createBonesWrapper(bones[i].children);
	}
}

/**********************************************************************
 METODA PRYWATNA
 Wczytuje kosc do obiektu bone z pliku, reprezentowanego przez obiekt
 file. Wewnatrz metody wystepuja jej rekurencyjne wywolania.
**********************************************************************/
void Skeleton::readBone(Bone &bone, std::fstream &file)
{
	unsigned int n=0;

	file.read((char*)&bone.id,sizeof(int));	//Odczytanie id.
	file>>bone.name; file.get();			//Odczytanie nazwy.

	file.read((char*)&n,sizeof(int));	//Odczytanie ilosci dzieci.
	for(unsigned int i=0; i<n; i++)		//Dla kazdego dziecka.
	{
		bone.children.push_back(Bone());
		readBone(bone.children.back(),file);
	}
}

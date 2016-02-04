#include "Tfp.h"

using namespace std;

Tfp::Tfp()
{
	path = "";
}

Tfp::Tfp(std::string archFileName)
{
	open(archFileName);
}

Tfp::Tfp(string archFileName, string path)
{
	open(archFileName);
	this->path = path;
}

Tfp::~Tfp()
{
}

bool Tfp::compress(string archFileName, string *fileNamesToCompress, unsigned int num)
{
	fstream f;	
	int *length = new int [num];
	
	file.open(archFileName.c_str(),ios_base::out|ios_base::binary);
	
	file.write("TFP! ",5);	
	file.write((char*)&num,sizeof(int));
	file.write(" ", sizeof(char));
	
	if(!file)
	{
		writeErr("Nie mona utwory¿yæ pliku archiwum.");
		file.close();
		return false;	
	}
	
	for(unsigned int i=0;i<num;i++)
	{
		f.open(fileNamesToCompress[i].c_str(),ios_base::in|ios_base::binary);
		
		if(!f) 
		{
			writeErr("Nie mo¿na odnaleœæ pliku do dodania lub plik jest aktualnie u¿ywany.");
			f.close();
			return false;		
		}
	
		f.seekg( 0, ios::end );
		file.write(fileNamesToCompress[i].c_str(), fileNamesToCompress[i].length()); 
		file.write(" ", sizeof(char));
		int tellg = f.tellg();
		file.write((char*)&tellg, sizeof(int));
		length[i] = tellg;
		f.seekg( 0, ios::beg );	
		f.clear();
		f.close();
	}
		
	for(unsigned int i=0;i<num;i++)
	{
		char *buffer=NULL;
		
		buffer = new char[length[i]];
		
		ifstream in(fileNamesToCompress[i].c_str(),ios::binary);
		in.read(buffer,length[i]);
		file.write(buffer,length[i]);
		in.close();
		delete buffer; buffer=NULL;
	}
	
	file.close();
	return true;
}

bool Tfp::open(string archFileName)
{
	string header;
	header = "";
	
	file.open(archFileName.c_str(),ios_base::in|ios_base::binary);
	
	if(!file) 
	{
		writeErr("Nie mo¿na odnaleœæ pliku lub plik jest aktualnie u¿ywany.");
		file.close();
		return false;		
	}
	
	file>>header; file.get();
	
	if(header.compare("TFP!") < 0)
	{
		writeErr("Plik nie jest poprawnym archiwum TFP!");
		file.close();
		return false;
	}
	
	headerSize=0;
		
	file.read((char*)&numFiles,sizeof(int));
		
	for(unsigned int i=0;i<numFiles;i++)
	{
		file>>names[i]; file.get();
		file.read((char*)&fileSizes[i],sizeof(int));
		headerSize+=names[i].length()+5;
	}
	
	return true;
}

bool Tfp::close()
{
	file.close();
	return true;
}

bool Tfp::decompress(unsigned int id)
{
	ofstream out;

	char *outBuffer=NULL; 
	outBuffer = new char[getFileSize(id)];
				
	string extractPath;
				
	if((path.substr(path.length()-1,1) == "/"))
	{
		extractPath=path+names[id];
		out.open(extractPath.c_str(),ios_base::binary);
	}
	else
	{
		extractPath=path+"/"+names[id];
		out.open(extractPath.c_str(),ios_base::binary);
	}
				
	getFile(id).read(outBuffer,getFileSize(id));
	
	tempNames[id] = names[id];
				
	out.write(outBuffer,getFileSize(id));
	out.close();	
				
	delete outBuffer; outBuffer = NULL;			

	return true;
}

void Tfp::setExtractPath(string path)
{
	this->path=path;
}

string Tfp::getExtractPath()
{
	return path;
}

bool Tfp::decompress(string fileName)
{	
	ofstream out;

	char *outBuffer=NULL; 
	outBuffer = new char[getFileSize(fileName)];
			
	string extractPath;
			
	if((path.substr(path.length()-1,1) == "/"))
	{
		extractPath=path+fileName;
		out.open(extractPath.c_str(),ios_base::binary);
	}
	else
	{
		extractPath=path+"/"+fileName;
		out.open(extractPath.c_str(),ios_base::binary);
	}
			
	getFile(fileName).read(outBuffer,getFileSize(fileName));
	
	tempNames[getFileId(fileName)] = fileName;
			
	out.write(outBuffer,getFileSize(fileName));
	out.close();	
			
	delete outBuffer; outBuffer = NULL;			

	return true;
}

fstream &Tfp::getFile(string fileName)
{		
	for(unsigned int id=0;id<numFiles;id++)
	{	
		if(names[id].compare(fileName.c_str()) == 0)
		{				
			unsigned int size=0;
			
			for(unsigned int i=0;i<id;i++)
				size+=fileSizes[i];
				
			file.seekg(10+headerSize+size);		
					
			break;
		}
	}
	
	return file;
}

fstream &Tfp::getFile(unsigned int id)
{					
	unsigned int size=0;
			
	for(unsigned int i=0;i<id;i++)
		size+=fileSizes[i];
				
	file.seekg(10+headerSize+size);
	
	return file;
}

unsigned int Tfp::getFileSize(string fileName)
{
	for(unsigned int id=0;id<numFiles;id++)
	{	
		if(names[id].compare(fileName.c_str()) == 0)
		{
			return fileSizes[id];
			break;
		}
	
	}	
	return 0;
}

unsigned int Tfp::getFileSize(unsigned int id)
{
	return fileSizes[id];
}

unsigned int Tfp::getFileId(string fileName)
{
	for(unsigned int id=0;id<numFiles;id++)
	{	
		if(names[id].compare(fileName.c_str()) == 0)
		{
			return id;
			break;
		}	
	}	
	return 0;
}

string* Tfp::getFileNamesFromTfp(string fileName)
{
	string header;
	header = "";
	
	ifstream f;
	
	f.open(fileName.c_str(),ios_base::in|ios_base::binary);
		
	if(!f) 
	{
		writeErr("Nie mo¿na odnaleœæ pliku lub plik jest aktualnie u¿ywany.");
		f.close();
		return false;		
	}
		
	f>>header; f.get();
		
	if(header.compare("TFP!") < 0)
	{
		writeErr("Plik nie jest poprawnym archiwum TFP!");
		f.close();
		return false;
	}
		
	headerSize=0;
		
	f.read((char*)&numFiles,sizeof(int));
		
	for(unsigned int i=0;i<numFiles;i++)
		f>>names[i]; f.get();
	
	f.close();	
	return names;
}

void Tfp::deteleDecompressedFiles()
{
	string delPath;
	for(unsigned int i=0;i<numFiles;i++)
	{
		if(tempNames[i].compare("") != 0)
		{
			if((path.substr(path.length()-1,1) == "/"))
				delPath = path+tempNames[i];
			else
				delPath = path+"/"+tempNames[i];
		
			if(remove(delPath.c_str()) != 0)
			{
				string err = "Nie mo¿na usun¹æ pliku "+delPath;
				writeErr(err);
			}
		}
	}
}

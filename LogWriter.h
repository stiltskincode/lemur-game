/**********************************************************************
 Pozniej tutaj napisze opis :)
**********************************************************************/

#ifndef LOGWRITER_H_
#define LOGWRITER_H_


#ifdef WRITE_ALL_LOGS
	#define writeInf(x) LogWriter::writeLog(__FILE__,__LINE__,x,0)
	#define writeWar(x) LogWriter::writeLog(__FILE__,__LINE__,x,1)
	#define writeErr(x) LogWriter::writeLog(__FILE__,__LINE__,x,2)
#else
	#ifdef WRITE_INFO_LOGS
		#define writeInf(x) LogWriter::writeLog(__FILE__,__LINE__,x,0)
	#else
		#define writeInf(x) LogWriter::emptyMethod()
	#endif
	
	#ifdef WRITE_WARNING_LOGS
		#define writeWar(x) LogWriter::writeLog(__FILE__,__LINE__,x,1)
	#else
		#define writeWar(x) LogWriter::emptyMethod()
	#endif
	
	#ifdef WRITE_ERROR_LOGS
		#define writeErr(x) LogWriter::writeLog(__FILE__,__LINE__,x,2)
	#else
		#define writeErr(x) LogWriter::emptyMethod()
	#endif
#endif

#include <string>
#include <fstream>

class LogWriter
{
	public:
		~LogWriter();
		static void begin(std::string fileName, bool erase);
		static void end();
		static void writeLog(const char *f, int l, std::string m, int t);
		static void emptyMethod() {}	//Pusta metoda typu inline.
		
	private:
		LogWriter(); //Konstruktor prywatny.
		
		static std::ofstream file;
		static bool ready;
		static int numErrors, numWarnings;
};

#endif /*LOGWRITER_H_*/

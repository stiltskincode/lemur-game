#include "LogWriter.h"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

ofstream LogWriter::file;
bool LogWriter::ready = false;
int LogWriter::numErrors = 0;
int LogWriter::numWarnings = 0;

LogWriter::LogWriter()
{
}

LogWriter::~LogWriter()
{

}

/**********************************************************************
 Statyczna metoda inicjalizacyjna klasy LogWriter. Jej zadaniem jest
 otworzenie pliku, w ktorym zapisywane beda logi. Inaczej mowiac,
 rozpoczyna ona "sesje" zapisywania logow. W pliku przygotowany
 zostanie stosowny naglowek, informujacy o godzinie i dacie rozpoczecia
 nowej sesj. Mozliwe jest stworzenie zupelnie nowego pliku lub
 dopisywanie do juz istniejacego.
**********************************************************************/
void LogWriter::begin(std::string fileName, bool erase = false)
{
	if(erase) file.open(fileName.c_str());
	else file.open(fileName.c_str(),ios::app);

	file<<"<table border=1>\n";
	file<<"<tr bgcolor=#00FF00>";
	file<<"<td><b>DATA</td><td><b>PLIK</td><td><b>LINIA</td><td><b>TRESC</td>";
	file<<"</tr>\n";
	
	ready=true;
}

/**********************************************************************
 Statyczna metoda konczaca bierzaca sesje logowania. Zapisuje ona do
 pliku odpowiednie znaczniki konczace tabele html oraz tworzy 
 podsumowanie.
**********************************************************************/
void LogWriter::end()
{
	//Zakonczenie, jezeli sesja nie zostala rozpoczeta.
	if(!ready) return;

	
	file<<"<tr><td colspan=4><b>PODSUMOWANIE: ";
	file<<"Ostrzezen: "<<numWarnings;
	file<<"   Bledow: "<<numErrors;
	file<<"</td></tr></table>";
	file<<"\n<br></b>";
	
	file.close();
}
/**********************************************************************
 Podstawowa statyczna metoda, ktora zapisuje log do pliku. NIE powinno
 sie jej wywolywac bezposrednio poprzez LogWriter::writeLog(...).
 Zamiast tego nalezy skorzystac z definicji preprocesora, odpowiednio:
 writeInf("tresc"), writeWar("tresc"), writeErr("tresc").
**********************************************************************/
void LogWriter::writeLog(const char *f, int l, string m, int t)
{
	//Zakonczenie, jezeli sesja nie zostala rozpoczeta.
	if(!ready) return;

	static time_t rawtime;		//Czas w postaci "surowej".
	static tm timeinfo;			//Czas w postaci sformatowanej.
	static char restime[30];	//Czas w postaci lancucha znakow.
	static string color;		//Kolor tla komorki.

	time(&rawtime);
	timeinfo = *localtime(&rawtime);
	strftime(restime,30,"%c",&timeinfo);

	file<<"<tr><td>"<<restime<<"</td><td>"<<f<<"</td><td>"<<l<<"</td>";
	switch(t)
	{
		case 0: file<<"<td>"<<m<<"</td>"; break;
		case 1: file<<"<td bgcolor=#FFFF00><b>"<<m<<"</td>"; numWarnings++; break;
		case 2: file<<"<td bgcolor=#FF0000><b><font color=white>";
				file<<m<<"</td>"; numErrors++; break;
	}
	file<<"</tr>\n";
}

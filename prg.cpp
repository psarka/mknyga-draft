/* Programa surašyta nekultūringai, bet veikia.
 * Ji:
 * Iš keturių failų skteorija.tex, algebra.tex, kombinatorika.tex ir
 * geometrija.tex sugeneruoja šešis failus. Nespalvotai versijai
 * sprendimai.tex (be nuorodų) ir spalvotai sprendimaiC.tex, skteorijaC.tex,
 * algebraC.tex, kombinatorikaC.tex ir geometrijaC.tex (su nuorodomis).
 *
 * Pagrindinį darbą atlieka funkcija parse, kuri nuskaito paduoto failo
 * turinį ir jį modifikavusi perrašo į sprendimų ir atitinkamo skyriaus
 * failus. Sutikusi komandą \input{pavadinimas} ji rekursyviai sudoroja nurodytą failą
 *
 * Į sprendimų failus yra perrašomi skyrių, skyrelių pavadinimai ir
 * uždavinių sprendimai (bei nuorodos, spalvotai versijai). Jei dėl
 * kažkokių priežasčių kokia nors informacija neturi pakliūti į sprendimų
 * failus (pvz. skyrelio be uždavinių pavadinimas), galima ją išskirti
 * originaliame faile komandomis %%noparse ir %%parse (rašomos atskirose
 * eilutėse) pvz:
 *
 * %%noparse
 * \section{Drakonų puota}
 * %%parse
 */

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<stdexcept>

using namespace std;
std::string IntToStr( int n ) //pagalbinė funkcija, paverčianti int į string
{
	std::ostringstream result;
	result << n;
	return result.str();
}

std::string ChrToStr( char* n ) //char* į string
{
	std::ostringstream result;
	result << n;
	return result.str();
}

void parse(char* input, char* otput)
{
	ifstream iin(input);
	ofstream ot(otput, ios::app);
	ofstream out("sprendimai.tex", ios::app);
	ofstream outC("sprendimaiC.tex", ios::app);
	string eile, fvardas, link;
	int i,j,jj,k,f,label = 0;
	int onoff = 0, noparse = 0;

	if (iin.is_open() && out.is_open() && ot.is_open())
	{
		while (!iin.eof())
		{
			getline (iin, eile);
			if (noparse == 1)  //jei įjungtas noparse rėžimas, failo turinys perrašomas tiesiai į
				           //atitinkamą *******C.tex failą, o sprendimų failai nekeičiami.
			{
				if (eile.find("%%parse",0) != string::npos) {noparse=0;}
				if (eile != "") {ot << eile <<"\n";} 
				else {ot << "\n";}
			}
			else
			{
				if (eile.find("%%noparse",0) != string::npos) {noparse=1;}
				i = eile.find("input",0);
				if (i!= string::npos) //jei randamas input, iškviečiama funkcija rekursyviai
				{
					j = eile.find("}",i);
					out.close(); outC.close(); ot.close();
					fvardas = eile.substr(i+6, j-i-6) + ".tex";
					parse(const_cast<char *>(fvardas.data()), otput);
					out.open("sprendimai.tex", ios::app);
					outC.open("sprendimaiC.tex", ios::app);
					ot.open(otput, ios::app);
					eile = "";
				}

				i = eile.find("chapter",0);
				if (i!= string::npos) 
				{
					j = eile.find("}",i);
					out << "\\section*" << eile.substr(i+7, j-i+2) << "\n";
					outC << "\\section*" << eile.substr(i+7, j-i+2) << "\n";

				}
				i = eile.find("\\section",0);
				if (i!= string::npos) 
				{
					j = eile.find("}",i);
					out << "\\subsection*" << eile.substr(i+8, j-i+2) << "\n";
					outC << "\\subsection*" << eile.substr(i+8, j-i+2) << "\n";

				}
				i = eile.find("\\subsection",0);
				if (i!= string::npos) 
				{
					j = eile.find("}",i);
					out << "\\subsubsection*" << eile.substr(i+11, j-i+1)<< "\n";
					outC << "\\subsubsection*" << eile.substr(i+11, j-i+1)<< "\n";

				}
				i = eile.find("\\end{enumerate}",0);
				if (i!= string::npos) 
				{
					if (onoff == 1) 
					{
						out << "\\end{enumerate} \n";
						outC << "\\end{enumerate} \n";
						onoff=0;
					}

				}
				i = eile.find("\\subsubsection{Uždaviniai}",0);
				if (i!= string::npos) 
				{
					onoff = 1;
					out << "\\begin{enumerate}" << "\n";
					outC << "\\begin{enumerate}" << "\n";
				}
				label++;
				i = eile.find("\\item",0);
				if (i!= string::npos && onoff == 1) 
				{
					link = ChrToStr(input) + IntToStr(label); //nuorodos relizuojamos per \label{}
					out << "\\item" << "\n";
					outC << "\\item" << "\\label{" << link << "}"<< 
					"\\mbox{} \\marginpar{ \\hyperref[b" << link <<"]{$\\wedge$}} "<<"\n";

					eile.insert(i+5, " \\mbox{} \\marginpar {\\hyperref[" + link +
						       	"]{$S$}}" + "\\label{b" + link + "}" );

				}
				i = eile.find("%",0);
				if (i!= string::npos && onoff == 1) 
				{

					out << eile.erase(i,1) << "\n";   //viskas, kas užkomentuota uždavinių subsubsection
					outC << eile << "\n";             //keliauja į sprendimų failus be pirmo procento
					eile = "";
				};
					if (eile != "") {ot << eile << "\n";} //tuščios eilutės svarbu!
					else {ot << "\n";}
			}

		};
	};
	ot.close();
	out.close();
	outC.close();
}


int main()
{
	ofstream out ("sprendimai.tex", ios::trunc); //visi failai išvalomi (::trunc)
	ofstream outC("sprendimaiC.tex", ios::trunc);
	ofstream sk("skteorijaC.tex",ios::trunc);
	ofstream al("algebraC.tex",ios::trunc);
	ofstream ko("kombinatorikaC.tex",ios::trunc);
	ofstream ge("geometrijaC.tex",ios::trunc);
	out << "\\chapter{Sprendimai}\n";
	outC << "\\chapter{Sprendimai}\n";
	out.close(); outC.close(); sk.close(); al.close(); ko.close(); ge.close();

	parse("skteorija.tex", "skteorijaC.tex");
	parse("algebra.tex", "algebraC.tex");
	parse("kombinatorika.tex", "kombinatorikaC.tex");
	parse("geometrija.tex", "geometrijaC.tex");
}

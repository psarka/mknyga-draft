#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<stdexcept>

using namespace std;
std::string IntToStr( int n )
{
	std::ostringstream result;
	result << n;
	return result.str();
}

int main()
{
	string eile, ll;
	int i,j,k,f,label=0;
	int onoff = 0;
	ofstream out ("sprendimaiC.tex");
	ofstream ot;
	ifstream iin;
        if (out.is_open()){out << "\\chapter{Sprendimai}\n";}
	for(f = 0; f<=3; f++) 
	{

		if (f == 0) {iin.open("skteorija.tex"); ot.open("skteorijaC.tex");}
		if (f == 1) {iin.open("algebra.tex"); ot.open("algebraC.tex");}
		if (f == 2) {iin.open("kombinatorika.tex"); ot.open("kombinatorikaC.tex");}
		if (f == 3) {iin.open("geometrija.tex"); ot.open("geometrijaC.tex");}

		if (iin.is_open() && out.is_open() && ot.is_open())
		{
			while (!iin.eof())
			{
				getline (iin, eile);
				i = eile.find("chapter",0);
				label++;
				if (i!= string::npos) 
				{
					if (onoff == 1) out << "\\end{enumerate} \n";
					onoff=0;
					j = eile.find("}",i);
					out << "\\section*" << eile.substr(i+7, j-i+2) << "\n";
				}
				i = eile.find("\\section",0);
				if (i!= string::npos) 
				{
					if (onoff == 1) out << "\\end{enumerate} \n";
					onoff=0;
					j = eile.find("}",i);
					out << "\\subsection*" << eile.substr(i+8, j-i+2) << "\n";
				}
				i = eile.find("\\subsection",0);
				if (i!= string::npos) 
				{
					if (onoff == 1) out << "\\end{enumerate} \n";
					onoff=0;
					j = eile.find("}",i);
					out << "\\subsubsection*" << eile.substr(i+11, j-i+1)<< "\n";
				}
				i = eile.find("\\subsubsection{Uždaviniai}",0);
				if (i!= string::npos) 
				{

					onoff = 1;
					out << "\\begin{enumerate}" << "\n";
				}
				i = eile.find("\\item",0);
				if (i!= string::npos && onoff == 1) 
				{

					out << "\\item" << "\\label{" << label << "}"<< "\\mbox{} \\marginpar{ \\hyperref[b" 
						<< label <<"]{$\\wedge$}} "<<"\n";
					eile.insert(i+5, " \\mbox{} \\marginpar {\\hyperref[" + IntToStr(label) + 
							"]{$S$}}" + "\\label{b" + IntToStr(label) + "}" );
				}
				i = eile.find("%",0);
				if (i!= string::npos && onoff == 1) 
				{

					out << eile.erase(i,1)<< "\n"; 
					eile = "";
				};
				if (eile != "") {ot << eile<<"\n";}
				else {ot << "\n";}


			};
		};
		iin.close();
		ot.close();
	}
	out << "\\end{enumerate}\n";
	out.close();
}; 



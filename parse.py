# -*- coding: UTF-8 -*-
import re
import pdb


def parse (inputf, outputf):
    iin = open(inputf, 'r')
    ot  = open(outputf,'a')
    out = open("sprendimai.tex", 'a')
    outC= open("sprendimaiC.tex",'a')
    label  = 0
    onoff  = 0
    noparse= 0

    for eile in iin:

        if noparse:
            if "%%parse" in eile:
                noparse = 0
            ot.write(eile)
        else:
            if "%%noparse" in eile:
                noparse = 1
            if "\input" in eile:
                i = eile.find("input")
                j = eile.find("}")
                out.close()
                outC.close()
                ot.close()
                fvardas = eile[i+6:j] + ".tex"
                parse(fvardas, outputf); 
                ot  = open(outputf,'a') 
                out = open("sprendimai.tex", 'a') 
                outC= open("sprendimaiC.tex",'a')
                eile= ""
            
            i = eile.find("chapter",0)
            if (i!= -1):
        
                j = eile.find("}",i);
                out.write("\section*" + eile[i+7:j-i+2] + "\n")
                outC.write("\section*" + eile[i+7:j-i+2] + "\n")
            
            i = eile.find("\section",0)
            if (i!= -1): 
                        
                j = eile.find("}",i);
                out.write("\subsection*" + eile[i+8:j-i+2] + "\n")
                outC.write("\subsection*" + eile[i+8:j-i+2] + "\n")

            i = eile.find("\subsection",0)
            if (i!= -1): 
        
                j = eile.find("}",i);
                out.write("\subsubsection*" + eile[i+11:j-i+1]+"\n")
                outC.write("\subsubsection*" + eile[i+11:j-i+1]+"\n")

            i = eile.find("\end{enumerate}",0)
            if (i!= -1): 
                if onoff: 
                    out.write("\end{enumerate} \n")
                    outC.write("\end{enumerate} \n")
                    onoff = 0

            i = eile.find(u"\subsubsection{Uždaviniai}".encode('utf8'),0);
            if (i!= -1): 
                #pdb.set_trace()
                onoff = 1
                out.write("\\begin{enumerate} \n");
                outC.write("\\begin{enumerate} \n");

            label+=1
            i = eile.find("\item",0);
            if i!= -1 and onoff: 

                link = inputf + str(label) #nuorodos relizuojamos per \label{}
                out.write("\item \n");
                outC.write("\item" + "\label{" + link + "}" + "\mbox{} \marginpar{\hyperref[b" + link + "]{$\wedge$}} \n")
                eile = eile[:i+5] + "\mbox{} \marginpar {\hyperref[" + link + "]{$S$}}" + "\label{b" + link + "}" + eile[i+5:]


            if re.match('\s*%', eile) and onoff:
                i = eile.find("%",0)
                out.write(eile[i+1:])   #viskas, kas užkomentuota uždavinių subsubsection
                outC.write(eile[i+1:])  #keliauja į sprendimų failus be pirmo procento
                eile = "";

            if (eile != ""):
                ot.write(eile)

    ot.write("\n")
    iin.close()
    ot.close()
    out.close()
    outC.close()



#Truncate and prepare files
out = open('sprendimai.tex', 'w')   
outC = open('sprendimaiC.tex', 'w')
sk = open('algebraC.tex', 'w')
al = open('skteorijaC.tex', 'w')
ko = open('kombinatorikaC.tex', 'w')
ge = open('geometrijaC.tex', 'w')
out.write( "\chapter{Sprendimai}\n")
outC.write("\chapter{Sprendimai}\n")
out.close(); outC.close(); sk.close(); al.close(); ko.close(); ge.close();

#Parse
parse("skteorija.tex", "skteorijaC.tex");
parse("algebra.tex", "algebraC.tex");
parse("kombinatorika.tex", "kombinatorikaC.tex");
parse("geometrija.tex", "geometrijaC.tex");









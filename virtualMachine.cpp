#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include "Register.h"

using namespace std;

void Load(std::ofstream &);
void Init();
//void StartEXc();
//void ExeUserProgram();
//void MOS();
//void Read();
//void Write();
//void Terminate();
void visualMem();
Register regis;

int main()
{
    std::stringstream ss;
    std::string filename="input.txt",ins_line;
    std::ifstream fin;
    std::ofstream fout;
    
    fin.open(filename);
    fout.open("output.txt",std::ios::out);
    if (fin.is_open() && fout.is_open()) {
        cout << "File opened!" << endl;
    }
    else { 
        std::cerr << "File coundnt be opened" << std::endl; 
    }

    while (!fin.eof())
    {
        std::getline(fin, ins_line);
        regis.instruction.push_back(ins_line);
    }

    Load(fout);

    fin.close();
    fout.close();
}

void Load(std::ofstream &fout)
{
    for (auto i = regis.instruction.begin(); i != regis.instruction.end(); ++i)
    {
        std::string su= *i;
        std::string st_end = "";
        for (int j = 0; j < 4; j++) { 
            st_end += su[j]; 
        }

        if (st_end == "$AMJ") { 
            //visualMem();          //to visualize the memory 
            Init();
            //fout << "$AMJ" << std::endl; 
        }

        else if (st_end == "$DTA") { 
            //StartExc();
            //fout << "$DTA" << std::endl;
        }

        else if (st_end == "$END") { 
            continue; 
        }

        else
        {   
            //program card
            int su_size=su.length()-1;
            for (int k = 0; k < su_size; k+=4)
            {
                std::string word = "";
                for (int g = 0; g < 4;g++) { 
                    word+= su[k+g];
                }
                regis.memory.push_back(word);
                regis.ins_word.push_back(word);
                fout << word;
                fout << "\n";
            }
        }
    }
}

void visualMem(){

    for(int i = 0; i < regis.memory.size(); ++i){
        if(i < 10)
            cout << "0" << i << "\t" << regis.memory.at(i) << endl;
        else
            cout << i << "\t" << regis.memory.at(i) << endl;
    }
}

void Init(){
   regis.IR[4] = {0};
   regis.M[2] = {0};
   regis.IC[2] = {0};
   regis.R[4] = {0};
   regis.C = 0;
   regis.memory.clear();
}

/* void StartExc(){
   ExeUserProgram();
} */

/* void ExeUserProgram(){
   regis.IR = regis.IC;
} */

/* void MOS(){}
void Read(){}
void Write(){}
void Terminate(){} */
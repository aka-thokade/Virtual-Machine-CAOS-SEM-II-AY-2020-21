#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <array>
#include "Register.h"

using namespace std;

void Load(std::ofstream &);
void Init();
void StartExc();
void ExeUserProgram();
void MOS(int, std::ofstream &);
void Read();
void Write();
void Terminate(std::ofstream &);
void visualMem();
Register regis;

string Memory[100];

int main()
{
    std::stringstream ss;
    std::string filename = "input.txt", ins_line;
    std::ifstream fin;
    std::ofstream fout;

    fin.open(filename);
    fout.open("output.txt", std::ios::out);
    if (fin.is_open() && fout.is_open())
    {
        cout << "File opened!" << endl;
    }
    else
    {
        std::cerr << "File coundnt be opened" << std::endl;
    }

    while (!fin.eof())
    {
        std::getline(fin, ins_line);
        regis.instruction.push_back(ins_line);
    }

    Load(fout);
    visualMem();

    fin.close();
    fout.close();
}

void Load(std::ofstream &fout)
{
    for (auto i = regis.instruction.begin(); i != regis.instruction.end(); ++i)
    {

        std::string inputLine = *i;
        strcpy(regis.buffer, inputLine.c_str());
        regis.buffer[40] = {'\0'};

        std::string controlCard = "";
        for (int j = 0; j < 4; j++)
        {
            controlCard += regis.buffer[j];
        }

        int memNeeded;

        if (controlCard == "$AMJ")
        {
            //visualMem();          //to visualize the memory
            Init();
            memNeeded = stoi(inputLine.substr(8, 4));
            //fout << "$AMJ" << std::endl;
        }

        else if (controlCard == "$DTA")
        {
            StartExc();
            //fout << "$DTA" << std::endl;
        }

        else if (controlCard == "$END")
        {
            continue;
        }

        else
        {
            //program card
            //TODO: instruction ko 4 me break down karna hai
            // TODO: store that in mem
            int inputLine_size = inputLine.length() - 1;
            int memLoc = 0;
            for (int k = 0; k < inputLine_size; k += 4)
            {

                string word = "";
                for (int l = 0; l < 4; ++l)
                {
                    /* if(inputLine[0] == 'H')
                        word = "H\0\0\0";
                    else
                        word += inputLine[k+l]; */
                    word += inputLine[k + l];
                }
                regis.ins_word.push_back(word);
                Memory[memLoc] = word;
                cout << "State " << memLoc + 1 << endl;
                visualMem();
                memLoc++;
            }
        }
    }
}

void visualMem()
{

    cout << "in visualmem" << endl;

    for (int i = 0; i < 5; ++i)
    {
        if (i < 10)
            cout << "0" << i << "\t" << Memory[i] << endl;
        else
            cout << i << "\t" << Memory[i] << endl;
    }
}

void Init()
{
    cout << "in init" << endl;
    regis.IR[4] = {'\0'};
    regis.M[2] = {0};
    regis.IC[2] = {0};
    regis.R[4] = {'\0'};
    regis.C = 0;
    Memory[100] = {'\0'};
    regis.buffer[40] = {'\0'};
}

void StartExc()
{
    cout << "in startexec" << endl;
    regis.IC[2] = {0};
    ExeUserProgram();
}

void ExeUserProgram()
{
    cout << "in exeuserprog" << endl;
    //regis.IR = regis.IC;
}

void MOS(int SI, std::ofstream &fout)
{
    cout << "in MOS" << endl;
    switch (SI)
    {
    case 1:
        // GD__
        Read();
        break;

    case 2:
        // PD__
        Write();
        break;

    case 3:
        // H___
        Terminate(fout);
        break;

    default:
        break;
    }
}

void Read()
{
    cout << "in read" << endl;
}
void Write()
{
    cout << "in write" << endl;
}
void Terminate(std::ofstream &fout)
{
    cout << "in terminate" << endl;
    fout << endl;
    fout << endl;
    Load(fout);
}
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
void StartExc(std::ofstream &fout, std::vector<string>::iterator);
void ExeUserProgram(std::ofstream &fout, std::vector<string>::iterator);
void MOS(int, std::ofstream &, std::vector<string>::iterator);
void Read(std::vector<string>::iterator);
void Write(std::ofstream &);
void Terminate(std::ofstream &);
void visualMem();
void showR();
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
        //regis.buffer[40] = {'\0'};

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
            StartExc(fout, i);
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
            int inputLine_size = inputLine.length();
            int memLoc = 0;
            for (int k = 0; k < inputLine_size; k += 4)
            {

                string word = "";
                for (int l = 0; l < 4; ++l)
                {
                    if (inputLine[k] == 'H')
                        word = "H   ";
                    else
                        word += inputLine[k + l];

                    // word += inputLine[k + l];
                }
                regis.ins_word.push_back(word);
                // just to see which word is being pushed
                /* for(int i = 0; i < regis.ins_word.size(); ++i)
                    cout << "ins_word" << regis.ins_word.at(i) << "\t"; */

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

    for (int i = 0; i < 20; ++i)
    {
        if (i < 10)
            cout << "0" << i << "\t" << Memory[i] << endl;
        else
            cout << i << "\t" << Memory[i] << endl;
    }
}

void showR(){
    cout << "Contents of R: " << regis.R[0] << regis.R[1] << regis.R[2] << regis.R[3] << endl;
}

void Init()
{
    cout << "in init" << endl;
    regis.IR[4] = {'\0'};
    regis.M[2] = {0};
    regis.IC = 0;
    regis.R[4] = {'\0'};
    regis.C = false;
    Memory[100] = {'\0'};
    regis.buffer[40] = {'\0'};
}

void StartExc(std::ofstream &fout, std::vector<string>::iterator i)
{
    cout << "in startexec" << endl;
    regis.IC = 0;
    ExeUserProgram(fout, i);
}

void ExeUserProgram(std::ofstream &fout, std::vector<string>::iterator i)
{
    int SI;
    char cmd[4];
    cout << "in exeuserprog" << endl;
    //regis.IR = regis.IC;
    while (true)
    {
        strcpy(cmd, Memory[regis.IC].c_str());
        for (int x = 0; x < 4; x++)
        {
            regis.IR[x] = cmd[x];
        }

        regis.IC++;

        string opCode = {regis.IR[0], regis.IR[1]};
        string memLoc = {regis.IR[2], regis.IR[3]};

        int opAddress;
        if (opCode != "H") //Hault does not have opAddress
            opAddress = stoi(memLoc);

        if (opCode == "LR") //If opCode is Load Register
        {
            for (int x = 0; x < 4; x++)
                regis.R[x] = Memory[opAddress][x]; //Loading the word pointed by opAddress to Register
            showR();
        }
        else if (opCode == "SR")
        {
            for (int x = 0; x < 4; x++)
                Memory[opAddress][x] = regis.R[x]; //Storing the word from Register to the memory pointed by opAddress
        }
        else if (opCode == "CR") //If opCode is Compare Register
        {
            string register_content = {regis.R[0], regis.R[1], regis.R[2], regis.R[3]};
            string memory_content = {Memory[opAddress][0], Memory[opAddress][1], Memory[opAddress][2], Memory[opAddress][3]};
            if (register_content == memory_content)
                regis.C = true;
            else
                regis.C = false;
        }
        else if (opCode == "BT") //If opCode is Branch on True
        {
            if (regis.C == true)
                regis.IC = opAddress;
        }
        else if (opCode == "GD") //If opCode is Get Data
        {
            SI = 1;
            MOS(SI, fout, i);
        }
        else if (opCode == "PD") //If opCode is Put Data
        {
            SI = 2;
            MOS(SI, fout, i);
        }
        else if (opCode == "H") //If opCode is Hault
        {
            SI = 3;
            MOS(SI, fout, i);
            break; //break while loop on encountering H
        }
    }
}

void MOS(int SI, std::ofstream &fout, std::vector<string>::iterator i)
{
    cout << "in MOS" << endl;
    switch (SI)
    {
    case 1:
        // GD__
        Read(i);
        break;

    case 2:
        // PD__
        Write(fout);
        break;

    case 3:
        // H___
        Terminate(fout);
        break;

    default:
        cout << "Interrupt not found" << endl;
        break;
    }
}

void Read(std::vector<string>::iterator i)
{
    cout << "in read" << endl;
    int k = 0;
    string word;
    regis.IR[3] = '0';
    string memLoc = {regis.IR[2], regis.IR[3]};  //Combining IR[2],IR[3] to get opAddress
    int opAddress = stoi(memLoc);                //Converting memLoc to integer
    regis.buffer[40] = {'\0'};                   // initialize buffer

    for (i = regis.instruction.begin(); i != regis.instruction.end(); ++i){
        std::string inputLine = *i;
        strcpy(regis.buffer, inputLine.c_str());

        std::string controlCard = "";
        for (int j = 0; j < 4; j++)
        {
            controlCard += regis.buffer[j];
        }

        if(controlCard == "$DTA"){
            i++;
            inputLine = *i;
            strcpy(regis.buffer, inputLine.c_str());
            cout << "Contents of buffer:" << endl;
            for(int x = 0; x < 40; ++x){
                cout << regis.buffer[x] << endl;
            }

            for (int x = 0; x < 10; x++) //For next 10 Blocks
            {
                for (int j = 0; j < 4; j++) //For the words in one block
                {
                    if (regis.buffer[0] == '$' && regis.buffer[1] == 'E' && regis.buffer[2] == 'N' && regis.buffer[3] == 'D') 
                    {
                        cout << "\nError: Out of Data";
                    }
                    word += regis.buffer[k]; //Initializing next 10 blocks with content of regis.buffer
                    k++;
                }
                Memory[opAddress + x] = word;
                word = "";
            }
        }
    }
    
    
    visualMem();
}

void Write(std::ofstream &fout)
{
    cout << "in write" << endl;
    regis.IR[3] = '0';
    string memLoc = {regis.IR[2], regis.IR[3]}; //Combining IR[2],IR[3] to get opAddress
    int opAddress = stoi(memLoc);                  //Converting memLoc to integer
    for (int x = 0; x < 10; x++)                //For next 10 Blocks
        for (int j = 0; j < 4; j++)             //For the words in one block
            fout << Memory[opAddress + x];   //Writing the contents to the output file
    fout << "\n";
}
void Terminate(std::ofstream &fout)
{
    cout << "in terminate" << endl;
    fout << "\n";
    fout << "\n";
    Load(fout);
}
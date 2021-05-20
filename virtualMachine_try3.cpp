#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class virtualMachine
{
    char Memory[100][4]; //Main Memory
    char IR[4];          //Instruction Register (4 bytes)
    int IC;              //Instruction Counter Register
    char R[4];           //General Purpose Register (4 bytes)
    bool C;              //Toggle (1 byte)
    int M;               //Memory Pointer
    char Buffer[40];     //Buffer (40 bytes)
    int SI;              //SI interrupt for service request

public:
    fstream fin;
    fstream fout;

    void Load();
    void Init();
    void StartExec();
    void ExecUserProgram();
    void MOS();
    void Read();
    void Write();
    void Terminate();

    void visualMem();
    void showR();
    void showC();
    void clearBuffer();
    void clearRegister();
};

void virtualMachine::Load()
{
    M = 0;
    while (!fin.eof())
    {
        for (int i = 0; i < 40; i++) //Initializing the Buffer
            Buffer[i] = '\0';

        fin.getline(Buffer, 40); //Getting the line from input file

        if (Buffer[0] == '$') //Control Card
        {
            if (Buffer[1] == 'A' && Buffer[2] == 'M' && Buffer[3] == 'J')
            {
                Init();
            }
            else if (Buffer[1] == 'D' && Buffer[2] == 'T' && Buffer[3] == 'A')
            {
                StartExec();
            }
            else
            {
                continue;
            }
        }
        else //Program Card
        {
            if (M >= 100)
            {
                cout << "Error: Memory Full";
                exit(100);
            }
            for (int b = 0; b < 40;)
            {
                if (Buffer[b] == '\0') //If end of Buffer contents
                    break;

                for (int i = 0; i < 4; i++, b++)
                {
                    Memory[M][i] = Buffer[b]; //Write from Buffer to Main Memory
                    if (Buffer[b] == 'H')     //If Hault statement found [Hault does not have opAddress]
                    {
                        b++;
                        break;
                    }
                }
                M++;
            }
        }
    }
}

void virtualMachine::Init()
{
    // cout << "in Init" << endl;

    /* Memory[100][4] = {' '};
    IR[4] = {' '};
    IC = 0;
    R[4] = {' '};
    C = false;
    M = 0;
    Buffer[40] = {' '};
    SI = 0; */
    
    IC = 0;
    SI = 0;
    M = 0;
    C = false;
    for (int i = 0; i < 4; i++)
    {
        IR[i] = ' ';
        R[i] = ' ';
    }
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 4; j++)
            Memory[i][j] = ' ';
    }
}

void virtualMachine::StartExec()
{
    // cout << "in StartExec" << endl;

    IC = 0;
    ExecUserProgram();
}

void virtualMachine::ExecUserProgram()
{
    // cout << "in ExecUserProgram" << endl;

    while (true)
    {
        for (int i = 0; i < 4; i++)
            IR[i] = Memory[IC][i];
        IC++;
        string opCode = {IR[0], IR[1]};
        string location = {IR[2], IR[3]};
        int opAddress;
        if (opCode != "H ") //Hault does not have opAddress
            opAddress = stoi(location);

        if (opCode == "LR") //If opCode is Load Register
        {
            for (int i = 0; i < 4; i++)
                R[i] = Memory[opAddress][i]; //Loading the word pointed by opAddress to Register
            showR();
        }
        else if (opCode == "SR")
        {
            for (int i = 0; i < 4; i++)
                Memory[opAddress][i] = R[i]; //Storing the word from Register to the memory pointed by opAddress
            visualMem();
        }
        else if (opCode == "CR") //If opCode is Compare Register
        {
            string register_content = {R[0], R[1], R[2], R[3]};
            string memory_content = {Memory[opAddress][0], Memory[opAddress][1], Memory[opAddress][2], Memory[opAddress][3]};
            if (register_content == memory_content)
                C = true;
            else
                C = false;
            showC();
        }
        else if (opCode == "BT") //If opCode is Branch on True
        {
            if (C == true)
                IC = opAddress;
        }
        else if(opCode == "AD") //If opCode is Add two numbers
        {
            string register_content = {R[0], R[1], R[2], R[3]};
            string memory_content = {Memory[opAddress][0], Memory[opAddress][1], Memory[opAddress][2], Memory[opAddress][3]};
            int num1 = stoi(memory_content);
            int num2 = stoi(register_content);
            int sum = num1 + num2;
            
            int digit;
            char cDigit;
            int R_posCount = 3;
            clearRegister();
            //to store sum in R
            while (sum > 0)
            {
                digit = sum % 10;
                cDigit = digit + '0';
                sum = sum / 10;
                R[R_posCount] = cDigit;
                R_posCount--;
            }

            showR();
        }
        else if (opCode == "GD") //If opCode is Get Data
        {
            SI = 1;
            MOS();
        }
        else if (opCode == "PD") //If opCode is Put Data
        {
            SI = 2;
            MOS();
        }
        else if (opCode == "H ") //If opCode is Hault
        {
            SI = 3;
            MOS();
            break;
        }
    }
}

void virtualMachine::MOS()
{
    // cout << "in MOS" << endl;

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
        Terminate();
        break;

    default:
        cout << "Interrupt not found" << endl;
        break;
    }
}

void virtualMachine::Read()
{
    // cout << "in Read" << endl;

    int k = 0;
    IR[3] = '0';
    string location = {IR[2], IR[3]}; //Combining IR[2],IR[3] to get opAddress
    int opAddress = stoi(location);   //Converting to integer
    for (int i = 0; i < 40; i++)      //Initializing Buffer
        Buffer[i] = '\0';
    fin.getline(Buffer, 40); //Getting the Data cards line by line

    for (int i = 0; i < 10; i++) //For next 10 Blocks
    {
        for (int j = 0; j < 4; j++) //For the words in one block
        {
            if (Buffer[0] == '$' && Buffer[1] == 'E' && Buffer[2] == 'N' && Buffer[3] == 'D') //If $END is found
            {
                cout << "\nError : Out of Data";
            }
            Memory[opAddress + i][j] = Buffer[k]; //Initializing next 10 blocks with content of Buffer
            k++;
        }
    }

    visualMem();
}

void virtualMachine::Write()
{
    // cout << "in Write" << endl;

    IR[3] = '0';
    string location = {IR[2], IR[3]};         //Combining IR[2],IR[3] to get opAddress
    int opAddress = stoi(location);           //Converting to integer
    for (int i = 0; i < 10; i++)              //For next 10 Blocks
        for (int j = 0; j < 4; j++)           //For the words in one block
            fout << Memory[opAddress + i][j]; //Writing the contents to the output file
    fout << "\n";

    visualMem();
}

void virtualMachine::Terminate()
{
    // cout << "in Terminate" << endl;
    fout << "\n\n"; //Two blank lines in output file
}

void virtualMachine::visualMem(){
    // cout << "in visualMem" << endl;

    for (int i = 0; i < 40; ++i)
    {
        if (i < 10)
            cout << "0" << i << "\t" << Memory[i][0] << Memory[i][1] << Memory[i][2] << Memory[i][3] << endl;
        else
            cout << i << "\t" << Memory[i][0] << Memory[i][1] << Memory[i][2] << Memory[i][3] << endl;
    }
}

void virtualMachine::showR(){
    // cout << "in showR" << endl;
    cout << "Contents of R: " << R[0] << R[1] << R[2] << R[3] << endl;
}

void virtualMachine::showC(){
    // cout << "in showC" << endl;
    cout << "Status of C: " << C << endl;
}

void virtualMachine::clearBuffer(){
    // cout << "in clearBuffer" << endl;
    for (int i = 0; i < 40; ++i)
    {
        Buffer[i] = ' ';
    }  
}

void virtualMachine::clearRegister(){
    // cout << "in clearBuffer" << endl;
    for (int i = 0; i < 4; ++i){
        R[i] = ' ';
    }
}

int main()
{
    virtualMachine machine1;

    //Taking Input File
    machine1.fin.open("input.txt", ios::in);
    if (!machine1.fin)
    {
        cout << "Please check the input file.\n\nExiting Program" << endl;
        exit(0);
    }

    //Initializing Output File
    machine1.fout.open("output.txt", ios::out);

    //Calling the Load Function
    machine1.Load();

    return 0;
}

/*
    This is simple IAS machine simulator in cpp code
    Author -> Pushkar Sandip Pawar
    To run instruction load them in memory starting from memory[0]
    Two sample programs p1_sum_of_first_n_numbers(long int memory[]) , sum_of_two_numbers( long int memory[]) are shown.
*/
#include <iostream>
#include<bits/stdc++.h>
using namespace std;
void decode_and_execute(int IR,int MAR,long int memory[],long int *AC,long int* MQ,long int *MBR,int *PC);
void p1_sum_of_first_n_numbers(long int memory[]);
void sum_of_two_numbers( long int memory[]);


//Sample programs
void p1_sum_of_first_n_numbers(long int memory[])
{
    /*
        Program to find sum of first n numbers
        c implementation
        int counter = 99,curr = 0,sum = 0,one = 1;
        while(true)
        {
            curr = curr + one;
            sum = sum + curr;
            counter = counter - one;
            if(counter < 0) break;
        }

        IAS implementation

        data
        memory location    data 
        500                 0   -> act as current number
        501                 0   -> act as sum (cummulative),final answer stored here
        502                 99  -> act as conunter, put n-1 here to find sum of n numbers
        503                 1   

        memory location     assembly language instructions
            0               LOAD M(500),ADD M(503)
            1               STOR M(500),LOAD M(501)
            2               ADD M(500),STOR M(501)
            3               LOAD M(502),SUB M(503)
            4               STOR M(502),JMP + M(0,0:19)
            5               HLT
    
    */
    //instructions 
    memory[0] = 22002274804;//Hex -> 051F7011F4
    memory[1] = 4820439540;//Hex -> 011F5211F4
    memory[2] = 142259278324; //Hex -> 211F5051F4
    memory[3] = 26297242102; //Hex -> 061F7011F6
    memory[4] = 64424645110; //Hex -> 0F000211F6
    memory[5] = 0;
    //data
    memory[500] = 0;
    memory[501] = 0;
    memory[502] = 99; //put n-1 to find sum of n numbers
    memory[503] = 1;
}
void sum_of_two_numbers( long int memory[])
{   
    /*
        program to find sum of two numbers 
        c implementation 
        int a = 15,b=10;
        int c = a+b;

        IAS 

        data
        mem location    data
            500          10
            501          15   -> final answer stored here

        mem location        assembly language instruction
            0                 NLI,LOAD M(500)     [NLI indicates no left instruction]
            1                 NLI,ADD M(501)
            2                 NLI,STOR M(501)
            3                 HLT

    */
    //instructions
    memory[0] = 4819255296; //HEX => 011F400000
    memory[1] = 22000173056; //HEX => 051F500000
    memory[2] = 142259257344; // HEX => 211F500000
    memory[3] = 0;
    //data
    memory[500] = 10;
    memory[501] = 15;
}

//Main simulator
int main ()
{
    long int memory[1000];	//first 500 entries for instructions and remaining for data
    long int MBR,AC,MQ; //only first 40bits are used from LSB.
    int IBR; //only first 20 bits used from LSB
    int IR; // only first 8 bits used from LSB
    int PC,MAR; //only firts 12 bits used from LSB
    PC =0;
    IBR = 0;
    p1_sum_of_first_n_numbers(memory); //Program to find sum of first n natural numbers
    //sum_of_two_numbers(memory); //Program to find sum of 2 numbers
    int count = 0;
    while(1)    //all zeros in an instruction means halt
    {
        cout<<count;
        count++;
      if(!(IBR)) // checking if IBR is empty or not
      {
           MAR = PC;
           MBR = memory[MAR];
           if(MBR == 0) break;
           if(MBR<<44) // checking if left instruction is required or not.(Fetch stage)
          {
            IBR = MBR>>20;
            IR = MBR;
            IR = IR<<12;
            IR = IR>>24;
            MAR = MBR;
            MAR = MAR<<20;
            MAR = MAR>>20;
            decode_and_execute(IR,MAR,memory,&AC,&MQ,&MBR,&PC); 
          }
          else
          {
              IR = MBR >> 32;
              MAR = MBR>>20;
              MAR = MAR<<20;
              MAR = MAR>>20;
              IBR = 0;
              PC++;
              decode_and_execute(IR,MAR,memory,&AC,&MQ,&MBR,&PC);
          }
      }
      else
      {
          IR = IBR >> 12;
          MAR = IBR <<20;
          MAR = MAR >> 20;
          PC++;
          decode_and_execute(IR,MAR,memory,&AC,&MQ,&MBR,&PC);
          IBR = 0;
      }
      cout <<"\n";
    }
    cout << "\nFinal Ans: " << memory[501]; //Final output of program
}
void decode_and_execute(int IR,int MAR,long int memory[],long int *AC,long int* MQ,long int *MBR,int *PC)
{
    switch(IR) // switch is decode and all the cases are executing the instructions.
    {
        case 1://LOAD M(X)
            *MBR = memory[MAR];
            *AC = *MBR;
            break;
        case 33://STOR M(X)
            *MBR = *AC;
            memory[MAR] = *MBR;
            break;
        case 15://JUMP + M(X,0:19)
            if(*AC>=0) *PC = MAR;
            break;
        case 5://ADD M(X)
            *MBR = memory[MAR];
            *AC = *AC+*MBR;
            break;
        case 6://SUB M(X)
            *MBR = memory[MAR];
            *AC = *AC - *MBR;
            break;
    }
    cout << " IR=>" << IR;
    cout << " MAR=>"<< MAR;
    cout << " PC=>"<<*PC;
    cout << " MBR=>"<<*MBR;
    cout <<" AC=>"<<*AC;
}

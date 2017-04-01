#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Register[26];
int Status=0; //status register in which bit masking is done for storing various information
int Reminders[100];
int Count=1;
int StackPointer=248;
int ProgramCounter=1;
char IR[100];

void convertToString(char operand[100], int num);
int convertToInt(char* str);
int convertToIntBase10(char* str);
void convertBinaryToInstruction();
void callFunction();
int opcodeCompare(char opcode[100], char function_opcode[100]);
void callFunction(char opcode[100], char binary_operand1[100], char binary_operand2[100]);
void readRegisterFromFile();

//--------------------------------INITIALIZERS-------------------------------------//

void initializeReg();
void reset();
void initalizeStack();

//-----------------------------------FUNCTIONS-------------------------------------//
void KITCHEN();
void INFO();
void POP();
void IRRIGATION();
void ENTERTAINMENT();
void REMINDER();
void PUSH(int binary_operand1);
void SETREMINDER(int binary_operand1);
void FLY(int binary_operand1);
void WATERCLOSET(int binary_operand1);
void WEATHERFORECAST(int binary_operand1);
void SETTINGS(int binary_operand1);
void DOOR(int binary_operand1);
void STORE(int binary_operand1, int binary_operand2);
void LOAD(int binary_operand1, int binary_operand2);
void MOVE(int binary_operand1, int binary_operand2);
void HVAC(int binary_operand1, int binary_operand2);
void LIGHT(int binary_operand1, int binary_operand2);
void RET();

//--------------------------------CONTROL SIGNALS--------------------------------//

int cmp(int a, int b);
int ADD(int a,int b);
int SUB(int a,int b);
int MUL(int a,int b);
int DIV(int a,int b);
void HALT();
void MOVE(int RegIndex1,int RegIndex2);
void IncrementPC();
void displayCPURegister();

//-------------------------------SHAKTIMAAN---------------------------------------//

int main(){
  initalizeStack();
  readRegisterFromFile();

  convertBinaryToInstruction();


  return 0;
}

void readRegisterFromFile(){

  FILE* readRegister = fopen("RegisterValue.txt", "r");
  char registerValue[100], line[100];
  int index=0;

  while(fgets(line, 100, readRegister)){

    int length = strlen(line), i;

    for(i=0; i<length; i++){
      if(line[i] == ' '){
        strncpy(registerValue, line+i+1, length - i - 2);

        registerValue[length-i-2] = '\0';

        Register[index++] = convertToIntBase10(registerValue);
        break;
      }
    }

  }

  fclose(readRegister);

  return ;
}

//convert string to decimal
int convertToIntBase10(char* str)
{
    if(str[0] == '-')
      return -1;
    int i,res = 0; // Initialize result
 
    // Iterate through all characters of input string and
    // update result
    for (i = 0; str[i] != '\0'; ++i)
      res = res*10 + str[i] - '0';
 
    // return result.
    return res;
}

int opcodeCompare(char opcode[100], char function_opcode[100]){
  FILE* opcodeRead = fopen("opcode.txt", "r");

  char binary_opcode[100], instruction[100];

  while(fgets(instruction, 100, opcodeRead)){

    int lengthOfInstruction = strlen(instruction)-2, i;

    for(i=0; i<lengthOfInstruction; i++){
      if(instruction[i] == ' '){
        strncpy(binary_opcode, instruction+i+1, lengthOfInstruction-i);

        binary_opcode[lengthOfInstruction-i] = '\0';

        if(strcmp(opcode, binary_opcode) == 0){
          strncpy(function_opcode, instruction, i);
          function_opcode[i] = '\0';
          fclose(opcodeRead);
          return 1;
        }
      }
    }

  }

  fclose(opcodeRead);
  return 0;
} 

void callFunction(char opcode[100], char binary_operand1[100], char binary_operand2[100]){

  if(strcmp(opcode, "KITCHEN") == 0)
    KITCHEN();

  else if(strcmp(opcode, "INFO") == 0)
    INFO();

  else if(strcmp(opcode, "POP") == 0)
    POP();

  else if(strcmp(opcode, "IRRIGATION") == 0)
    IRRIGATION();

  else if(strcmp(opcode, "ENTERTAINMENT") == 0)
    ENTERTAINMENT();

  else if(strcmp(opcode, "REMINDER") == 0)
    REMINDER();

  else if(strcmp(opcode, "PUSH") == 0)
    PUSH(convertToInt(binary_operand1));

  else if(strcmp(opcode, "SETREMINDER") == 0)
    SETREMINDER(convertToInt(binary_operand1));

  else if(strcmp(opcode, "FLY") == 0)
    FLY(convertToInt(binary_operand1));

  else if(strcmp(opcode, "WATERCLOSET") == 0)
    WATERCLOSET(convertToInt(binary_operand1));

  else if(strcmp(opcode, "WEATHERFORECAST") == 0)
    WEATHERFORECAST(convertToInt(binary_operand1));

  else if(strcmp(opcode, "SETTINGS") == 0)
    SETTINGS(convertToInt(binary_operand1));

  else if(strcmp(opcode, "DOOR") == 0)
    DOOR(convertToInt(binary_operand1));

  else if(strcmp(opcode, "STORE") == 0)
    STORE(convertToInt(binary_operand1), convertToInt(binary_operand2));

  else if(strcmp(opcode, "LOAD") == 0)
    LOAD(convertToInt(binary_operand1), convertToInt(binary_operand2));

  else if(strcmp(opcode, "MOVE") == 0)
    MOVE(convertToInt(binary_operand1), convertToInt(binary_operand2));

  else if(strcmp(opcode, "HVAC") == 0)
    HVAC(convertToInt(binary_operand1), convertToInt(binary_operand2));

  else if(strcmp(opcode, "LIGHT") == 0)
    LIGHT(convertToInt(binary_operand1), convertToInt(binary_operand2));

  else if(strcmp(opcode, "RET") == 0)
    RET();

  else if(strcmp(opcode, "HALT") == 0)
    HALT();

  return ;
}

//main function which converts instruction into binary code
void convertBinaryToInstruction(){

  FILE* input = fopen("binary_code.txt", "r");

  char line[100], instruction[100];
  char binary_operand1[100], binary_operand2[100],  opcode[100], function_opcode[100];

  int prev, count, i, j;

  while (fgets(line, 100, input)) {

    int length = strlen(line);

    strncpy(opcode, line, 6);

    opcode[6] = '\0';

    if(strcmp(opcode, "111111") == 0){
      strncpy(opcode, line, 11);
      
      opcode[11] = '\0';

      if(strcmp(opcode, "11111111111") == 0){
        strncpy(opcode, line, 16);

        opcode[16] = '\0';
        
        if(opcodeCompare(opcode, function_opcode)){
          strcpy(IR,function_opcode);
          //strcat(IR," ");
          callFunction(function_opcode, binary_operand1, binary_operand2);
        }  

      } else{
        strncpy(binary_operand1, line+11, 5);

        binary_operand1[5] = '\0';

        if(opcodeCompare(opcode, function_opcode)){
          char temp[100];

          strcpy(IR,function_opcode);
          strcat(IR," ");
          
          convertToString(temp, convertToInt(binary_operand1));
          strcat(IR, temp);
        
          callFunction(function_opcode, binary_operand1, binary_operand2);
        }
      }

    } else{
      strncpy(binary_operand1, line+6, 5);
      strncpy(binary_operand2, line+11, 5);

      binary_operand1[5] = '\0';
      binary_operand2[5] = '\0';

      if(opcodeCompare(opcode, function_opcode)){
        char temp[100];

        strcpy(IR,function_opcode);
        strcat(IR," ");

        convertToString(temp, convertToInt(binary_operand1));
        strcat(IR, temp);
        strcat(IR, " ");


        convertToString(temp, convertToInt(binary_operand2));
        strcat(IR, temp);
        
        callFunction(function_opcode, binary_operand1, binary_operand2);
      }
    }
    if(strcmp(function_opcode,"FLY")!=0)
        displayCPURegister();

  }  

  fclose(input);
}

//converts decimal to string
void convertToString(char operand[100], int num){
  int i=2;

  operand[0]='R';

  while (num != 0){
      int rem = num % 10;
      operand[i--] =  rem + '0';
      num = num/10;
  }
  operand[3]='\0';
  return ;
}

//convert string to decimal
int convertToInt(char* str)
{
    int i,res = 0, length=strlen(str), value=1; // Initialize result
 
    // Iterate through all characters of input string and
    // update result
    for (i = length-1; i >= 0 ; i--){
      res = res + (str[i] - '0')*value ;
      value *= 2;
    }
      
    // return result.
    return res;
}

//////////////////--------------FUNCTIONS---------------//////////////
struct Stack1{
  int TOS;
  int BOS;
  int array[100];
};
typedef struct Stack1 Stack1;

Stack1 stack1;

void initializeReg(){

  int i;

  for(i=1;i<=27;i++)
    Register[i]=0;
}

void reset(){

  initializeReg();
 

  ProgramCounter=0;
  //Stack Pointer
  StackPointer=248;

}

void initalizeStack(){
  stack1.TOS=-1;
  stack1.BOS=-1;
  StackPointer=248;
}


void PUSH(int RegIndex){
    
    stack1.TOS += 1;
    stack1.array[stack1.TOS]=RegIndex;
    StackPointer+=2;//SP
}

void POP(){

  if(StackPointer>248){

    ProgramCounter=stack1.array[stack1.TOS];
    stack1.TOS -= 1;
    StackPointer -= 2;//SP
  
  }
  else{
    printf("Stack underflow");
  }
  
}

void RET(){
  printf("-----------------------------*****************--------------------------RET------------------*****************------------\n\n");
  displayCPURegister();
  return ;
}

void FLY(int RegIndex){

  printf("-------------------****************-------------------FLY----------------------------*****************-----\n\n");
  printf("FLY to Label\n");
  displayCPURegister();
  PUSH(ProgramCounter);
  ProgramCounter=Register[RegIndex];

  FILE* inputFly = fopen("binary_code.txt", "r");

  char line[100], instruction[100];
  char binary_operand1[100], binary_operand2[100],  opcode[100], function_opcode[100];

  int prev, count, i, j, lineCount=1;

  while (fgets(line, 100, inputFly)) {
   
    if(lineCount != Register[RegIndex]){
      lineCount++;
      continue;
    }
 
    int length = strlen(line);

    strncpy(opcode, line, 6);

    opcode[6] = '\0';

    if(strcmp(opcode, "111111") == 0){
      strncpy(opcode, line, 11);
      
      opcode[11] = '\0';

      if(strcmp(opcode, "11111111111") == 0){
        strncpy(opcode, line, 16);

        opcode[16] = '\0';

        if(opcodeCompare(opcode, function_opcode)){
          callFunction(function_opcode, binary_operand1, binary_operand2);
          if(strcmp(function_opcode, "RET") == 0)
            break;
        }  

      } else{
        strncpy(binary_operand1, line+11, 5);

        binary_operand1[5] = '\0';

        if(opcodeCompare(opcode, function_opcode)){
          callFunction(function_opcode, binary_operand1, binary_operand2);
        }
      }

    } else{
      strncpy(binary_operand1, line+6, 5);
      strncpy(binary_operand2, line+11, 5);

      binary_operand1[5] = '\0';
      binary_operand2[5] = '\0';

      if(opcodeCompare(opcode, function_opcode)){
        callFunction(function_opcode, binary_operand1, binary_operand2);
      }
    }
    if(strcmp(function_opcode,"RET")!=0){
      printf("\nHELLO\n");
      displayCPURegister();
    }

  }  

  fclose(inputFly);

  POP();
}

//-----------------------FUNCTIONS -------------------------------------//

void IRRIGATION(){

  printf("\n\n-------------*************-----------IRRIGATION---------------------**************-------------------------------\n\n");
  char a[2][100]={"It's Rainy Season","It's hot Season"};
  
  int select=rand()%2;

  if(cmp(select,0)==1){
    printf("%s\n",a[select]);
    printf("Plants don't need Water\n");
  }
  else{
    printf("%s\n",a[select]);
    printf("Plants need Water\n");
  } 
}

void KITCHEN(){
      
      printf("\n\n-------------*************-------------------------KITCHEN---------------------**************-------------------------------\n\n");
      char a[2][20]={"Gas is ON","Ventilation started"};
  
      int select=rand()%2;

      printf("%s\n",a[select]);

}

void SETTINGS(int RegIndex){

    printf("\n\n-----------------------********----------------SETTINGS-------------------********-----------------------------\n\n");
    int password=5;
    int volume=5;
    int value=Register[RegIndex];

    if(cmp(password,value)==1){
        printf("1.Increase Volume 2.Decrease Volume 3.Change Password\n\n");
        int select=rand()%4;

        switch(select){

          case 1:{
              printf("Volume of Shaktimaan before changing is %d\n",volume);
              int inc=rand()%20;
              volume=ADD(volume,inc);
              printf("Now Volume of Shaktimaan is %d\n",volume);
              break;
          }
          case 2:{
              printf("Volume of Shaktimaan before changing is %d\n",volume);
              int dec=rand()%20;
              volume=SUB(volume,dec);
              printf("Now Volume of Shaktimaan is %d\n",volume);
              break;
          }
          case 3:{
              printf("Password of Shaktimaan before changing is %d\n",password);
              int change=rand()%20;
              password=change;
              printf("Password is changed -- NEW PASSWORD: %d\n", password);
              break;
          }
          default:
              printf("Wrong choice\n");
        }
    }
    else 
      printf("You are not Mark\n You can't change settings");
}
void INFO(){

  printf("\n\n-----------********-----------------INFO-----------------------------------------********----------------------------\n\n");
  int choice;
  printf("Enter your question\n1.What is currentTime\n2.What is name of device\n");
  scanf("%d",&choice);

  switch(choice){

    case 1:printf("CurrentTime is %d hours\n", rand()%24);
        break;
    case 2:printf("Your device name is Shaktimaan\n");
        break;
    default:printf("wrong choice\n");
  }
}


void SETREMINDER(int RegIndex){

  printf("\n\n--------------------------***********---------------------SETREMINDER------------------**********-----------------\n\n");

  int time=Register[RegIndex];
  printf("Reminder is set On time %d",time);
  Reminders[Count]=time;
  Count++;

}

void REMINDER(){

  printf("\n\n--------------------------***********------------------------REMINDER------------------**********-----------------\n\n");
  int i;


  for(i=1;i<Count;i++){
      int time=Reminders[i];
      if(cmp(time,8)==1){
        printf("Reminder for Breakfast\n");
      }

      else if(cmp(time,20)==1){
        printf("Reminder for Dinner\n");
      }

      else if(cmp(time,17)==1){
        printf("Reminder for Snacks\n");
      }

      else if(cmp(time,13)==1){
        printf("Reminder for Lunch\n");
      }


      else if(cmp(time,9)==1){
        printf("Reminder for COA LECTURE\n");
      }
      else 
        printf("Reminder for Sleep\n");
  }

}

void WATERCLOSET(int RegIndex){

    printf("------------------------------***********-------WATERCLOSET-------------------**********---------------------------\n\n");
    int time=Register[RegIndex];

    printf("Set the temperature of Water according to Weather at Time %d\n",time);
    int temp=rand()%100;

    if(cmp(temp,25)==2)
    {
      printf("Decrease the temp of water\n");
      temp=SUB(temp,temp-25);
    }
    else if(cmp(temp,20)==3){
      printf("Increase the temp of Water\n");
      temp=ADD(temp,25-temp);
    }


}

void STORE(int binary_operand1, int binary_operand2){};
void LOAD(int binary_operand1, int binary_operand2){};

void LIGHT(int RegIndex,int RegIndex1){

  printf("\n\n----------------------------------********----------------LIGHT-----------------------*******-----------------------------\n\n");

  int roomNo=Register[RegIndex];
  int on_off=Register[RegIndex1];

  if(cmp(on_off,1)==1){
    if(Status & (1<< roomNo)){
      printf("LIGHT is already On");
    }

    else{
      printf("TURNING ON THE LIGHTS FOR ROOM NO %d",roomNo);
      Status |= (1 << roomNo);
    }
  }
  else{
    if( !(Status && (1 << roomNo)) ){
      printf("LIGHT is already Off");
    }
    else{
      printf("TURNING OFF THE LIGHTS FOR ROOM NO %d",roomNo);
      Status ^= (1 << roomNo);
    }
  }

  printf("\n");

  return ;
}

void DOOR(int RegIndex){

  printf("\n\n----------------------------------********----------------DOOR-----------------------*******-----------------------------\n\n");
  int password=Register[RegIndex];

  if(cmp(password,1)==1){
    printf("Hii Mark Your Dad has come home\n");
  }
  else if(cmp(password,2)==1){
    printf("Hii Mark Your Mom has come home\n");
  }
  else if(cmp(password,3)==1){
    printf("Hii Mark Your Son Jarvis has come home\n");
  }
  else{
    printf("Hii Mark A stranger is at the Door\n Go and Recieve him\n");
  } 
}

void WEATHERFORECAST(int RegIndex){

    printf("\n\n--------------------------******---------------WEATHERFORECAST-------------------******------------------------\n\n");
    int CurrentTime=Register[RegIndex];

    if(cmp(CurrentTime,8)==3){
      printf("Early Morning");
    }

    else if(cmp(CurrentTime,8)==2 && cmp(CurrentTime,15)==3){
      printf("Sunny Day");
    }
    else if(cmp(CurrentTime,15)==2 && cmp(CurrentTime,20)==3){
      printf("Good Evening");
    }
    else if(cmp(CurrentTime,20)==2 && cmp(CurrentTime,24)==3){
      printf("Dark Night");
    } else{
      printf("Wrong Time Format");
    }

    printf("\n");
}

void HVAC(int RegIndex,int RegIndex1){

  printf("\n\n--------------------------------------------------HVAC-----------------------------------------------------------\n\n");

  int type=Register[RegIndex];

  //1-------HEAT-----//
  if(cmp(type,1) == 1){
    printf("You Want to control TEMPERTURE\n");
    int celsius=rand()%40;
    
    int fahrenheit=ADD(DIV(MUL(celsius,9),5),32);
    
    printf("In ROOM NO %d\n",Register[RegIndex1]);
    printf("TEMPERTURE IN CELSIUS %dC\n",celsius);
    printf("TEMPERTURE IN FAHRENHEIT %dF\n",fahrenheit);

    if(cmp(celsius,30)==2){
        printf("It's too hot\n");
        printf("Switch On A.C.\n");
    }
    else if(cmp(celsius,10)==3){
      printf("It's too cold\n");
      printf("Switch On heater\n");
    } else{
      printf("Normal Temperture\n");
    }
  }//-------VENTILATION---------//
  else if(cmp(type,2) == 1){
      printf("Room is very dusty\n");
      printf("Ventilation is On in Room No %d\n",Register[RegIndex1]);
  }//--------AC--------//
  else if(cmp(type,3) == 1){
      printf("It's too HOT\n");
      printf("AC is ON in Room No %d\n",Register[RegIndex1]);
  }

}


void ENTERTAINMENT(){

  printf("\n\n-----------********-----------------ENTERTAINMENT-----------------------------------------********----------------------------\n\n");

  int choice,select;
  printf("What you want to watch 1.Movie 2.Season 3.Song\n");
  scanf("%d",&choice);

  switch(choice){

    case 1:printf("Select Movie\n 1.Sultan 2.Dangal 3.Happy New Year 4.Krish\n");
        
        scanf("%d",&select);
        switch(select){
          case 1:printf("Yor are now watching Movie Sultan\n");
              break;
          case 2:printf("Yor are now watching Movie Dangal\n");
              break;
          case 3:printf("Yor are now watching Movie Happy New Year\n");
              break;
          case 4:printf("Yor are now watching Movie Krish\n");
              break;
          default:printf("Wrong choice\n");
              break;
        }
        break;
    case 2:printf("Select Season\n 1.F.R.I.E.N.D.S \n2.Big Boss \n3.Roadies\n");
      
        scanf("%d",&select);
        switch(select){
          case 1:printf("Yor are now watching F.R.I.E.N.D.S\n");
              break;
          case 2:printf("Yor are now watching Big Boss\n");
              break;
          case 3:printf("Yor are now watching Roadies\n");
              break;
          default:printf("Wrong choice");
              break;
        }
        break;
    case 3:printf("Select Songs\n 1.Let Me Love You \n 2.Shape of You \n 3.We don't talk anymore\n 4.Closer\n 5.This is What You came for\n");
        
        scanf("%d",&select);
        switch(select){
          case 1:printf("Enjoy Song Let Me Love You\n");
              break;
          case 2:printf("Enjoy Song Shape of You\n");
              break;
          case 3:printf("Enjoy Song We don't talk anymore\n");
              break;
          case 4:printf("Enjoy Song Closer\n");
              break;
          case 5:printf("Enjoy Song This is What You came for\n");
              break;
          default:printf("Wrong choice");
              break;
        }
        break;
    default:printf("wrong choice");
  }
}

//-----------------------CONTROL SIGNAL-----------------------------------//

int cmp(int a, int b){

  printf("ALU Control signal for CMP 000 is activated\n");

  if(a==b)
    return 1;
  else if(a>b)
    return 2;
  else 
    return 3;
}

int ADD(int a,int b){
  printf("ALU Control Signal for ADD 001 is activated\n");
  return a+b;
}


int SUB(int a,int b){
  printf("ALU Control Signal for SUB 002 is activated\n");
  return a-b;

}
int MUL(int a,int b){

  printf("ALU Control Signal for MUL 003 is activated\n");
  return a*b;

}

int DIV(int a,int b){
  printf("ALU Control Signal for DIV 004 is activated\n");
  return a/b;

}

void HALT(){
  printf("------------------------------************----------HALT---------------*******************----------------------\n\n");
  displayCPURegister();
  exit(0);
}

void MOVE(int RegIndex1,int RegIndex2){
  printf("Move data from Register %d to Register %d\n", RegIndex1, RegIndex2);
  Register[RegIndex2]=Register[RegIndex1];

}

void IncrementPC(){
//  printf("Add is selected from ALU\n One input on ALU is 1 and One input is PC \n 1 is added to PC \n Now PC=PC+1\n");
  ProgramCounter+=1;
}


void displayCPURegister(){

  printf("\n\n-------------------****----------------------------------CPU Register-----------------****-----------------------\n\n");
  int i;
  printf("General purpose Register\n");
  //FILE* RegisterRead=fopen("RegisterValue.txt",)
  for(i=0;i<27;i++){
    printf("R%d: %d\t",i,Register[i]);
  }

  printf("\nSpecial purpose Regsiter");
  printf("\nPC: %d\t",ProgramCounter);

  printf("IR: %s\t", IR);

  printf("SP: %d\t",StackPointer);

  printf("TIME: %d\t",Register[28]);
  printf("Status = %d\t", Status);
  printf("LIGHT1 FLAG: %d\t", ( Status & (1 << 1) ? 1 : 0 ) );
  printf("LIGHT2 FLAG: %d\t", ( Status & (1 << 2) ? 1 : 0 ) );
  printf("LIGHT3 FLAG: %d\t", ( Status & (1 << 3) ? 1 : 0 ) );
  printf("LIGHT4 FLAG: %d\t", ( Status & (1 << 4) ? 1 : 0 ) );
  printf("DOOR FLAG: %d\t", ( Status & (1 << 5) ? 1 : 0 ) );
  printf("EMERGENCY FLAG: %d\n", ( Status & (1 << 6) ? 1 : 0 ) );

  IncrementPC();

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Register[32];

void makeSymbolTable();
int convertToInt(char* str);
int convertToIntBase2(char* str);
int FindSymbolAddress(char checkSymbol[100]);
void searchOpcodeRepresentation(char instruction[100], char binary_instruction[100]);
void searchRegisterRepresentation(char operand[100], char binary_operand[100]);
int checkEmptyRegister();
void convertToString(char operand[100], int num);
void initializeRegister();
void convertInstructionToBinary();
void writeRegisterInFile();

int main(){
  initializeRegister();
  
  makeSymbolTable();

  convertInstructionToBinary();

  writeRegisterInFile();

  return 0;
}

//writing register in a file
void writeRegisterInFile(){
  int i;

  FILE* writeRegister = fopen("RegisterValue.txt", "w");

  for(i=0; i<=26; i++){
    fprintf(writeRegister, "R%d: %d\n", i, Register[i]);
  }

  fclose(writeRegister);

  return ;
}


//checks empty register to be used for immediate addressing
int checkEmptyRegister(){
  int i;

  for(i=26; i>=0; i--){
    if(Register[i] == -1)
      return i;
  }
  return -1;
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


//initialize register i.e. each register is free to be used
void initializeRegister(){
  int i;
  
  for(i=0; i<32; i++)
    Register[i]=-1;

  return ;
}



//main function which converts instruction into binary code
void convertInstructionToBinary(){

  FILE* input = fopen("input.txt", "r");

  FILE* writeBinary = fopen("binary_code.txt", "w");

  char line[100], instruction[100], operand1[100], operand2[100];
  char binary_operand1[100], binary_operand2[100], binary_instruction[100];

  int prev, count, i, j;

  while (fgets(line, 100, input)) {
    
    count=0;
    prev=0;

    int length = strlen(line);

    for(i=0; i<length; i++){

      //checks for space or if its last word and then process each of it to convert into 
      //equivalent binary code
      if(line[i]==' ' || i==length-1){
        if(count==0){
          strncpy(instruction, line+prev , i-prev);
          
          instruction[i-prev]='\0';

          //if its a lable
          if(FindSymbolAddress(instruction) != -1)
            break;

          //takes the substring for opcode and converts into binary code
          searchOpcodeRepresentation(instruction, binary_instruction);

          //prints the binary code into output file
          for(j=0; j<strlen(binary_instruction); j++)
            fprintf(writeBinary, "%c", binary_instruction[j]);
          
          prev = i+1;
          
          //count signifies instruction format
          if(strlen(binary_instruction) == 6)
            count=2;
          else if(strlen(binary_instruction) == 11)
            count=1;
          
          //converting first operand        
        } else if( ( (count==2 && i != length-1) || count==1 ) ){
        
          //handling immediate addressing mode
          if(line[prev]=='#'){
            strncpy(operand1, line+prev+1 , i-prev-1);

            operand1[i-prev-1] = '\0';

            int emptyRegister = checkEmptyRegister();
            
            Register[emptyRegister] = convertToInt(operand1);

            convertToString(operand1, emptyRegister);
            
          } else{
            strncpy(operand1, line+prev , i-prev);

            operand1[i-prev] = '\0';

            //if its lable then find its equivalent address and stores it in a register

            int symbolAddress=FindSymbolAddress(operand1);     

            if(symbolAddress!=-1){
              int emptyRegister=checkEmptyRegister();
              Register[emptyRegister] = symbolAddress;
              convertToString(operand1,emptyRegister);
            }
          }

          prev = i+1;
          //search for equivalent binary code
          searchRegisterRepresentation(operand1, binary_operand1);
          
          //Register[convertToIntBase2(binary_operand1)]=0;

          for(j=1; j<strlen(binary_operand1)-1; j++)
            fprintf(writeBinary, "%c", binary_operand1[j]);

          //2nd operand
        } else if(count==2 && i == length-1){
        
          if(line[prev]=='#'){
            strncpy(operand2, line+prev+1 , i-prev-1);

            operand2[i-prev-1] = '\0';

            int emptyRegister = checkEmptyRegister();
            
            Register[emptyRegister] = convertToInt(operand2);

            convertToString(operand2, emptyRegister);
          } else{
            strncpy(operand2, line+prev , i-prev);

            operand2[i-prev] = '\0';

            int symbolAddress=FindSymbolAddress(operand2);     

            if(symbolAddress!=-1){
              int emptyRegister=checkEmptyRegister();
              Register[emptyRegister] = symbolAddress;
              convertToString(operand2,emptyRegister);
            }

          }
          searchRegisterRepresentation(operand2, binary_operand2);

        //  Register[convertToIntBase2(binary_operand2)]=0;

          for(j=1; j<strlen(binary_operand2)-1; j++)
            fprintf(writeBinary, "%c", binary_operand2[j]);
          
        }
    
      }

    }  
    if(FindSymbolAddress(instruction) == -1)
      fprintf(writeBinary, "\n");
  }

  fclose(input);
  fclose(writeBinary);

}


//This functions reads from the input file i.e. having mnemonic code and then 
//convert them to symbol table 
void makeSymbolTable(){
  
  //opening file to read and write
  FILE* input = fopen("input.txt", "r");
  FILE* symbolTableWrite = fopen("symbol_table.txt", "w");

  char line[100];
  int lineCount=0;
  
  //loop runs till the end of file
  while (!feof(input)) {
    lineCount++;

    fgets(line, 100, input);

    int length = strlen(line);

    //loop which checks for lables to insert them into symbol table
    for(int i=0; i<length; i++){
      if(line[i]==':'){
        line[i]='\0';
        fprintf(symbolTableWrite, "%s: %d\n", line, lineCount);
      }
    }
  }

  fclose(input);
  fclose(symbolTableWrite);
}

//convert string to decimal
int convertToInt(char* str)
{
    int res = 0; // Initialize result
 
    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
      res = res*10 + str[i] - '0';
 
    // return result.
    return res;
}

//convert string to decimal
int convertToIntBase2(char* str)
{
    int res = 0, length=strlen(str), value=1; // Initialize result
 
    // Iterate through all characters of input string and
    // update result
    for (int i = length-1; i >= 0 ; i--){
      res = res + (str[i] - '0')*value ;
      value *= 2;
    }
      
    // return result.
    return res;
}


//function for finding symbol address from the symbol table
int FindSymbolAddress(char checkSymbol[100]){

  FILE* SymbolRead=fopen("symbol_table.txt","r");
  char line[100], symbolAddress[100];
  int i, length, address=-1, found=0;

  while(!feof(SymbolRead)){

      fgets(line,100,SymbolRead);
      length = strlen(line);

      for(i=0;i<length;i++){

        if(line[i]==':'){
          strncpy(symbolAddress,line+i+2,length);
          address=atoi(symbolAddress);
          found=1;
          break;
        }

        if(line[i]!=checkSymbol[i]){
          break;
        }
      }
  }

  if(!found){
    address = -1;
  }
  
  fclose(SymbolRead);      

  return address;
}


//This function searches for the given opcode in the opcode.txt file and returns the equivalent
//binary code
void searchOpcodeRepresentation(char instruction[100], char binary_instruction[100]){

  FILE* opcodeRead = fopen("opcode.txt", "r");
  char line[100];
  int i, length;
  int found=0;
  while (!feof(opcodeRead)) {

    fgets(line, 100, opcodeRead);

    length = strlen(line); 
    for(i=0; i<length; i++){
      if(line[i]==' '){
        strncpy(binary_instruction, line+i+1, length-i-2);
        binary_instruction[length-i-2]='\0';
        found=1;
        break;
      }
      
      if(line[i]!=instruction[i]){
        break;
      }
    }
  }
  //if no equivalent binary code is found it means its wrong opcode
  if(!found){
    printf("Wrong Opcode!!\nWrond Mnemonic Code!!\n");
    exit(0);
  }

  fclose(opcodeRead);

  return;
}

//searches the equivalent register binary code in register.txt
void searchRegisterRepresentation(char operand[100], char binary_operand[100]){

  FILE* registerRead = fopen("register.txt", "r");
  char line[100];
  int length, i;
  int found=0;

  while (!feof(registerRead)) {

    fgets(line, 100, registerRead);

    length = strlen(line);

    for(i=0; i<length; i++){

      if(line[i]==' '){
        strncpy(binary_operand, line+i, length-i);
        binary_operand[length-i]='\0';
        found=1;
        break;
      }
      
      if(line[i]!=operand[i]){
        break;
      }
    }
  }

  //if no equivalent register is not found
  if(!found){
    printf("No Such Register Found!!\nWrond Mnemonic Code!!\n");
    exit(0);
  }

  fclose(registerRead);

  return;
}

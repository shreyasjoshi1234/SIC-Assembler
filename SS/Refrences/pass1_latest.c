// Updating with OPTAB search

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXOP 10 //Maximum opcodes present in OPTAB

static int start_flag = 0;

// Structure to hold components of instruction
struct instruction
{
    char *label;
    char *opcode;
    char *operand;
};

//Structure to hold OPTAB instruction
struct optabcode
{
    char *opcode;
    int objectcode;
};

// Function to display an instruction on to console
void displayInstruction(struct instruction inst)
{
    printf("%s %s %s\n",inst.label,inst.opcode,inst.operand);
}

// Function to display an instruction



// Function to tokenize instruction from a line (string) and return structure
struct instruction tokenizeInstruction(char line[])
{
    int count=0;
    struct instruction inst;
    char *token1=NULL;
    char *token2=NULL;
    char *token3=NULL;

    token1=strtok(line," ");
    token2=strtok(NULL," ");
    token3=strtok(NULL," ");

    if(token2==NULL)
        count=1;

    if(token3==NULL && token2!=NULL)
        count=2;
    
    if(token3!=NULL)
        count=3;

    if(count==1)
    {
        inst.label="";
        inst.opcode=token1;
        inst.operand="";
    }

    if(count==2)
    {
        inst.label="";
        inst.opcode=token1;
        inst.operand=token2;
    }

    if(count==3)
    {
        inst.label=token1;
        inst.opcode=token2;
        inst.operand=token3;
    }
    return inst;
}

struct optabcode tokenizeOpcode(char line[])
{
    struct optabcode temp;

    char *token1=NULL;
    char *token2=NULL;

    temp.opcode=strtok(line," ");
    token2=strtok(NULL," ");

    sscanf(token2,"%X",&temp.objectcode);

    return temp;
}

/* Function returns number of entries in OPTABLE after reading from file 
and storing objectcode in Structure Array*/

int getOPTAB(FILE *fp4,char opcodearray[][MAXOP],int codearray[])
{
    char temp;  // Temporary character variable
    char line[30];  // Temporary string to hold a line/instruction
    struct optabcode temp_optab;
    int i=0;
    int j=0;

    while(!feof(fp4))
    {
        // Read a character
        fscanf(fp4,"%c",&temp);
        
        // Put it in a character array
        line[i++]=temp;
        if(temp=='\n')
        {
            line[--i]='\0';  // Adding null terminator
            i=0;

            temp_optab = tokenizeOpcode(line);

            strcpy(opcodearray[j],temp_optab.opcode);

            codearray[j] = temp_optab.objectcode;
            j++;
        }
    }

    line[--i]='\0';
    temp_optab = tokenizeOpcode(line);

    strcpy(opcodearray[j],temp_optab.opcode);
    codearray[j] = temp_optab.objectcode;
    j++;

    // returns number of entries in OPTABLE
    return j;
}




// Returning Address as Hexadecimal integer even though in struct it is a string?
void getAddress(struct instruction inst,int *address)
{
    static int read=0;  // read flag is initially 0, after reading address it is set to 1

    if(!strcmp(inst.opcode,"Start")||!strcmp(inst.opcode,"start")||!strcmp(inst.opcode,"START"))
    {
        if(inst.operand==NULL){
            *address=0;
            read=1;
            //printf("address = %d\n",address);
        }

        if(inst.operand!=NULL)
        {
            //*address=atoi(inst.operand);
            sscanf(inst.operand,"%X",address);
            read=1;
            start_flag=1;
        }
    }
    if(read==0)
    {
        *address=0;
        read=1;
    }
}

void updateAddress(int *address,struct instruction temp_inst,char opcodearray[][MAXOP],int codearray[])
{
    int hex_operand;
    if(!strcmp(temp_inst.opcode,"WORD")||!strcmp(temp_inst.opcode,"Word")||!strcmp(temp_inst.opcode,"word"))
    {
        *address = *address+3;

        return;
    }

    else if(!strcmp(temp_inst.opcode,"RESW")||!strcmp(temp_inst.opcode,"Resw")||!strcmp(temp_inst.opcode,"resw"))
    {
        sscanf(temp_inst.operand,"%d",&hex_operand);
        *address = *address+ (3*hex_operand);

        return;
    }

    else if(!strcmp(temp_inst.opcode,"RESB")||!strcmp(temp_inst.opcode,"Resb")||!strcmp(temp_inst.opcode,"resb"))
    {
        sscanf(temp_inst.operand,"%d",&hex_operand);
        *address = *address + (1*hex_operand);

        return;
    }

    else if(!strcmp(temp_inst.opcode,"BYTE")||!strcmp(temp_inst.opcode,"Byte")||!strcmp(temp_inst.opcode,"byte"))
    {
        int count=0;
        int i=2;    // characters start at 2nd index inside C'sample' or X'sample'
        while(temp_inst.operand[i]!=39)
        {
            
            count++;
            i++;
        }
        
        hex_operand = count;


        if(temp_inst.operand[0]=='X'||temp_inst.operand[0]=='x')
        {
            *address = *address + (0.5*hex_operand);
        }

        else if(temp_inst.operand[0]=='C'||temp_inst.operand[0]=='c')
        {
            *address = *address + (1*hex_operand);
        }

        return;
        
    }

    // Else search opcode in OPTAB
    int opcode_found = 0;   // set to 1 if opcode found in OPTAB, else 0

    for(int i=0;i<MAXOP;i++)
    {
        if(!strcmp(temp_inst.opcode,opcodearray[i]))
        {
            opcode_found = 1;
            break;
        }
    }

    
    if(opcode_found == 1)
    {
        *address = *address + 3;

        return;
    }

    else
    {
        printf("\nERROR: opcode %s not found in OPTABLE\n",temp_inst.opcode);
        exit(6);
    }
    

}

void addSYMTAB(int *address,FILE *fp3, struct instruction inst)
{
    // Check if label already exists

    rewind(fp3);  // Take file pointer to Start of File
    
    char temp;  // Temporary character variable
    char line[30];  // Temporary string to hold a line/instruction
    int i=0;

    while(!feof(fp3))
    {
        // Read a character
        fscanf(fp3,"%c",&temp);
        
        // Put it in a character array
        line[i]=temp;

        // Increment string index
        i++;

        if(temp==' ')
        {
            line[--i]='\0'; // Adding null terminator
            i=0;

            // Check if label names are same !strcmp(*temp_inst.opcode,*inst.label)
            if(!strcmp(inst.label,line))//line contains symbol like Str1
            {
                printf("\nERROR: Label %s is repeated\n",inst.label);
                exit(5);
            }
        }

        if(temp=='\n')
        {
            i=0;
        }
    }

    // If code gets executed till here then
    // 1. The label we want to insert is a new one and does not exist in the SYMTAB
    // 2. FILE pointer fp3 has reached EOF

    // Adding new label in SYMTAB
    fprintf(fp3,"%s %x\n",inst.label,*address);

    return;
}

// This function is called first
/* Reads first line expecting "Start" opcode. Address is updated in LOCCTR.
Line counter is incremented and Character counter i is used and reset.
*/
int ProcessFirstLine(FILE *fp1,FILE *fp2,FILE *fp3,struct instruction temp_inst,int *address,char opcodearray[][MAXOP],int codearray[])
{
    char temp;
    char line[200];
    int i=0;
    int linecount=0;

    while(!feof(fp1)){
        
        // Read a character
        fscanf(fp1,"%c",&temp);
    
        // Put it in a character array
        line[i]=temp;

        // Increment string index
        i++;

        if(temp=='\n')
        {
            // string has been read till '\n' so string is "example\n"
            // But we need one line in one string variable - "example\0"
            line[--i]='\0'; // Adding null terminator
            i=0;

            // Empty Lines
            if(line[0]=='\0')
            {
                continue;
            }

            // Comment Lines
            if(line[0]=='.')
            {
                printf("%s\n",line);
                fprintf(fp2,"%s\n",line);
                continue;
            }
            
            temp_inst = tokenizeInstruction(line);

            getAddress(temp_inst,address);

            displayInstruction(temp_inst);
            if(start_flag==1){
                // Don't call Update Address or addsymboltable
                fprintf(fp2," %s %s %s\n",temp_inst.label,temp_inst.opcode,temp_inst.operand);
            }
            else{
                // Symbol Table
                if(temp_inst.label!="")
                {
                    addSYMTAB(address,fp3,temp_inst);
                }
                fprintf(fp2,"%x %s %s %s\n",*address,temp_inst.label,temp_inst.opcode,temp_inst.operand);
                
                // Update Address
                updateAddress(address,temp_inst,opcodearray,codearray);
            }
            linecount++;
            return linecount;  // Break/return from the loop after reading first line
        }
    }
    return 1;
}

int ProcessLines(FILE *fp1,FILE *fp2,FILE *fp3,struct instruction temp_inst,int *address,char opcodearray[][MAXOP],int codearray[])
{
    char temp;  // Temporary character variable
    char line[30];  // Temporary string to hold a line/instruction
    int i=0;
    int linecount = 0;

    while(!feof(fp1))
    {
        // Read a character
        fscanf(fp1,"%c",&temp);
        
        // Put it in a character array
        line[i++]=temp;

        

        // If the read character is '\n' then we have completed reading a line
        if(temp=='\n')
        {
            // string has been read till '\n' so string is "example\n"
            // But we need one line in one string variable - "example\0"
            line[--i]='\0'; // Adding null terminator
            i=0;

            if(line[0]=='\0')
            {
                continue;
            }

            // Comment Lines; directly write into file
            if(line[0]=='.')
            {
                printf("%s\n",line);
                fprintf(fp2,"%s\n",line);
                continue;
            }
            
            temp_inst = tokenizeInstruction(line);

            if(!strcmp(temp_inst.opcode,"End"))
            {
                break;
            }

            displayInstruction(temp_inst);
            
            // Symbol Table
            if(temp_inst.label!="")
            {
                addSYMTAB(address,fp3,temp_inst);
            }

            fprintf(fp2,"%x %s %s %s\n",*address,temp_inst.label,temp_inst.opcode,temp_inst.operand);
            
            updateAddress(address,temp_inst,opcodearray,codearray);

            linecount++;
        }
    }
    
    // Last Line
    line[--i]='\0';
    temp_inst = tokenizeInstruction(line);
    
    displayInstruction(temp_inst);
    
    linecount++;

    if(!strcmp(temp_inst.opcode,"End"))
    {
        fprintf(fp2,"%x %s %s %s\n",*address,temp_inst.label,temp_inst.opcode,temp_inst.operand);
        return linecount;
    }

    // Symbol Table
    if(temp_inst.label!="")
    {
        addSYMTAB(address,fp3,temp_inst);
    }

    fprintf(fp2,"%x %s %s %s\n",*address,temp_inst.label,temp_inst.opcode,temp_inst.operand);
    updateAddress(address,temp_inst,opcodearray,codearray);

    return linecount;
}




int main()
{
    //File pointers
    FILE *fp1;  // Read file pointer
    FILE *fp2;  // Write file pointer
    FILE *fp3;  // SYMTAB file pointer
    FILE *fp4;  // OPTAB file pointer

    // Opening all files
    fp1=fopen("input.txt","r");
    fp2=fopen("output.txt","w");
    fp3=fopen("SYMTAB.txt","w+");
    fp4=fopen("OPTAB.txt","r");
    

    // File opening Exceptions
    if(fp1==NULL){
        printf("  => Error opening the read file\n\n");
        return 1;
    }
    if(fp2==NULL){
        printf("  => Error opening the write file\n\n");
        return 1;
    }
    if(fp3==NULL){
        printf("  => Error opening the Symbol Table file\n\n");
        return 1;
    }
    if(fp4==NULL){
        printf("  => Error opening the Opcode Table file\n\n");
        return 1;
    }

    printf("  ------AllFiles opened----- \n\n");
    int optable_size;
    char opcodearray[5][MAXOP]; // Array of strings to hold opcode names
    int codearray[MAXOP];   // Array of int to hold corresponding object codes

    //Read the OPTAB and store data into the array
    optable_size = getOPTAB(fp4,opcodearray,codearray);
    printf("\nSize of OPTAB is = %d\n\n",optable_size);


    printf("---------------Read file---------- \n");

    struct instruction temp_inst;
    int starting_address;
    int address;
    int program_length;
    int linecount;

    
    // Process the first line 
    linecount = ProcessFirstLine(fp1,fp2,fp3,temp_inst,&address,opcodearray,codearray);
    starting_address = address;
    //If there is no start then starting address is 0
    if(start_flag==0)
    {
        starting_address=0;
    }
    

    // Process remaining Lines
    linecount += ProcessLines(fp1,fp2,fp3,temp_inst,&address,opcodearray,codearray);

    program_length = address-starting_address;

    printf("\nStarting Address = Hex: %x, Decimal: %d\n",starting_address,starting_address);
    printf("Address = Hex: %x, Decimal: %d\n",address,address);
    printf("Program Length = Hex: %x, Decimal: %d\n",program_length,program_length);

    printf("\n\n--------------------------\n");
    printf("\nNumber of lines is %d\n",linecount);
    

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);


    printf("\n  ---------All files closed------------ \n\n");

    
    return 0; // Execution Completed Successfully here
}
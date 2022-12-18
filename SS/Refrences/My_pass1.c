#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXOP 100

int start_f=0;


void displayOPTAB(char opcodearray[][MAXOP],int codearray[]);
struct optabcode tokenizeOpcode(char line[]);
void updateAddress(int *address,struct instruction temp_inst,char opcodearray[][MAXOP],int codearray[]);
int getOPTAB(FILE *fp4,char opcodearray[][MAXOP],int codearray[]);
void getsddress(struct instruction temp_inst,int *address);struct instruction tokenizeInstruction(char line[]);
struct instruction tokenizeInstruction(char line[]);
void displayInstruction(struct instruction inst);
void displayOPTAB(char opcodearray[][MAXOP],int codearray[]);
int ProcessFirstLine(FILE *fp1,FILE *fp2,int *address,char opcodearray[][MAXOP],int codearray);



struct optabcode
{
    char *opcode;
    int objectcode;
};

struct instruction{
    char *label;
    char *opcode;
    char *operand;
};

void displayOPTAB(char opcodearray[][MAXOP],int codearray[])
{
    printf("\nOPTAB: \n");
    for(int i=0;i<MAXOP;i++)
    {
        printf("\n%s %d",opcodearray[i],codearray[i]);
    }
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
        sscanf(temp_inst.operand,"%X",&hex_operand);
        *address = *address+ (3*hex_operand);

        return;
    }

    else if(!strcmp(temp_inst.opcode,"RESB")||!strcmp(temp_inst.opcode,"Resb")||!strcmp(temp_inst.opcode,"resb"))
    {
        sscanf(temp_inst.operand,"%X",&hex_operand);
        *address = *address + (1*hex_operand);

        return;
    }

    else if(!strcmp(temp_inst.opcode,"BYTE")||!strcmp(temp_inst.opcode,"Byte")||!strcmp(temp_inst.opcode,"byte"))
    {
        int count=0;
        int i=2;    // characters start at 2nd index inside C'sample' or X'sample'
        while(temp_inst.operand[i]!=39)
        {
            //printf("\n%c",temp_inst.operand[i]);
            count++;
            i++;
        }
        
        hex_operand = count;
        //printf("\nhex_operand = %d\n",hex_operand);
        //sscanf(&count,"%X",&hex_operand);

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

void displayInstruction(struct instruction inst)
{
    printf("%s %s %s",inst.label,inst.opcode,inst.operand);
}

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
        // if(!strcmp(inst.label,"(null)"))
        // {
        //     printf("\nScheme\n");
        // }
        inst.opcode=token2;
        inst.operand=token3;
    }
    return inst;
}

void getsddress(struct instruction temp_inst,int *address)
{
    *address=0;
    if((strcmp("Start",temp_inst.opcode)==0 || strcmp("START",temp_inst.opcode)==0|| strcmp("start",temp_inst.opcode)==0))
    {
        sscanf(temp_inst.operand,"%x",address);
        start_f=1;
        
    }


}
int ProcessFirstLine(FILE *fp1,FILE *fp2,int *address,char opcodearray[][MAXOP],int codearray)
{
    struct instruction temp_inst;
    char line[31];
    int linecount=0;
    while(!feof(fp1))
    {
        fgets(line,30,fp1);
        if(line[strlen(line)-1=='\n'])
            line[strlen(line)-1]='\0';
            if(line[0]=='.')
            {
                printf("%s\n",line);
                fprintf(fp2,"%s\n",line);
                continue;
            }
            temp_inst=tokenizeInstruction(line);
            getaddress(temp_inst,address);
            // sscanf() ------------------------
            displayInstruction(temp_inst);
            if(start_f==1)
            {
                fprintf(fp2,"%s %s %s",temp_inst.label,temp_inst.opcode,temp_inst.operand);
            }
            else{
                fprintf(fp2,"%x%s %s %s",address,temp_inst.label,temp_inst.opcode,temp_inst.operand);
                    updateAddress(address,temp_inst,opcodearray,codearray);
                    // symbol tabel

            }
    linecount++;
    return linecount;

    }
} 


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


int main()
{
    FILE *fp1;  // Read file pointer
    FILE *fp2;  // Write file pointer
    FILE *fp3;  // SYMTAB file pointer
    FILE *fp4;  // OPTAB file pointer

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
        return 2;
    }
    if(fp3==NULL){
        printf("  => Error opening the Symbol Table file\n\n");
        return 3;
    }
    if(fp4==NULL){
        printf("  => Error opening the Opcode Table file\n\n");
        return 4;
    }

    printf("  ------All files opened------ \n\n");

    int optable_size;
    char opcodearray[MAXOP][MAXOP]; // Array of strings to hold operation names
    int codearray[MAXOP];   // Array of int to hold corresponding object codes
    struct instruction temp_inst;
    int starting_address;
    int address;
    int program_length;
    int linecount;

    int x=getOPTAB(fp4,opcodearray,codearray);
    displayOPTAB(opcodearray,codearray);
    linecount=ProcessFirstLine(fp1,fp2,&address,opcodearray,codearray);
    if(start_f==0)
    {
        starting_address=0;
    }
    starting_address=address;
    
    
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);



    printf("\n  -Files closed- \n\n");

    return 0; // Execution Completed Successfully
}
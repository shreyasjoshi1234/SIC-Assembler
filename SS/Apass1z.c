#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXOP 100

static int start_flag=0;

int getOPTAB(FILE *fp4,char opcodearray[][MAXOP],int codearray[]);
void  ProcessFirstLine(FILE *fp1,FILE *fp2,FILE *fp3,int *address,char opcodearray [][MAXOP],int codearray[]);
void displayOPTAB(char opcodearray[][MAXOP],int codearray[],int n);
void processNextLines(FILE *fp1,FILE *fp2,FILE *fp3,int *address,char opcodearray[][MAXOP],int codearray[]);
// void  addSymbol(FILE *fp3,struct instruction inst, int *address);
// void updateAddress(int *address,struct instruction temp_inst,char opcodearray[][MAXOP],int codearray[]);
// void  addSymbol(FILE *fp3,struct instruction tinst, int *address);
struct instruction tokanizeopcode(char[]);

struct instruction
{
    char *label;
    char *opcode;
    char *operand;
};


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
        
    }
}

void  addSymbol(FILE *fp3,struct instruction tinst, int *address,int lc)
{
    rewind(fp3);
    int flag=0;
    char symbol[MAXOP];
    int symboladdress;
    while(!feof(fp3))
    {
        fscanf(fp3,"%s %x",symbol,&symboladdress);
        if(strcmp(symbol,tinst.label)==0)
        {
            flag=1;
            break;
        }
    }

    if(flag)
    {
        printf("\n-----ERROR Label %s is repeated in line %d-----",tinst.label,lc);
    }
    else{
        fprintf(fp3,"%s %x\n",tinst.label,*address);
    }
}

void processNextLines(FILE *fp1,FILE *fp2,FILE *fp3,int *address,char opcodearray[][MAXOP],int codearray[])
{
    int lc=1;
    char line[31];
    struct instruction temp_inst;
    while(!feof(fp1))
    {
        fgets(line,30,fp1);
        lc++;

        if(line[strlen(line)-1]=='\n')
            line[strlen(line)-1]='\0';

        if(strcmp(line,"End")==0)
        {
            fprintf(fp2,"%x %s\n",*address,line);
        }
        
        if(line[0]=='\0')
        {
            //Empty line inbetween the code are ignored
            continue;
        }
    
        if(line[0]=='.')
        {
            fprintf(fp2,"%s\n",line);
            continue;
            //Comment line write into intermediate file and do nothing(continue)
        }
        temp_inst=tokanizeopcode(line);

        if(strcmp(temp_inst.opcode,"End")==0)
        {
            break;
        }
        fprintf(fp2,"%x %s %s %s\n",*address,temp_inst.label,temp_inst.opcode,temp_inst.operand);
        if(strcmp(temp_inst.label,"")!=0)
        {
            //Search label in SYMTAB and if found display error else add to symbol table
            addSymbol(fp3,temp_inst,address,lc);
        }
        //Now update address and start from here
        updateAddress(address, temp_inst,opcodearray,codearray);
    }
}

struct instruction tokanizeopcode(char line[])
{
    struct instruction inst;

    int count=0;
    char *token1=NULL;
    char *token2=NULL;
    char *token3=NULL;

    token1=strtok(line," ");
    token2=strtok(NULL," ");
    token3=strtok(NULL," ");

    if(token3!=NULL)
    {
        count=3;
        inst.label=token1;
        inst.opcode=token2;
        inst.operand=token3;
    }

    if(token3==NULL && token2!=NULL)
    {
        count=2;
        inst.label="";
        inst.opcode=token1;
        inst.operand=token2;
    }
    
    if(token3==NULL && token2==NULL)
    {
        count=1;
        inst.label="";
        inst.opcode=token1;
        inst.operand="";
    }
    return inst;
}

void  ProcessFirstLine(FILE *fp1,FILE *fp2,FILE *fp3,int *address,char opcodearray [][MAXOP],int codearray[])
{
    *address=0;
    char line[31];
    int lc=0;
    struct instruction temp_inst;
    while(!feof(fp1))
    {
        fgets(line,31,fp1);

        if(line[strlen(line)-1=='\n'])
            line[strlen(line)-1]='\0';

            if(line[0]=='.')
            {
                fprintf(fp2,"%s\n",line);
                continue;//For comments continue
            }

            temp_inst=tokanizeopcode(line);

            if(strcmp(temp_inst.opcode,"START")==0 || strcmp(temp_inst.opcode,"Start")==0)
            {
                // *address=temp_inst.operand;
                sscanf(temp_inst.operand,"%x",address);
                start_flag=1;
                fprintf(fp2,"%s %s %s\n",temp_inst.label,temp_inst.opcode,temp_inst.operand);
                //For start no address is assigned and is written into intermediate file and no need to to check symbol table
            }
            else
            {
                fprintf(fp2,"%x %s %s %s\n",*address,temp_inst.label,temp_inst.opcode,temp_inst.operand);
                //Addsymbol table
                addSymbol(fp3,temp_inst,address,lc);
                //Update address
                updateAddress(address,temp_inst,opcodearray,codearray);
            }
            break;
    }
}


void displayOPTAB(char opcodearray[][MAXOP],int codearray[],int n)
{
    printf("\n-----OPTAB-----\n");
    for(int i=0;i<n;i++)
    {
        printf("OPCODE:%s INT_CODE:%d HEX_CODE:%x\n",opcodearray[i],codearray[i],codearray[i]);
    }
    printf("-----END OPTAB------\n");
}

int getOPTAB(FILE *fp4,char opcodearray[][MAXOP],int codearray[])
{
    char line[31];
    char opcode[MAXOP];
    int code;
    int i=0;
    char *temp;
    while(!feof(fp4))
    {
        // fgets(line,30,fp4);
        // if(line[strlen(line)-1]=='\n')
        //     line[strlen(line)-1]='\0';
        fscanf(fp4,"%s %s",opcode,code);
        
        strcpy(opcodearray[i],opcode);
        // codearray[i]=code;
        // strcpy(*temp,opcodearray[i]);
        // sscanf(temp,"%x",&opcode[i]);
        temp=code; //Don't make it *temp else all addresses will be zero
        sscanf(temp,"%x",&codearray[i]);
        i++;
    }
    return i;
}

int main()
{

    FILE *fp1,*fp2,*fp3,*fp4;
    fp1=fopen("INPUTFILEP1.txt","r");
    fp2=fopen("OUTPUTFILEP1.txt","w");
    fp3=fopen("SYMTABP1.txt","w+");
    fp4=fopen("OPTABP1.txt","r");

    //All variable are here
    int OPTAB_size;
    int starting_address;
    int last_address;
    char opcodearray[MAXOP][MAXOP];
    int codearray[MAXOP];
    int address;

    if(fp1==NULL)
    {
        printf("\nError reading file 1\n");
        return 1;
    }
    if(fp2==NULL)
    {
        printf("\nError reading file 2\n");
        return 1;
    }
    if(fp3==NULL)
    {
        printf("\nError reading file 3\n");
        return 1;
    }
    if(fp4==NULL)
    {
        printf("\nError reading file 4\n");
        return 1;
    }

    printf("\n-----------All files are opened----------\n");

    //GET OPTAB ready
    OPTAB_size=getOPTAB(fp4,opcodearray,codearray);
    displayOPTAB(opcodearray,codearray,OPTAB_size);
    ProcessFirstLine(fp1,fp2,fp3,&address,opcodearray,codearray);
    starting_address=address;
    if(start_flag==0)
    {
        starting_address=0;
    }
    printf("\n----- Start address is %x -----\n",starting_address);
    processNextLines(fp1,fp2,fp3,&address,opcodearray,codearray);
    last_address=address;
    printf("\nProgram length  Hex: %x Decimal: %d",last_address-starting_address,last_address-starting_address);

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    printf("\n------- All files closed ---------\n");
    return 0;
}
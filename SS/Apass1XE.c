#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXOP 100

static int start_flag=0;

int getOPTAB(FILE *fp4,char opcodearray[][MAXOP],int codearray[],int instsize[]);
void  ProcessFirstLine(FILE *fp1,FILE *fp2,FILE *fp3,int *address,char opcodearray [][MAXOP],int codearray[],int instsize[]);
void displayOPTAB(char opcodearray[][MAXOP],int codearray[],int instsizee[],int n);
void processNextLines(FILE *fp1,FILE *fp2,FILE *fp3,int *address,char opcodearray[][MAXOP],int codearray[],int instsize[]);
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


void updateAddress(int *address,struct instruction temp_inst,char opcodearray[][MAXOP],int codearray[],int instsize[])
{
    int hex_operand;
    if(!strcmp(temp_inst.opcode,"WORD") || !strcmp(temp_inst.opcode,"Word") || !strcmp(temp_inst.opcode,"word"))
    {
        *address = *address+3;

        return;
    }

    else if(!strcmp(temp_inst.opcode,"RESW") || !strcmp(temp_inst.opcode,"Resw") || !strcmp(temp_inst.opcode,"resw"))
    {
        sscanf(temp_inst.operand,"%d",&hex_operand);
        *address = *address+ (3*hex_operand);

        return;
    }

    else if(!strcmp(temp_inst.opcode,"RESB") || !strcmp(temp_inst.opcode,"Resb") || !strcmp(temp_inst.opcode,"resb"))
    {
        sscanf(temp_inst.operand,"%d",&hex_operand);
        *address = *address + (1*hex_operand);

        return;
    }

    else if(!strcmp(temp_inst.opcode,"BYTE") || !strcmp(temp_inst.opcode,"Byte") || !strcmp(temp_inst.opcode,"byte"))
    {
        int count=0;
        int i=2;    // characters start at 2nd index inside C'sample' or X'sample'
        while(temp_inst.operand[i]!=39)
        {
            
            count++;
            i++;
        }
        
        hex_operand = count;

        if(temp_inst.operand[0]=='X' || temp_inst.operand[0]=='x')
        {
            *address = *address + (0.5*hex_operand);
        }

        else if(temp_inst.operand[0]=='C'||temp_inst.operand[0]=='c')
        {
            *address = *address + (1*hex_operand);
        }
        return;       
    }

    if(temp_inst.opcode[0]=='+')
    {   
        *address = *address + 4;
        return;
    }
    // Else search opcode in OPTAB
    int opcode_found = 0;   // set to 1 if opcode found in OPTAB, else 0
    int i;

    for(i=0;i<MAXOP;i++)
    {
        if(!strcmp(temp_inst.opcode,opcodearray[i]))
        {
            opcode_found = 1;
            break;
        }
    }

    if(opcode_found == 1)
    {
        *address = *address + instsize[i];

        return;
    }
    else
    {
        printf("\nERROR: opcode %s not found in OPTABLE\n",temp_inst.opcode);
        exit(6);
    }
}

void  addSymbol(FILE *fp3,struct instruction tinst, int *address)
{
    rewind(fp3);
    int flag=0;
    char symbol[MAXOP];
    char symboladdress[MAXOP];
    while(!feof(fp3))
    {
        fscanf(fp3,"%s %s",symbol,symboladdress);
        if(strcmp(symbol,tinst.label)==0)
        {
            flag=1;
            break;
        }
    }

    if(flag)
    {
        printf("-----ERROR Label %s is repeated-----",tinst.label);
    }
    else{
        fprintf(fp3,"%s %x\n",tinst.label,*address);
    }
}

void processNextLines(FILE *fp1,FILE *fp2,FILE *fp3,int *address,char opcodearray[][MAXOP],int codearray[],int instsize[])
{
    char line[31];
    struct instruction temp_inst;
    while(!feof(fp1))
    {
        fgets(line,30,fp1);

        if(line[strlen(line)-1]=='\n')
            line[strlen(line)-1]='\0';

            if(strcmp(line,"BASE")==0)
            {
                fprintf(fp2,"%s\n",line);
                continue;
            }

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

        if(strcmp(temp_inst.opcode,"TIXR")==0 || strcmp(temp_inst.opcode,"COMPR")==0 || strcmp(temp_inst.opcode,"ADDR")==0)
    {
    
    char line1[100];
    line1[0]='\0';
    strcpy(line1,temp_inst.operand);
    char *c1;
    char *c2;
    c1=strtok(line1,",");
    c2=strtok(NULL,",");
        
    printf("--%s--\n",c1);
    printf("--%s--\n",c2);

        if(c2!=NULL)
        {
             if(c1=='a' || c1=='A' || c1=='S' || c1=='s' || c1=='t' || c1=='T' || c1=='B' ||c1=='b'||c1=='x' ||c1=='X')
                {
                    printf("\nOperand1 %s is invalid --\n",temp_inst.operand);
                }
                else
                {

                }
        }
        if(c2!=NULL)
        {
            if(c2=='a' || c2=='A' || c2=='S' || c2=='s' || c2=='t' || c2=='T' || c2=='B' || c2=='b'||c2=='x' || c2=='X')
                {                    
                    printf("\nOperand %s is invalid --\n",temp_inst.operand);    

                }
                else
                    {
                        
                    }
        }
        }
        else{         
    }
            if(strcmp(temp_inst.opcode,"BASE")==0)
            {
                fprintf(fp2,"%s %s\n",temp_inst.opcode,temp_inst.operand);
             
                continue;
            }


        if(strcmp(temp_inst.opcode,"End")==0)
        {
            break;
        }
        fprintf(fp2,"%x %s %s %s\n",*address,temp_inst.label,temp_inst.opcode,temp_inst.operand);
        if(strcmp(temp_inst.label,"")!=0)
        {
            //Search label in SYMTAB and if found display error else add to symbol table
            addSymbol(fp3,temp_inst,address);
        }
        //Now update address and start from here
        updateAddress(address, temp_inst,opcodearray,codearray,instsize);
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

void  ProcessFirstLine(FILE *fp1,FILE *fp2,FILE *fp3,int *address,char opcodearray [][MAXOP],int codearray[],int instsize[])
{
    *address=0;
    char line[31];
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

            if(strcmp(temp_inst.opcode,"START")==0  || strcmp(temp_inst.opcode,"Start")==0)
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
                addSymbol(fp3,temp_inst,address);
                //Update address
                updateAddress(address,temp_inst,opcodearray,codearray,instsize);
            }
            break;
    }
}

void displayOPTAB(char opcodearray[][MAXOP],int codearray[],int instsize[],int n)
{
    printf("\n-----OPTAB-----\n");
    for(int i=0;i<n;i++)
    {
        printf("OPCODE:%s INT_CODE:%d HEX_CODE:%x SIZE:%d\n",opcodearray[i],codearray[i],codearray[i],instsize[i]);
    }
    printf("-----END OPTAB------\n");
} 

int getOPTAB(FILE *fp4,char opcodearray[][MAXOP],int codearray[],int instsize[])
{
    char line[31];
    char opcode[MAXOP];
    int code;
    int size;
    int i=0;
    char *temp;
    while(!feof(fp4))
    {
        // fgets(line,30,fp4);
        // if(line[strlen(line)-1]=='\n')
        //     line[strlen(line)-1]='\0';
        fscanf(fp4,"%s %s %d",opcode,code,&size);
        instsize[i]=size;
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
    fp1=fopen("INPUTFILEXE.txt","r");
    fp2=fopen("OUTPUTFILEXE.txt","w");
    fp3=fopen("SYMTABXE.txt","w+");
    fp4=fopen("OPTABXE.txt","r");

    //All variable are here
    int OPTAB_size;
    int starting_address;
    int last_address;
    char opcodearray[MAXOP][MAXOP];
    int codearray[MAXOP];
    int address;
    int instsize[MAXOP];

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
    OPTAB_size=getOPTAB(fp4,opcodearray,codearray,instsize);
    displayOPTAB(opcodearray,codearray,instsize,OPTAB_size);
    ProcessFirstLine(fp1,fp2,fp3,&address,opcodearray,codearray,instsize);
    starting_address=address;
    if(start_flag==0)
    {
        starting_address=0;
    }
    printf("\n----- Start address is %x -----\n",starting_address);
    processNextLines(fp1,fp2,fp3,&address,opcodearray,codearray,instsize);
    last_address=address;
    printf("\nProgram length  Hex: %x Decimal: %d",last_address-starting_address,last_address-starting_address);

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    printf("\n------- All files closed ---------\n");
    return 0;
}
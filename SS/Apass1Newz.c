/*


Final START 1000
LDX zero
Str6 RESB 4096
TIXR Count
JLT CopyStr
zero WORD 0
Str3 BYTE c'BVB_CET'
Str1 BYTE c'BVB_CET'
Str51 RESB 4096
Str5 RESB 4096
Str9 RESB 4096
Str10 RESB 4096
Str211 RESB 4096
Str31 BYTE x'A1B2C3'
temp RESW 10
Count WORD 7
End

LDX 74
STCH 54
TIX 2C
JLT 38
CLEAR b4
SUBR 94
COMPR a0
TIXR b8



*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLEN 112

struct inst
{
    char *label;
    char *opcode;
    char *operand;
};


    struct inst tokenizeopcode(char line[])
    {
           int count=0;
    struct inst inst;
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


void  addSymbol(FILE *fp3,struct inst tinst, int *address)
{
    if(tinst.label=="")
        return;
   rewind(fp3);
    int flag=0;
    char symbol[MAXLEN];
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
        printf("\n-----ERROR Label %s is repeated in line-----\n",tinst.label);
    }
    else{
        fprintf(fp3,"%s %x\n",tinst.label,*address);
    }
}

static int start=0;

    void displayoptab(char opcodearr[][MAXLEN],int codearr[],int n)
    {
        printf("\n=====OPTAB=======\n");
        for(int i=0;i<n;i++)
        printf("\n%s %x",opcodearr[i],codearr[i]);
        printf("\n=======OPTAB completed========\n");
    } 

    int getoptab(FILE *fp4,char opcodearr[][MAXLEN],int codearr[])
    {
        int i=0;
        char line[MAXLEN];
        while(!feof(fp4))
        {
            fgets(line,MAXLEN,fp4);

            if(line[strlen(line)-1]=='\n')
            line[strlen(line)-1]='\0';
            
            //If line is empty continue
            if(feof(fp4))
            {
                continue;
            }

            char *t1,*t2;
            t1=strtok(line," ");
            t2=strtok(NULL," ");
            strcpy(opcodearr[i],t1);
            sscanf(t2,"%x",&codearr[i]);
            i++;
        }
        return i;
    }

void updateaddress(int *address,struct inst inst,char opcode[][MAXLEN],int code[],int n)
{
    int opflag=0;

    if(strcmp("WORD",inst.opcode)==0 || strcmp("word",inst.opcode)==0)
    {
        *address+=3;
        return;
    }
    if(strcmp("RESW",inst.opcode)==0 || strcmp("resw",inst.opcode)==0)
    {
        int add;
        sscanf(inst.operand,"%d",&add);
        *address+=(add*3);
        return;
    }
    if(strcmp("RESB",inst.opcode)==0 || strcmp("resb",inst.opcode)==0)
    {
        int add;
        sscanf(inst.operand,"%d",&add);
        *address+=(add);
        return;
    }
    if(strcmp("BYTE",inst.opcode)==0 || strcmp("byte",inst.opcode)==0)
    {
        int c=0;
        for(int i=2;i<strlen(inst.operand);i++)
        {
            c++;
        }
        c--;
        if(inst.operand[0]=='c' || inst.operand[0]=='C')
        {
            *address+=c;
            return;
        }
        if(inst.operand[0]=='x' || inst.operand[0]=='X')
        {
            *address+=(c/2);
            return;
        }
    }
    for(int i=0;i<n;i++)
    {
        if(strcmp(opcode[i],inst.opcode)==0)
        {
            opflag=1;
        }
    }
    if(opflag)
    {
            *address+=3;
            return;
    }
    else
    {
        printf("\nInvalid opcode : %s\n",inst.opcode);
        return;
    }
    
}

void  proflines(FILE *fp1,FILE *fp2,FILE *fp3,FILE *fp4,char opcodearr[][MAXLEN],int codearr[],int *address,int n)
{
        struct inst tempinst;
        char line[MAXLEN];
        while(!feof(fp1))
        {
            fgets(line,MAXLEN,fp1);

            if(line[strlen(line)-1]=='\n')
            line[strlen(line)-1]='\0';

            if(line[0]=='.')
            {
                fprintf(fp2,"%s\n",line);
                continue;
            }

            tempinst=tokenizeopcode(line);

            if(strcmp(tempinst.opcode,"START")==0 || strcmp(tempinst.opcode,"start")==0)
            {
                start=1;
                    sscanf(tempinst.operand,"%x",address);
                  printf("%s %s %s\n",tempinst.label,tempinst.opcode,tempinst.operand);
                fprintf(fp2,"%s %s %s\n",tempinst.label,tempinst.opcode,tempinst.operand);
            
            }
            else{
                start=0;
                *address=0;
                printf("%04x %s %s %s\n",*address,tempinst.label,tempinst.opcode,tempinst.operand);
                fprintf(fp2,"%04x %s %s %s\n",*address,tempinst.label,tempinst.opcode,tempinst.operand);
                    //add symbol
                addSymbol(fp3,tempinst,address);

                    //update address
            updateaddress(address,tempinst,opcodearr,codearr,n);

              }
            break;
        }
}

    void  pronlines(FILE *fp1,FILE *fp2,FILE *fp3,FILE *fp4,char opcodearr[][MAXLEN],int codearr[],int *address,int n)
    {
        struct inst tempinst;
        char line[MAXLEN];
        while(!feof(fp1))
        {
            fgets(line,MAXLEN,fp1);

            if(line[strlen(line)-1]=='\n')
            line[strlen(line)-1]='\0';

            if(line[0]=='.')
            {
                fprintf(fp2,"%s\n",line);
                continue;
            }

            tempinst=tokenizeopcode(line);

             if(!strcmp(tempinst.opcode,"End"))
            {
                break;
            }
            
            printf("%x %s %s %s\n",*address,tempinst.label,tempinst.opcode,tempinst.operand);
            fprintf(fp2,"%x %s %s %s\n",*address,tempinst.label,tempinst.opcode,tempinst.operand);
            //add symbol
        addSymbol(fp3,tempinst,address);
            
            //update address
            updateaddress(address,tempinst,opcodearr,codearr,n);
        }//End of while(1feof(fp1))

    }


int main()

{
    FILE *fp1,*fp2,*fp3,*fp4;
    fp1=fopen("INPUTFILEP1.txt","r");
    fp2=fopen("OUTPUTFILEP1.txt","w");
    fp3=fopen("SYMTABP1.txt","w+");
    fp4=fopen("OPTABP1.txt","r");

    char opcodearr[MAXLEN][MAXLEN];
    int codearr[MAXLEN];

    int address=0;



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

    int optabsize=getoptab(fp4,opcodearr,codearr);
    displayoptab(opcodearr,codearr,optabsize);

    proflines(fp1,fp2,fp3,fp4,opcodearr,codearr,&address,optabsize);
    pronlines(fp1,fp2,fp3,fp4,opcodearr,codearr,&address,optabsize);

    printf("\n======Assembler pass1 done=====\n");

}

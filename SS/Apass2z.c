#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXLEN 100

    struct inst
    {
         int location;
    char *label;
    char *opcode;
    char *operand;

    };

void displayoptab(char opcodearr[][MAXLEN],int codearr[],int n)
    {
        printf("\n=====OPTAB=======\n");
        for(int i=0;i<n;i++)
        printf("\n%s %x",opcodearr[i],codearr[i]);
        printf("\n=======OPTAB completed========\n");
    } 

    void displaysymtab(char opcodearr[][MAXLEN],int codearr[],int n)
    {
        printf("\n=====SYMTAB=======\n");
        for(int i=0;i<n;i++)
        printf("\n%s %x",opcodearr[i],codearr[i]);
        printf("\n=======SYMTAB completed========\n");
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

    int getsymtab(FILE *fp4,char opcodearr[][MAXLEN],int codearr[])
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

   struct inst tokenize(char line[])
{
    int count=0;
    struct inst inst1;
    char *token1=NULL;
    char *token2=NULL;
    char *token3=NULL;
    char *token4=NULL;

    token1=strtok(line," ");
    token2=strtok(NULL," ");
    token3=strtok(NULL," ");
    token4=strtok(NULL," ");

    if(token3==NULL && token4==NULL)
        count=2;

    if(token3!=NULL && token4==NULL)
        count=3;

    if(token4!=NULL)
        count=4;

        int temp;
        char *ptr;
    // sscanf(token1,"%x",&temp);
    temp = strtol(token1, &ptr, 16);


    inst1.location=temp;

    if(count==2)
    {
        inst1.label="";
        inst1.opcode=token2;
        inst1.operand="";
    }


    if(count==3)
    {
        inst1.label="";
        inst1.opcode=token2;
        inst1.operand=token3;
    }

    if(count==4)
    {
        inst1.label=token2;
        inst1.opcode=token3;
        inst1.operand=token4;
    }

    return inst1;
}


    void processfline(FILE *fp1,FILE *fp2,FILE *fp3,FILE *fp4,FILE *fp5,int programlength)
    {
        char name[MAXLEN];
        char s[MAXLEN];
        int add;
        fscanf(fp1,"%s %s %x",name,s,&add);

        fprintf(fp5,"H^%s^%04x^%x\n",name,add,programlength);
        fprintf(fp2,"%s %s %x\n",name,s,add);
    }

    void processnlines(FILE *fp1,FILE *fp2,FILE *fp3,FILE *fp4,FILE *fp5,char opcodearr[][MAXLEN],int codearr[],char symbolarr[][MAXLEN],int addarr[])
    {
        struct inst tempinst;
        char line[MAXLEN];
        while(!feof(fp1))
        {
            fgets(line,31,fp1);

            if(line[strlen(line)-1]=='\n')
            line[strlen(line)-1]='\0';

            if(line[0]=='.')
            {
                fprintf(fp2,"%s\n",line);
                continue;
            }
            if(strcmp(line,"END")==0)
            {
                fprintf(fp2,"%s\n",line);
                continue;
            }
            // printf("=======");
            tempinst=tokenize(line);
            // printf(";=====;==");
            // printf("%s\n",line);
            // fprintf(fp2,"%s\n",line);

            fprintf(fp2,"%x %s %s %s\n",tempinst.location,tempinst.label,tempinst.opcode,tempinst.operand);
            printf("%x %s %s %s\n",tempinst.location,tempinst.label,tempinst.opcode,tempinst.operand);

        }
    }


int main()
{
    FILE *fp1,*fp2,*fp3,*fp4,*fp5;
    fp1=fopen("INPUTFILEP1.txt","r");
    fp2=fopen("OUTPUTFILEP1.txt","w");
    fp3=fopen("SYMTABP1.txt","r");
    fp4=fopen("OPTABP1.txt","r");
    fp5=fopen("objectcode.txt","w");

    int programlength=100;
    char opcodearr[MAXLEN][MAXLEN];
    int codearr[MAXLEN];

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

    char symbolarr[MAXLEN][MAXLEN];
    int addarr[MAXLEN];

    int symtabsize=getoptab(fp3,symbolarr,addarr);
    displaysymtab(symbolarr,addarr,symtabsize);

    processfline(fp1,fp2,fp3,fp4,fp5,programlength);

    processnlines(fp1,fp2,fp3,fp4,fp5,opcodearr,codearr,symbolarr,addarr);

}
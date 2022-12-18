// Linking Loader
// 1. Processing Header record

/*
Control | Symbol | Address | Length
Section | Name   |            
------------------------------
PROGA	        	500 	63
PROGB		        563     7F
PROGC		        5E2     51
*/

/*


H^PROGA^000000^000064
D^LISTA^000040^ENDA^000054
R^LISTB^ENDB^LISTC^ENDC
T^000020^141033^465555^678909^568787^345678
T^000054^000014^789087^776555^876666^456666
M^000054^06^+^LISTC
E^000020

H^PROGB^000000^00007F
D^LISTB^000060^ENDB^000070
R^LISTA^ENDA^LISTC^ENDC
T^000036^141033^465555^678909^568787^345678
T^000070^000000^789087^776555^876666^456666
M^000054^06^+^ENDA
M^000054^06^-^LISTA
M^000054^06^+^LISTC
E

H^PROGC^000000^000051
D^LISTC^000030^ENDC^000042
R^LISTA^ENDA^LISTC^ENDB
T^000018^141033^465555^678909^568787^345678
T^000042^000020^789087^776555^876666^456666
M^000054^06^+^ENDA
M^000054^06^-^LISTA
M^000054^06^+^PROGC
E
END

Name	Address	Length
PROGA		1000	64
LISTA		1040
ENDA		1054
PROGB		1064	7f
LISTB		10c4
ENDB		10d4
PROGC		10e3	51
LISTC		1113
ENDC		1125


*/




#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BIG 1024
#define SMOL 128

struct H_ESTAB
{
    char csname[SMOL];
    char extsym[SMOL];
    int address;
    int length;
};

struct D_ESTAB
{
    char varname[SMOL];
    int varadd;

};

struct H_ESTAB getHeaderTokens(char line[])
{
    struct H_ESTAB temp;
    char *t1=strtok(line,"^");//H
    char *t2=strtok(NULL,"^");//Name
    strcpy(temp.csname,t2);
    strcpy(temp.extsym,"");
    char *t3=strtok(NULL,"^");//Address
    sscanf(t3,"%x",&temp.address);
    char *t4=strtok(NULL,"^");//Length
    sscanf(t4,"%x",&temp.length);
    return temp;
}

int Process(FILE* fp1,FILE* fp2,FILE* fp3,struct H_ESTAB Hrecords[],int PROGADDR)
{
    int line_count = 0;
    int Hcount = 0;


    while(!feof(fp1))
    {
        char line[SMOL];
        char text[BIG];
        text[0]='\0';
        struct H_ESTAB Hrecords[SMOL];

        fgets(line,SMOL,fp1);

        // If there is an empty line
        if(line[0] == '\n')
        {
            continue;
        }

        // fgets things
        if(line[strlen(line)] == '\n')
        {
            line[strlen(line)-1] = '\0';
        }

        strcpy(text,line);

        // If line is a Header Record
        if(line[0] == 'H' && line[1] == '^')
        {
            Hrecords[Hcount]=getHeaderTokens(line);

            if(Hcount==0)
            {
                Hrecords[Hcount].address += PROGADDR;
            }
            else
            {
                Hrecords[Hcount].address += Hrecords[Hcount-1].address + Hrecords[Hcount-1].length ;
            }
            // fprintf(fp2,"%s\t%s\t%x\t%x\n",Hrecords[Hcount].csname,Hrecords[Hcount].extsym,Hrecords[Hcount].address,Hrecords[Hcount].length);
            // printf("\n%s\t%s\t%x\t%x",Hrecords[Hcount].csname,Hrecords[Hcount].extsym,Hrecords[Hcount].address,Hrecords[Hcount].length);
            Hcount++;
        }

        if(line[0]=='T' && line[1]=='^')
        {
            int addr;
            char *chardigit,*temp;
            int digit;
            char *token1,*token2,*token3;
            token1=strtok(line,"^"); //T
            token2=strtok(NULL,"^"); //Address
            temp=token2;
            //Extracting a digit to put those many number of x
            sscanf(&token2[5],"%x",&digit);

            token2=temp;
            token2[5]='0';
            printf("Tt %s %s %c %d\n",token1,token2,token2[5],digit);

            //Writting address in fp3
            sscanf(token2,"%x",&addr);
            fprintf(fp3,"%x ",addr);

            

            //Putting x 
             int x=0;
            while(x<digit)
            {
            
                fprintf(fp3,"XX");
                x=x+1;
            }

            int countdigit=0;
            
            for(int k=2;k<strlen(text);k++)
            {
                if(countdigit==32)
                {
                    fprintf(fp3,"\n%x",addr+0x10);
                }

                if(countdigit%8==0) 
                {
                    fprintf(fp3," ");
                }

                if(text[k]!='^')
                {
                fprintf(fp3,"%c",text[k]);
                countdigit++;
                }
                
            }

            printf("\n");
            fprintf(fp3,"\n");


        }

        // If line is a Define Record
        
        // If line is a Refer Record
        else if(line[0] == 'R' && line[1] == '^')
        {
            //Pass2
        }

        else if(line[0] == 'E' && line[1] == '^')
        {
            //Pass2
            
        }

        line_count++;
    }

    return line_count;
}

int main()
{
    FILE *fp1 = fopen("ll-input.txt","r"); //Open input file in read mode
    FILE *fp2 = fopen("ESTAB.txt","w"); //Open input file in write mode
    FILE *fp3 = fopen("Memory.txt","w"); //Open input file in write mode


    if(fp1==NULL){
        perror("  => Error opening the Input read file");
        return -1;
    }
    if(fp2==NULL){
        printf("  => Error opening the ESTAB write file");
        return -2;
    }

    struct H_ESTAB Hrecords[BIG];
    struct D_ESTAB Drecords[BIG];
    int PROGADDR;
    int CSADDR;
    int EXEADDR;

    
    PROGADDR=1000;
    CSADDR=PROGADDR;
    EXEADDR=PROGADDR;

    
    int line_count = Process(fp1,fp2,fp3,Hrecords,PROGADDR);

    return 0;   // Program executed successfully
}
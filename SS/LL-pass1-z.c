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




*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLEN 112

struct HESTAB
{
    char name[MAXLEN];
    int length;
    int addr;
};

struct HESTAB token(char line[])
{
    struct HESTAB temp;
    int add,len;
    char *t1,*t2,*t3,*t4;
    t1=strtok(line,"^");
    t2=strtok(NULL,"^");
    t3=strtok(NULL,"^");
    t4=strtok(NULL,"^");

    strcpy(temp.name,t2);
    sscanf(t3,"%x",&add);
    sscanf(t4,"%x",&len);
    temp.addr=add;
    temp.length=len;
    return temp;
}

struct DESTAB
{
    char name[MAXLEN];
    int addr;
};

void processlines(FILE *fp1,FILE *fp2,int progaddr,struct HESTAB Hrec[],struct DESTAB Drec[] )
{
            fprintf(fp2,"Name    Address    Length");
            printf("\nName    Address    Length");


    int Dcnt=0;
    int Hcnt=0;

    char line[MAXLEN];

    while(!feof(fp1))
    {
        fgets(line,MAXLEN,fp1);
        if(line[strlen(line)-1]=='\n')
        {
            line[strlen(line)-1]='\0';

        }

        if(line[0]=='H')
        {
            // Head record
            Hrec[Hcnt]=token(line);

            if(Hcnt==0)
            {
                Hrec[Hcnt].addr+=progaddr;
            }
            else{
                Hrec[Hcnt].addr+=Hrec[Hcnt-1].addr+Hrec[Hcnt-1].length;
            }



            fprintf(fp2,"\n%s    %x         %x",Hrec[Hcnt].name,Hrec[Hcnt].addr,Hrec[Hcnt].length);
            printf("\n%s    %x    %x",Hrec[Hcnt].name,Hrec[Hcnt].addr,Hrec[Hcnt].length);

            Hcnt++;
        }

        int cnt=0;
        if(line[0]=='D')
        {
            for(int i=0;i<strlen(line);i++)
            {
                if(line[i]=='^')
                    cnt++;
            }

            char *t=strtok(line,"^");
            char *t3,*t4;
            int tempadd;
            cnt=cnt/2;
            for(int i=0;i<cnt;i++)
            {
                t3=strtok(NULL,"^");
                t4=strtok(NULL,"^");
                strcpy(Drec[Dcnt].name,t3);
                sscanf(t4,"%x",&tempadd);
                Drec[Dcnt].addr=tempadd;
                Drec[Dcnt].addr+=Hrec[Hcnt-1].addr;
                fprintf(fp2,"\n%s    %x",Drec[Dcnt].name,Drec[Dcnt].addr);
                printf("\n%s    %x",Drec[Dcnt].name,Drec[Dcnt].addr);


                Dcnt++;


            }
        }
    }


}

int main()

{
    int progaddr=0;
    FILE *fp1,*fp2;
    fp1=fopen("ll-input.txt","r");
    fp2=fopen("ESTABZ.txt","w");

    struct HESTAB  Hrec[MAXLEN]; 
    struct DESTAB  Drec[MAXLEN]; 

    if(fp1==NULL)
    {
        printf("\nError opening input file");
    }
    if(fp1==NULL)
    {
        printf("\nError opening output file");
    }
    printf("\nEnter starting location:\n");
    scanf("%x",&progaddr);

    processlines(fp1,fp2,progaddr,Hrec,Drec);

    printf("\n======Done========\n");
}
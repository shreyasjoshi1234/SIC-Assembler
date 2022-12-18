#include <stdio.h>
#include <string.h>
#define MAX 20

struct external_symbol_table
{
  char cs_name[MAX];
  char ext_sys_name[MAX];
  int address;
  int length;
} est[MAX];


void pass1()
{
  char csname[MAX], inpline[MAX];
  FILE *f1, *f2;
  int program_address, cs_address, addresss, len;
  int count = 0;

  f1 = fopen("INPUT1.txt", "r");
  f2 = fopen("estab.txt", "w");

  printf("Enter the location to store the Program: ");
  scanf("%d", &program_address);


    fscanf(f1, "%s", inpline);

    cs_address = program_address;

    while(strcmp(inpline,"END") !=0 )
    {


      if((strcmp(inpline, "H") == 0))
      {
        fscanf(f1, "%s", csname);
        strcpy(est[count].cs_name, csname);

          strcpy(est[count].ext_sys_name, "**");

          fscanf(f1, "%d", &addresss);
          est[count].address = addresss + cs_address;

          fscanf(f1, "%d", &len);
          est[count].length = len;

          fprintf(f2, "%s\t%s\t\t%d\t%d\n", est[count].cs_name, est[count].ext_sys_name, est[count].address, est[count].length);
          count++;
      }
      else if(strcmp(inpline, "D") == 0)
      {
        fscanf(f1, "%s", inpline);

        while(strcmp(inpline, "R") != 0)
        {
          strcpy(est[count].cs_name, "**");

            strcpy(est[count].ext_sys_name, inpline);

            fscanf(f1, "%d", &addresss);
            est[count].address = addresss + cs_address;

            est[count].length = 0;

            fprintf(f2, "%s\t%s\t\t%d\t%d\n", est[count].cs_name, est[count].ext_sys_name, est[count].address, est[count].length);

            count++;

            fscanf(f1, "%s", inpline);
          }
          cs_address += len;
      }
      else if (strcmp(inpline, "T") == 0)
      {
          while (strcmp(inpline, "E") != 0)
          {
            fscanf(f1, "%s", inpline);
          }
      }
      fscanf(f1, "%s", inpline);
    }

    fclose(f1);
    fclose(f2);
    printf("Stored In Estab\n");
}


int main()
{
  pass1();

  return  0;
}

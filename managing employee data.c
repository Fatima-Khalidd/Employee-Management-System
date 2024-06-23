#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define maxline 30
typedef struct
{
  int id;
  char name[50];
  float salary;
  float Pf;
  float Hltins;
  char email[50];
} employee;
int rec;

void menu();                                          /*displays menu bar*/
void dotask(employee emp[], char op);           /*Performs different task based on the option chosen by the user*/
void addEmployee(employee emp[]);                     /*Used to add employee*/
void SearchEmployee(employee emp[]);                  /*Searches a specific and shows his record*/
int searchEmpid(employee emp[], int targetid); /*Searches if the particular employee exists in the record*/
void printEmp(employee emp[]);                        /*Displays the data of all the employees which are registered in record*/
float Netsalary(employee emp);                        /*Returns the netsalary of the employee*/
void readinfo(employee emp[]);                 /*Reads all the data of employees from the record file*/
void RemoveEmp(employee emp[]);                       /*Removes a specific employee from the record file*/
void Updateinfo(employee emp[]);                      /*Updates data of a specific employee's data*/

int main()
{
  bool exit = false;
  char op;
  employee emp[maxline];
  readinfo(emp);
  printf("Total %d records read from the data file.\n", rec);
  menu();
  do
  {
    fflush(stdin);
    printf("\nYour Option: ");
    scanf("%c", &op);
    if (op == '6')
    {
      exit = true;
    }
    else
    {
      dotask(emp, op);
    }

  } while (exit != true);

  return 0;
}
void readinfo(employee emp[])
{
  FILE *pf = fopen("emp.bin", "rb");
  if (pf == NULL)
  {
    printf("Error in opening file !!");
  }

  int i = 0;
  while (!feof(pf) && !ferror(pf))
  {
    fread(&emp[i], sizeof(employee), 1, pf);
    i++;
  }
  fclose(pf);
  rec = i - 1;
}
void menu()
{
  printf("MENU:-\n");
  printf("1.Add an Employee \n");
  printf("2.Print Employee Details \n");
  printf("3.Search an Employee\n");
  printf("4.Remove an Employee\n");
  printf("5.Update an Employee\n");
  printf("6.Exit\n");
}
void dotask(employee emp[], char op)
{
  switch (op)
  {
  case '1':
    addEmployee(emp);

    printf("Now we have %d employees in record!!\n", rec);
    break;

  case '2':
    printEmp(emp);

    break;
  case '3':

    SearchEmployee(emp);
    break;
  case '4':
    RemoveEmp(emp);
    break;
  case '5':
    Updateinfo(emp);
    break;

  default:
    printf("Invalid option, Valid option is from 1-6\n");
    break;
  }
}

void addEmployee(employee emp[])
{

  employee temp;
  char code;
  bool isUnique = false;
  do
  {
    fflush(stdin);
    printf("Enter Employee id : ");
    scanf("%d", &temp.id);
    isUnique = false;
    if (searchEmpid(emp, temp.id) != -1)
    {
      isUnique = true;
      printf("Employee id %d already exists in record, Input a unique id.\n", temp.id);
      printf("M/m for menu and any other to add employee: ");
      scanf(" %c", &code);
      if (code == 'm' || code == 'M')
      {
        return;
      }
    }

  } while (isUnique);

  fflush(stdin);
  printf("Enter the Employee name : ");
  gets(temp.name);
  printf("Enter the Employee Salary($) :");
  scanf("%f", &temp.salary);
  printf("Enter the Employee Pf($) :");
  scanf("%f", &temp.Pf);
  printf("Enter the Employee Health insurance($) :");
  scanf("%f", &temp.Hltins);
  fflush(stdin);
  printf("Enter the Employee email : ");
  gets(temp.email);
  FILE *pf = fopen("emp.bin", "ab");
  if (pf == NULL)
  {
    printf("Error in opening file !!");
  }
  fwrite(&temp, sizeof(employee), 1, pf);
  printf("Employee with id  %d  is successfully added.\n", temp.id);
  fclose(pf);
  readinfo(emp);
}
int searchEmpid(employee emp[], int targetid)
{

  for (int i = 0; i < rec; i++)
  {
    if (emp[i].id == targetid)
    {
      return i;
    }
  }

  return -1;
}

void SearchEmployee(employee emp[])
{
  int i;
  int Reqid, Sid;
  char code;
  bool cont = false;
  do
  {

    fflush(stdin);
    printf("Enter the id of the employee you want to search : ");
    scanf("%d", &Reqid);
    Sid = searchEmpid(emp, Reqid);

    if (Sid != -1)
    {
      cont = false;
      printf("%-10s%-20s%-25s%-15s%-10s%-20s\n", "Empid", "Name", "Email", "Basic($)", "Pf($)", "Health insurance");
      printf("------------------------------------------------------------------------------------------------------------------------------\n");
      printf("%-10d%-20s%-25s%-15.2f%-10.2f  %-20.2f\n", emp[Sid].id, emp[Sid].name, emp[Sid].email, emp[Sid].salary, emp[Sid].Pf, emp[Sid].Hltins);
      printf("------------------------------------------------------------------------------------------------------------------------------\n");
    }

    else
    {
      printf("This employee id %d doesn't exist in record!!\n", Reqid);
      cont = true;
      fflush(stdin);
      printf("Press m/M for menu and any other key to again search employee : ");
      scanf("%c", &code);
      if (code == 'm' || code == 'M')
      {
        return;
      }
    }

  } while (cont);
}

void printEmp(employee emp[])
{
  float totalpf, totalhltins, totalsalary, totalnetsalary;
  employee temp;

  for (int i = 0; i < rec - 1; i++)
  {
    for (int j = 0; j < rec - i - 1; j++)
    {
      if (strncmp(emp[j].name,emp[j+1].name,1)>0)
      {
        temp = emp[j];
        emp[j] = emp[j + 1];
        emp[j + 1] = temp;
      }
    }
  }

  printf("%-10s%-20s%-25s%-15s%-10s%-20s%-10s\n", "Empid", "Name", "Email", "Basic($)", "Pf($)", "Health insurance($)", " Net($)");
  printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  for (int i = 0; i < rec; i++)
  {
    printf("%-10d%-20s%-25s%-15.2f%-10.2f   %-16.2f%-10.2f\n", emp[i].id, emp[i].name, emp[i].email, emp[i].salary, emp[i].Pf, emp[i].Hltins, Netsalary(emp[i]));
    printf("\n");
    totalpf += emp[i].Pf;
    totalsalary += emp[i].salary;
    totalhltins += emp[i].Hltins;
    totalnetsalary += Netsalary(emp[i]);
  }

  printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  printf("%-10s%-45s%-15.2f%-10.2f   %-16.2f%-10.2f\n", "Total", "In  ($):", totalsalary, totalpf, totalhltins, totalnetsalary);
}
float Netsalary(employee emp)
{
  return emp.salary - (emp.Pf + emp.Hltins);
}
void RemoveEmp(employee emp[])
{
  int Reqid;
  bool cont = false;
  char menucode;
  do
  {
    fflush(stdin);
    printf("\nEnter the employee id of the employee you want to delete from the record: ");
    scanf("%d", &Reqid);
    int delid = searchEmpid(emp, Reqid);
    if (delid == -1)
    {
      printf("This employee id doesn't exist in the record.\n");
      cont = true;
      printf("Press M/m to go to menu and any other to try again with correct id : ");
      menucode = getch();
      if (menucode =='M' || menucode == 'm')
      {
        return;
      }
    }
    else
    {
      cont = false;
      FILE *pf = fopen("emp.bin", "wb");
      if (pf == NULL)
      {
        printf("Error in opening file!!");
      }

      for (int i = 0; i < rec; i++)
      {

        if (i == delid)
        {
        }
        else
        {
          fwrite(&emp[i], sizeof(employee), 1, pf);
        }
      }

      fclose(pf);
      readinfo(emp);
      printf("Employee with id %d is succcessfully deleted!!\n", Reqid);
      printf("Now we have %d employees in record.\n", rec);
    }
  } while (cont);
}
void Updateinfo(employee emp[])
{

  int upid, Reqid, code;
  char menucode;
  bool cont = false;
  do
  {
    printf("Enter the id to update : ");
    scanf("%d", &Reqid);
    upid = searchEmpid(emp, Reqid);
    if (upid == -1)
    {
      printf("This employee id doesn't exist in the record.\n");
      printf("Press m/M for menu and any other key to try again:  ");

      menucode = getche();
      printf("\n");
      if (menucode == 'm' || menucode == 'M')
      {
        return;
      }
      cont = true;
    }

    else
    {
      cont = false;
      FILE *pf = fopen("emp.bin", "wb");

      printf("What do you want to change : \n");
      printf(" Enter (1) for Salary\n");
      printf(" Enter (2) for Pf\n");
      printf(" Enter (3) for Health insurance\n");
      printf(" Enter (4) for email\n");
      printf(" Enter code : ");
      scanf("%d", &code);

      while (code > 4 || code < 1)
      {
        printf("Wrong code,Valid code is from 1-4\n");
        printf(" Enter code : ");
        scanf("%d", &code);
      }

      for (int i = 0; i < rec; i++)
      {
        if (upid == i)
        {
          if (code == 1)
          {
            printf("What is the new salary of employee : ");
            scanf("%f", &emp[i].salary);
          }
          else if (code == 2)
          {
            printf("What is the new Pf of employee : ");
            scanf("%f", &emp[i].Pf);
          }

          else if (code == 3)
          {
            printf("What is the new Health insurance of employee : ");
            scanf("%f", &emp[i].Hltins);
          }
          else if (code == 4)
          {
            fflush(stdin);
            printf("What is the new Email of employee : ");
            scanf("%s", &emp[i].email);
          }
        }

        fwrite(&emp[i], sizeof(employee), 1, pf);
      }

      fclose(pf);
      readinfo(emp);
      printf("Employee with id %d is succcessfully updated \n", Reqid);
    }
  } while (cont);
}
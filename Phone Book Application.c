#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

/*structure to hold the information of a single contact*/
typedef struct
{
    char firstName[40];
    char lastName[40];
    char mobileNumber[20];
} Contact;

/* enumerator for different search types*/
typedef enum {FIRST_NAME, LAST_NAME, MOBILE_NUMBER} search_t;

/* array to hold our contacts*/
Contact phonebook[MAX_SIZE];

/*current size of the phone book array*/
int currentSize = 0;

/*function prototypes*/
void addEntry();
void display(int index);
void search(char *key, search_t type);
void saveToFile();
void readFromFile();
void removeEntry(char *mobileNumber);
void clear();

void strtolower(char *str);

int main()
{
    readFromFile();
    int i;
    char selection[41];
    int choice;
    while(1)
    {
        puts("===============================================");
        puts("*****PHONE BOOK APPLICATION MAIN MENU*****");
        puts("================================================");
        puts("    1.Insert a new entry");
        puts("    2.Display All Entry");
        puts("    3.Search By First Name");
        puts("    4.Search By Last Name");
        puts("    5.Search By Mobile Number");
        puts("    6.Delete a Contact (By Mobile Number)");
        puts("    7.Save Database");
        puts("    8.Exit");

        do
        {
            printf("Enter Your Choice : ");
            gets(selection);
            choice = atoi(selection);
        }
        while(choice < 1 || choice > 8);

        switch(choice)
        {
        case 1:
            addEntry();
            clear();
            break;
        case 2:
            if(currentSize == 0)
            {
                puts("Phonebook is empty !!!");
                return 0;
            }
            for(i=0; i<currentSize; i++)
            {
                display(i);
                puts("");//extra new line
            }
            clear();
            break;
        case 3:
            printf("Enter First Name to search : ");
            gets(selection);
            search(selection,FIRST_NAME);
            clear();
            break;
        case 4:
            printf("Enter Last Name to search : ");
            gets(selection);
            search(selection,LAST_NAME);
            clear();
            break;
        case 5:
            printf("Enter Mobile Number to search : ");
            gets(selection);
            search(selection,MOBILE_NUMBER);
            clear();
            break;
        case 6:
            printf("Enter mobile number to delete : ");
            gets(selection);
            removeEntry(selection);
            clear();
            break;
        case 7:
            saveToFile();
            clear();
            break;
        case 8:
            exit(0);
            break;
        default:
            break;
        }

    }


    getch();
}

void clear()
{
    system("PAUSE");// press any key to continue
    system("CLS");//clear the screen ( Linux :clear // turbo C : Clrscr )
}


void addEntry()
{
    if(currentSize == MAX_SIZE)
    {
        puts("Error:Phonebook is full!!");
        return;
    }

    Contact c;

    printf("Enter First Name : ");
    gets(c.firstName);
    printf("Enter Last Name : ");
    gets(c.lastName);
    printf("Enter Mobile Number : ");
    gets(c.mobileNumber);

    phonebook[currentSize] = c;
    ++currentSize;

    printf("\n Contact added successfully\n");

}


void display(int index)
{
    if(index<0 || index>=currentSize)
    {
        puts("Error:Invalid Index!!!");
        return;
    }

    Contact c = phonebook[index];

    printf("First Name : %s\n",c.firstName);
    printf("Last Name : %s\n",c.lastName);
    printf("Mobile Number : %s\n",c.mobileNumber);

}

/*
  *CAUTION:
  *This function modifies the given string
*/
void strtolower(char *str)
{
    int len = strlen(str);
    int i;
    for(i=0; i<len; i++)
    {
        str[i] = tolower(str[i]);
    }
}


void search(char *key,search_t type)
{
    int found = 0;
    int i;

    strtolower(key);
    char content[41];

    if(type == FIRST_NAME)  // search by first name
    {
        for(i=0; i<currentSize; i++)
        {
            strcpy(content,phonebook[i].firstName);
            strtolower(content);

            if(strcmp(content,key) == 0)
            {
                display(i);
                found = 1;
            }
        }
    }
    else if(type == LAST_NAME)  // search by last name
    {
        for(i=0; i<currentSize; i++)
        {
            strcpy(content,phonebook[i].lastName);
            strtolower(content);

            if(strcmp(content,key) == 0)
            {
                display(i);
                found = 1;
            }
        }
    }
    else if(type == MOBILE_NUMBER)  // search by mobile number
    {
        for(i=0; i<currentSize; i++)
        {
            strcpy(content,phonebook[i].mobileNumber);
            strtolower(content);

            if(strcmp(content,key) == 0)
            {
                display(i);
                found = 1;
            }
        }
    }
    else
    {
        puts("Error:Invalid search type!!!");
        return;
    }

    if(!found)
    {
        puts("Not Found in the phone book");
    }
}


void saveToFile()
{
    FILE *fp;
    if((fp = fopen("phonebook.db","wb")) == NULL)
    {
        puts("Error: Can't create a database file!!!");
        return;
    }

    /* save the current size of the phonebook*/
    if(fwrite(&currentSize,sizeof(currentSize),1,fp) != 1)
    {
        puts("Error: can't save data!!");
        return;
    }

    /* save the phonebook contents*/
    if(fwrite(phonebook,sizeof(phonebook),1,fp) != 1)
    {
        puts("Error: can't save data!!");
        return;
    }

    puts("Phonebook saved to file successfully!!!!");
}

// currentSize -= 1
//========================================
// 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
//========================================
void removeEntry(char *mobileNumber)
{
    if(currentSize == 0)
    {
        puts("Phonebook is empty!! Nothing to delete!!");
        return;
    }

    int i,j;
    int count = 0;
    for(i=0; i<currentSize; i++)
    {
        if(strcmp(phonebook[i].mobileNumber,mobileNumber)==0)
        {
            for(j=i; j<currentSize-1; j++)
            {
                phonebook[j] = phonebook[j+1];
            }

            strcpy(phonebook[j].firstName,"");
            strcpy(phonebook[j].lastName,"");
            strcpy(phonebook[j].mobileNumber,"");

            currentSize -= 1;
            ++count;
        }
    }
    if(count==0)
    {
        puts("No entry deleted");
    }
    else
    {
        printf("%d entries deleted\n", count);
    }
}



void readFromFile()
{
    FILE *fp;
    if((fp = fopen("phonebook.db","rb")) == NULL)
    {
        puts("Error: Can't load the database file!!!");
        return;
    }

    /* read the size of phonebook*/
    if(fread(&currentSize,sizeof(currentSize),1,fp) != 1)
    {
        puts("Error: can't read data!!");
        return;
    }

    /* read the actual phonebook contents*/
    if(fread(phonebook,sizeof(phonebook),1,fp) != 1)
    {
        puts("Error: can't read data!!");
        return;
    }
}

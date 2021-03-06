#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "Console Window.h"
#include "Menu.h"

#define MAX_CONTACTS 500
#define FIRST_NAME_SIZE 15
#define LAST_NAME_SIZE 15
#define ADDRESS_SIZE 40
#define PHONE_SIZE 15
#define EMAIL_SIZE 40
#define FILE_NAME_SIZE 100 // same as width of the frame

//Box frame
#define HEIGHT 20
#define WIDTH 50 // 50 is the minimum width so the program display info correctly
#define X_POS 3
#define Y_POS 4

typedef struct
{
    int day;
    int month;
    int year;
} date;


typedef struct
{
    char fname[FIRST_NAME_SIZE];
    char lname[LAST_NAME_SIZE];
    date DOB;
    char address[ADDRESS_SIZE];
    char phonum[PHONE_SIZE];
    char email[EMAIL_SIZE];
} contact;



int count = 0; //count used to count number of contacts in the entered text file
contact s[MAX_CONTACTS];// max number of conatcts
int file_exist = 0; // if 0 means no file is loaded into the program yet 1 means the opposite
int tempY_POS = 0; // holders for Y position used in function as temporary value


char* enterData(int max, int tempY_POS, HANDLE console);


int main()
{
    SetConsoleTitle("Phonebook");
    Console_Font_Size();
    mainMenu("No file"," ");
    return 0;
}



void mainMenu(char loadmessage[], char savemessage[])
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console,15);
    windowSize(HEIGHT, WIDTH-6); //WIDTH-6 related to the value 50 if changed -6 will not make it correct
    ShowConsoleCursor(0, console);

    drawTitle("Main");
    draw_box(HEIGHT,WIDTH,X_POS-1,Y_POS-1);
    gotoxy(X_POS, Y_POS-2);
    printf("%s  %s",loadmessage, savemessage);
    int selection = menu("Load View Search Add Delete Edit Sort Save Exit", HEIGHT, WIDTH);

    switch(selection)
    {
    case 0:
        system("cls");
        load(console);
        break;
    case 1:
        system("cls");
        printer(console);
    case 2:
        system("cls");
        Search(console);
        break;
    case 3:
        system("cls");
        Add(404,console);
        break;
    case 4:
        system("cls");
        Delete(console);
        break;
    case 5:
        system("cls");
        Modify(console);
        break;
    case 6:
        system("cls");
        Sort(X_POS, tempY_POS, console);
        break;
    case 7:
        system("cls");
        save_data(console);
        break;
    case 8:
        system("cls");
        EXIT(console);
        break;
    }
}



void gotoxy(int x, int y)
{
    COORD coord = {0,0};
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}



void draw_box(int height, int width, int x, int y)
{
    int i,j;
    //+-+
    gotoxy(x,y);
    printf("%c",201);
    for(i = 0; i < width; i++)
        printf("%c",205);
    printf("%c",187);

    //| |
    for(i = 0; i < height; i++)
    {
        gotoxy(x,++y);
        printf("%c",186);
        for(j = 0; j < width; j++)
            printf(" ");
        printf("%c\n",186);
    }

    // +-+
    gotoxy(x,++y);
    printf("%c",200);
    for(i = 0; i < width; i++)
        printf("%c",205);
    printf("%c",188);
}



void extend_box(int height, int width,int x,int y)
{
    if(y-Y_POS < HEIGHT)return;
    int i,j;
    //| |
    gotoxy(x,y);
    for(i = 0; i < height; i++)
    {

        printf("%c",186);
        for(j = 0; j < width; j++)
            printf(" ");
        printf("%c\n",186);
        gotoxy(x,++y);
    }
    // +-+
    printf("%c",200);
    for(i = 0; i < width; i++)
        printf("%c",205);
    printf("%c",188);
}



void drawTitle(char title[])
{
    draw_box(1,11,WIDTH-9,0);
    gotoxy(WIDTH-5,1);
    printf("%s",title);
}



int again(int tempY_POS, HANDLE console)
{
    extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
    gotoxy(X_POS, ++tempY_POS);


    printf("Do operation again?");
    gotoxy(X_POS, ++tempY_POS);
    int selection = menu("Yes No", tempY_POS*2+1, X_POS*2);
    return selection;
}



void check_file_exist(void)
{
    if(file_exist)return;
    else mainMenu("Load a file first!!\a"," ");
}



char* enterData(int max, int tempY_POS, HANDLE console)
{
    ShowConsoleCursor(1, console);
    int i = 0;
    int tempX_POS = X_POS;
    char *entered = malloc(max * sizeof(char));

    int cha;
    gotoxy(tempX_POS, tempY_POS);
    _cputs(">");
    gotoxy(++tempX_POS, tempY_POS);
    do
    {
        cha = _getch();
        if(cha == 13)break;

        // if to handle erasing
        if(cha == 8 && i == 0)continue;
        if(cha == 8)
        {

            if(tempX_POS > X_POS && tempX_POS-1 != X_POS)
            {
                gotoxy(--tempX_POS,tempY_POS);
                cha = ' ';
                _putch(cha);
                gotoxy(tempX_POS,tempY_POS);
                --i;
                continue;
            }
        }
        entered[i++] = (char)cha;
        _putch( cha );
        ++tempX_POS;
    }
    while(i < max);
    entered[i] = '\0';
    ShowConsoleCursor(0, console);
    return entered;
}



int enterNumber(char* number)
{
    int n = strlen(number);
    int z = 1;
    int i;
    for ( i = 0 ; i < n; i++ )
    {
        if(!isdigit(number[0]))
        {
            z = 0;
            break;
        }
    }
    return z;
}



void load(HANDLE console)
{
    drawTitle("Load");
    draw_box(3,WIDTH,X_POS-1,Y_POS-1);
    tempY_POS = Y_POS;
    gotoxy(X_POS,tempY_POS);
    printf("Enter file path or drag and drop the file here:");
    gotoxy(X_POS, ++tempY_POS);
    printf("Note:file name can be used in format(filename.txt)");
    gotoxy(X_POS, ++tempY_POS);
    printf("but file must be in same directory");

    //warnings messages
    ++tempY_POS;
    draw_box(3,WIDTH,X_POS-1,++tempY_POS);
    gotoxy(X_POS, ++tempY_POS);
    printf("Note:text in file must be in this format:");
    gotoxy(X_POS, ++tempY_POS);
    printf("first name,last name,day,month,year of birth,");
    gotoxy(X_POS, ++tempY_POS);
    printf("address,phone number,email");
    ++tempY_POS;
    gotoxy(X_POS, ++tempY_POS);

    char *filename = enterData(FILE_NAME_SIZE, tempY_POS, console);
    FILE *f;
    FILE *f2;
    f = fopen(filename,"r");
    f2 = fopen(filename,"r"); // temp file to count the lines from it
    free(filename);

    if(f!= NULL)
    {
        int lines = 0;
        char newLineReader;
        for (newLineReader = getc(f2); newLineReader != EOF; newLineReader = getc(f2))
        {
            if (newLineReader == '\n')
                lines++;
        }
        fclose(f2);

        if(lines > MAX_CONTACTS)
        {
        gotoxy(X_POS-1,Y_POS-2);
        printf("Error!!MAX contacts is %d!\a",MAX_CONTACTS);
        load(console);
        }

        while(!feof(f))
        {
            char temp;
            fscanf(f,"%[^,],",s[count].fname);
            fscanf(f,"%[^,],",s[count].lname);
            fscanf(f,"%d%c",&s[count].DOB.day,&temp);
            fscanf(f,"%d%c",&s[count].DOB.month,&temp);
            fscanf(f,"%d%c",&s[count].DOB.year,&temp);
            fscanf(f,"%[^,],",s[count].address);
            fscanf(f,"%[^,],",&s[count].phonum);
            fscanf(f,"%[^\n]\n",s[count].email);
            count++;
        }
        fclose(f);
        system("cls");
        file_exist = 1;
        mainMenu("File is loaded","Not saved!");
    }
    else
    {
        gotoxy(X_POS-1,Y_POS-2);
        printf("Error opening the file!!\a");
        load(console);
    }
}



void Search(HANDLE console)
{
    check_file_exist();
    drawTitle("Search");
    draw_box(HEIGHT,WIDTH,X_POS-1,Y_POS-1);

    tempY_POS = Y_POS;
    gotoxy(X_POS,tempY_POS);
    printf("Enter the last name: ");
    gotoxy(X_POS,++tempY_POS);
    char *input = enterData(LAST_NAME_SIZE, tempY_POS, console);
    int check = 0;
    int x,i;
    x = strlen(input);

    char srch[x];
    strcpy(srch,input);
    free(input);
    for(i = 0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            gotoxy(X_POS,++tempY_POS);
            if(check == 0)
            {
                printf("Found contacts:-");
                check = 1;
                gotoxy(X_POS,++tempY_POS);
            }
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf("Contact No.%d",i);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" First name: %s",s[i].fname);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" Address: %s",s[i].address);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" phone number: %s",s[i].phonum);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" E-mail: %s",s[i].email);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
        }
    }
    if(check == 1)
    {
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("Search is done!");
    }
    else
    {
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        printf("**Name is not found!!**\a");
    }

    // check if operation will be done again or do another operation
    int redo = again(tempY_POS, console);
    if(redo == 0)
    {
        Search(console);
    }
    else
    {
        system("cls");
        mainMenu("File is loaded","Not saved!");
    }
}



void Add(int f, HANDLE console)
{
    check_file_exist();
    drawTitle("Add");
    draw_box(HEIGHT+3,WIDTH,X_POS-1,Y_POS-1);

    if(f == 404)f = count;

    tempY_POS = Y_POS;
    gotoxy(X_POS, tempY_POS);


    char *input;
    printf("First name: ");
    gotoxy(X_POS, ++tempY_POS);
    input = enterData(FIRST_NAME_SIZE, tempY_POS, console);
    strcpy(s[f].fname,input);
    free(input);

    int valid = 0;
    // last name
    do
    {
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("Last name: ");
        gotoxy(X_POS,++tempY_POS);
        input = enterData(LAST_NAME_SIZE, tempY_POS, console);
        if(!strlen(input))
        {
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS, ++tempY_POS);
            printf("**Last name can not be empty**\a");
            valid = 0;
        }
        else
        {
            valid = 1;
            strcpy(s[f].lname,input);
        }
    }
    while(!valid);
    free(input);


    // Date of birth
    // char to hold the integer to check isdigit() function then atoi transfer them to int variables
    char* day;
    char* month;
    char* year;

    int dayValid = 0;
    int monthValid = 0;
    int yearValid = 0;
    valid = 0;

    do
    {
        gotoxy(X_POS,++tempY_POS);
        printf("Date Of Birth(day month year):");

        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("Day:");
        gotoxy(X_POS,++tempY_POS);
        day = enterData(3, tempY_POS, console);

        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("Month:");
        gotoxy(X_POS,++tempY_POS);
        month = enterData(3, tempY_POS, console);

        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("Year:");
        gotoxy(X_POS,++tempY_POS);
        year = enterData(5, tempY_POS, console);

        if(!strlen(day) || !strlen(month) || !strlen(year))
        {
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS, ++tempY_POS);
            printf("**some info are missing**\a");
            valid = 0;
        }

        dayValid  = enterNumber(day);
        monthValid = enterNumber(month);
        yearValid  = enterNumber(year);

        if(!dayValid || !monthValid || !yearValid )
        {
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS, ++tempY_POS);
            printf("**Invalid Date**\a");
            valid = 0;
        }
        else
        {
            valid = 1;
        }
        if((atoi(day) < 0 || atoi(day) > 31)||(atoi(month) > 12 || atoi(month) < 0)||(atoi(year) > 2021 || atoi(year) < 1900))
        {
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS, ++tempY_POS);
            printf("**Incorrect Date**\a");
            valid = 0;
        }
    }
    while(!valid);
    s[f].DOB.day = atoi(day);
    s[f].DOB.month = atoi(month);
    s[f].DOB.year = atoi(year);
    free(day);
    free(month);
    free(year);

    gotoxy(X_POS,++tempY_POS);
    printf("Address: ");
    gotoxy(X_POS,++tempY_POS);
    input = enterData(ADDRESS_SIZE, tempY_POS, console);
    strcpy(s[f].address,input);
    free(input);

    char* phone;
    valid = 0;
    do
    {
        int error1 = 0;
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("Phone number: ");
        gotoxy(X_POS,++tempY_POS);
        phone  = enterData(13, tempY_POS, console);
        valid = enterNumber(phone);
        if(!valid)
        {
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf("**Invalid number**\a");
            error1 = 1;
        }

        if((strlen(phone) < 11 || strlen(phone) > 11) && !error1)
        {
            valid = 0;
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf("**Phone number must be 11 digit**\a");
        }
    }
    while(!valid);
    strcpy(s[f].phonum, phone);
    free(phone);


    int check = 0, i = 0;
    char x[20], y = '@';

    while(check == 0)
    {
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("E-mail : ");
        gotoxy(X_POS,++tempY_POS);
        input = enterData(EMAIL_SIZE, tempY_POS, console);
        strcpy(s[f].email,input);
        free(input);

        strcpy(x,s[f].email);
        int z=strlen(x);

        for(i=0; i<z; i++)
        {
            if( x[i] == '@' && x[0] != '@')
            {
                for(++i ; i<z; ++i)
                {
                    if( x[i] == '.' )
                    {
                        check = 1;
                        break;
                    }
                }
            }
        }
        if(check == 0)
        {
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf("**Invalid email!!**\a");
        }
    }
    if(f == count)
    {
        count++;
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("**Add is done successfully!**");

        // check if operation will be done again or do another operation
        int redo = again(tempY_POS, console);
        if(redo == 0)
        {
            Add(404,console);
        }
        else
        {
            system("cls");
            mainMenu("File is loaded","Not saved!");
        }
    }
}



void Modify(HANDLE console)
{
    check_file_exist();
    drawTitle("Edit");
    ShowConsoleCursor(0, console);
    draw_box(HEIGHT, WIDTH, X_POS-1, Y_POS-1);
    tempY_POS = Y_POS;

    char *input;
    gotoxy(X_POS,tempY_POS);
    printf("Enter the last name: ");
    gotoxy(X_POS,++tempY_POS);
    input = enterData(EMAIL_SIZE, tempY_POS, console);

    int numberOfFound = 0;
    int i,x = strlen(input);

    char srch[x];
    strcpy(srch,input);
    free(input);
    for(i = 0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            if(numberOfFound == 0)
            {
                gotoxy(X_POS,++tempY_POS);
                gotoxy(X_POS,++tempY_POS);
                printf("Found contacts :-");
                gotoxy(X_POS,++tempY_POS);
            }
            numberOfFound++;
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf("Contact number %d",i);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" First name: %s",s[i].fname);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" Last name: %s",s[i].lname);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" Address: %s",s[i].address);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" phone number: %s",s[i].phonum);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" E-mail: %s",s[i].email);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
        }
    }

    int found[numberOfFound], z = 0;
    for(i=0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            found[z] = i;
            z++;
        }
    }

    if(numberOfFound)
    {
        int n;
        tempY_POS += 3;
        draw_box(3, WIDTH, X_POS-1, tempY_POS);
        int valid = 0;
        int exist = 0;
        char* editIndex;
        do
        {
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf("Enter contact number to Edit: ");
            gotoxy(X_POS,++tempY_POS);
            editIndex  = enterData(2 + (int)log10(count), tempY_POS, console); //1 + (int)log10(count) get the length of the total number of contacts
            valid = enterNumber(editIndex);
            if(!valid)
            {
                extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
                gotoxy(X_POS,++tempY_POS);
                printf("**Numbers only!**\a");
            }


            if(valid)
            {
                n = atoi(editIndex);
                for(i = 0; i < numberOfFound; i++)
                {

                    if(n == found[i])
                    {
                        exist = 1;
                    }
                }

                if(!exist)
                {
                    extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
                    gotoxy(X_POS,++tempY_POS);
                    printf("**Numbers does not exist!**\a");
                }
            }
        }
        while(!valid || !exist);
        free(editIndex);

        system("cls");
        Add(n, console);
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("Modification complete!");
    }
    else
    {

        gotoxy(X_POS,++tempY_POS);
        printf("**Name is not found!!**\a");
    }

    // check if operation will be done again or do another operation
    int redo = again(tempY_POS, console);
    if(redo == 0)
    {
        Modify(console);
    }
    else
    {
        system("cls");
        mainMenu("File is loaded","Not saved!");
    }
}



void Delete(HANDLE console)
{
    check_file_exist();
    drawTitle("Delete");
    ShowConsoleCursor(0, console);
    draw_box(HEIGHT, WIDTH, X_POS-1, Y_POS-1);
    tempY_POS = Y_POS;


    gotoxy(X_POS,tempY_POS);
    printf("Enter the last name: ");
    gotoxy(X_POS,++tempY_POS);
    char *input = enterData(LAST_NAME_SIZE, tempY_POS, console);

    int numberOfFound = 0;
    int i,x = strlen(input);

    char srch[x];
    strcpy(srch,input);
    free(input);

    for(i = 0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            if(numberOfFound == 0)
            {
                gotoxy(X_POS,++tempY_POS);
                printf("Found contacts :-");
            }
            numberOfFound++;
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf("Contact No.%d",i);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" First name: %s",s[i].fname);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" Last name: %s",s[i].lname);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" Address: %s",s[i].address);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" phone number: %s",s[i].phonum);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf(" E-mail: %s",s[i].email);
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);

        }
    }

    int found[numberOfFound]; // array with size of the found contacts to holds their index
    int z = 0;
// loop to assign the number of the found contacts in y array
    for(i = 0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            found[z] = i;
            z++;
        }
    }

    if(numberOfFound)
    {
        int n;
        int valid = 0;
        int exist = 0;
        char* deleteIndex;
        do
        {
            extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
            gotoxy(X_POS,++tempY_POS);
            printf("Enter contact number to delete: ");
            gotoxy(X_POS,++tempY_POS);
            deleteIndex  = enterData(2 + (int)log10(count), tempY_POS, console); //1 + (int)log10(count) get the length of the total number of contacts
            valid = enterNumber(deleteIndex);
            if(!valid)
            {
                extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
                gotoxy(X_POS,++tempY_POS);
                printf("**Numbers only!**\a");
            }

            if(valid)
            {
                n = atoi(deleteIndex);
                for(i = 0; i < numberOfFound; i++)
                {
                    if(n == found[i])
                    {
                        exist = 1;
                    }
                }
                if(!exist)
                {
                    extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
                    gotoxy(X_POS,++tempY_POS);
                    printf("**Numbers does not exist!**\a");
                }
            }
        }
        while(!valid || !exist);
        free(deleteIndex);

        // delete goes here
        for(i = n; i < count + 1; i++)
        {
            s[i] = s[i+1];
        }
        --count;


        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("Delete complete!");
    }
    else
    {
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("Name is not found!!\a");
    }


    // check if operation will be done again or do another operation
    int redo = again(tempY_POS, console);
    if(redo == 0)
    {
        system("cls");
        Delete(console);
    }
    else
    {
        system("cls");
        mainMenu("File is loaded","Not saved!");
    }
}



void printer(HANDLE console)
{
    check_file_exist();
    drawTitle("View");
    draw_box(HEIGHT,WIDTH,X_POS-1,Y_POS-1);
    tempY_POS = Y_POS;
    gotoxy(X_POS,tempY_POS);
    printf("My contacts:-");
    int i;
    for(i = 0; i < count; i++)
    {
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf("No.%d",i+1);
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf(" First Name: %s",s[i].fname);
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf(" Last Name: %s",s[i].lname);
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf(" Address: %s",s[i].address);
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf(" Phone Nember: %s",s[i].phonum);
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
        printf(" E-mail: %s",s[i].email);
        extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
        gotoxy(X_POS,++tempY_POS);
    }
    extend_box(3, WIDTH, X_POS-1,tempY_POS+1);
    gotoxy(X_POS,++tempY_POS);
    printf("Print is done!");
    gotoxy(X_POS,++tempY_POS);

    // check if operation will be done again or do another operation
    int redo = again(tempY_POS, console);
    if(redo == 0)
    {
        printer(console);
    }
    else
    {
        system("cls");
        mainMenu("File is loaded","Not saved!");
    }
}



void Sort(HANDLE console)
{
    check_file_exist();
    drawTitle("Sort");
    draw_box(HEIGHT/2,WIDTH,X_POS-1,Y_POS-1);
    tempY_POS = Y_POS;
    gotoxy(X_POS, tempY_POS);
    printf("A-sort by last name");
    gotoxy(X_POS, ++tempY_POS);
    printf("B-sort by data of birth");
    gotoxy(X_POS, ++tempY_POS);
    int selection = menu("A B", tempY_POS*2+1, X_POS*2);
    gotoxy(X_POS, ++tempY_POS);
    int i;
    int j;
    contact temp;
    if (selection == 0)
    {
        for (i = 0; i < count; i++)
        {
            for (j = i + 1; j < count; j++)
            {
                if (s[i].lname[0] > s[j].lname[0])
                {
                    temp= s[i];
                    s[i] = s[j];
                    s[j] = temp;
                }
            }
        }
    }
    else if (selection == 1)
    {
        for (i = 0; i < count; i++)
        {
            for (j = i + 1; j < count; j++)
            {
                if (s[i].DOB.year > s[j].DOB.year )
                {
                    temp = s[i];
                    s[i] = s[j];
                    s[j] = temp;
                }
                else if (s[i].DOB.year == s[j].DOB.year && s[i].DOB.month > s[j].DOB.month)
                {
                    temp = s[i];
                    s[i] = s[j];
                    s[j] = temp;
                }
                else if (s[i].DOB.year == s[j].DOB.year && s[i].DOB.month == s[j].DOB.month && s[i].DOB.day > s[j].DOB.day)
                {
                    temp = s[i];
                    s[i] = s[j];
                    s[j] = temp;
                }
            }
        }
    }

    if(selection == 0)
    {
        gotoxy(X_POS,++tempY_POS);
        printf("Sort by last name done successfully!");
    }
    if(selection == 1)
    {
        gotoxy(X_POS,++tempY_POS);
        printf("Sort by date of birth done successfully!");
    }
    // check if operation will be done again or do another operation
    int redo = again(tempY_POS, console);
    if(redo == 0)
    {
        Sort(console);
    }
    else
    {
        system("cls");
        mainMenu("File is Sorted", "Not saved!");
    }
}



void EXIT(HANDLE console)
{
    if(!file_exist)
    {
        draw_box(HEIGHT/4, WIDTH, X_POS-1, Y_POS-1);
        gotoxy((WIDTH/3),Y_POS+2);
        printf("program exit successfully!");
        tempY_POS = HEIGHT/2 + 2;
        gotoxy(X_POS,++tempY_POS);
        exit(0);
    }
    else
    {
        drawTitle("Exit");
        ShowConsoleCursor(0, console);
        draw_box(HEIGHT/2, WIDTH, X_POS-1, Y_POS-1);

        tempY_POS = Y_POS;
        char message1[] = "Any unsaved data will be lost!";
        char message2[] = "Are you sure you want to exit?";
        char message3[] = "program exit successfully!";
        gotoxy(((WIDTH-strlen(message1))/2)+2,tempY_POS);
        printf("%s",message1);
        gotoxy(((WIDTH-strlen(message2))/2)+2,++tempY_POS);
        printf("%s",message2);

        draw_box(2, WIDTH-10, X_POS+4, tempY_POS*2-2);
        int selection = menu("yes no", HEIGHT, WIDTH);

        if(selection == 0)
        {
            draw_box(2, WIDTH-10, X_POS+4, tempY_POS*2-2);
            gotoxy(((WIDTH-strlen(message3))/2)+2,tempY_POS*2-1);
            printf("%s",message3);
            tempY_POS = HEIGHT/2 + 2;
            gotoxy(X_POS, tempY_POS+2);
            exit(0);
        }
        else
        {
            mainMenu("File is loaded","Not saved!");
        }
    }
}



void save_data(HANDLE console)
{

    check_file_exist();
    drawTitle("Save");
    ShowConsoleCursor(0, console);
    draw_box(HEIGHT/2, WIDTH, X_POS-1, Y_POS-1);

    tempY_POS = Y_POS;

    gotoxy(X_POS,tempY_POS);
    printf("Name the file in which data will be saved: ");

    gotoxy(X_POS,++tempY_POS);
    char *input = enterData(FILE_NAME_SIZE, tempY_POS, console);

    char holder[strlen(input)];
    strcpy(holder,input);
    free(input);
    strcat(holder, ".txt");

    FILE*f;
    f = fopen(holder,"w");

    int i;
    for(i = 0; i<count; i++)
    {
        fprintf(f,"%s,",s[i].fname);
        fprintf(f,"%s,",s[i].lname);
        fprintf(f,"%d-%d-%d,", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
        fprintf(f,"%s,",s[i].address);
        fprintf(f,"%s,",s[i].phonum);
        fprintf(f,"%s\n",s[i].email);
    }
    gotoxy(X_POS,++tempY_POS);
    printf("Your work has been saved!");
    fclose(f);

    gotoxy(X_POS,++tempY_POS);
    printf("Exit the program?");
    int selection = menu("yes no", HEIGHT, WIDTH);
    if(selection == 0)
    {
        char message3[] = "program exit successfully!";
        draw_box(2, WIDTH-10, X_POS+4, tempY_POS*2-5);
        gotoxy(((WIDTH-strlen(message3))/2)+2,tempY_POS*2-4);
        printf("%s",message3);
        tempY_POS = HEIGHT/2 + 3;
        gotoxy(X_POS,++tempY_POS);
        exit(0);

    }
    else
    {
        mainMenu("File is loaded","Saved!");
    }
}

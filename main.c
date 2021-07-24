#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "Console Window.h"
#include "Menu.h"

#define FIRST_NAME_SIZE 15
#define LAST_NAME_SIZE 15
#define ADDRESS_SIZE 40
#define PHONE_NO_SIZE 12
#define EMAIL_SIZE 40
#define FILE_NAME_SIZE 50 // same as width of the frame

//Box frame
#define HEIGHT 20
#define WIDTH 50 // 50 is the minimum width so the program display info correctly
#define X_POS 3
#define Y_POS 4

/// problems in code:-
/// 1- read integers for delete function crash if user enter chars
/// 2- erase after typing in the enterdata() function
/// 3- extend box() & check boundry() are not applied in the hole functions only in search() function
/// 4- read file path because enterdata() function prevent long names as I make it do
/// 5-
/// 6-

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
    char phonum[PHONE_NO_SIZE];
    char email[EMAIL_SIZE];
} contact;



int count = 0; //count used to count number of contacts in the entered text file
contact s[100];// max number of conatcts
int file_exist = 0; // if 0 means no file is loaded into the program yet 1 means the opposite
int tempheight; // holders for height used in function as temporary value
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
    printf("%s",loadmessage);
    int selection = menu("Load Search Add Delete Modify Print Save Exit", HEIGHT, WIDTH);

    switch(selection)
    {
    case 0:
        system("cls");
        load(console);
        break;
    case 1:
        Search(console);
        break;
    case 2:
        Add(404,console);
        break;
    case 3:
        Delete(console);
        break;
    case 4:
        Modify(console);
        break;
    case 5:
        printer(console);
        break;
    case 6:
        system("cls");
        save_data(console);
        break;
    case 7:
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



int checkBoundary(int addToHeight,int currentY_POS,int currentX_POS, int currentBoxHeight)
{
    int i;
    int temp = currentY_POS;
    for(i = 1; i <= addToHeight ; i++)
    {
        if(temp - (Y_POS-1) > currentBoxHeight) // (Y_POS-1) here because the y position count from 0
        {
            extend_box(addToHeight, WIDTH, currentX_POS-1, currentY_POS);
            return currentBoxHeight + addToHeight;
        }
        temp++;
    }
    return currentBoxHeight;
}



void extend_box(int height, int width,int x,int y)
{
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
    // gotoxy(x,++y);
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



int again(int tempY_POS, int tempheight, HANDLE console)
{
    gotoxy(X_POS, ++tempY_POS);
    tempheight = checkBoundary(5, tempY_POS, X_POS, tempheight);
    printf("Do operation again?");
    gotoxy(X_POS, ++tempY_POS);
    int selection = menu("Yes No", tempY_POS*2+1, X_POS*2);
    return selection;
}



void check_file_exist(void)
{
    if(file_exist)return;
    else mainMenu("Load a file first!!\a\a"," ");
}



char* enterData(int max, int tempY_POS, HANDLE console)
{
    ShowConsoleCursor(1, console);
    int i = 0;
    int tempX_POS = X_POS+1;
    char *entered = malloc(max * sizeof(char));


    int cha;
    gotoxy(X_POS,tempY_POS);
    _cputs(">");
    gotoxy(X_POS+1,tempY_POS);
    do
    {
        ++tempX_POS;
        cha = _getch();
        //if to handle the enter button and if backspace is pressed at the beginning
        if(i == 0 && cha == 8)continue;
        if(cha == 13)break;

        // if to handle erasing
        if(cha == 8 && i != 0)
        {
            cha = ' ';
            --tempX_POS;
            gotoxy(--tempX_POS,tempY_POS);
            _putch(cha);
            gotoxy(tempX_POS,tempY_POS);
            continue;
        }
        entered[i++] = (char)cha;
        _putch( cha );
    }
    while(i < max);
    entered[i] = '\0';
    ShowConsoleCursor(0, console);
    return entered;
}



void load(HANDLE console)
{
    drawTitle("Load");
    draw_box(HEIGHT/2,WIDTH,X_POS-1,Y_POS-1);
    gotoxy(X_POS, (HEIGHT/2)-2);
    printf("Note that:text in file must be in this format:");
    gotoxy(X_POS, (HEIGHT/2)-1);
    printf(" first name,last name,day,month,year of birth,");
    gotoxy(X_POS, (HEIGHT/2));
    printf(" address,phone number,email");

    tempY_POS = Y_POS;
    FILE *f;
    gotoxy(X_POS,tempY_POS);
    printf("Please enter file name in format(filename.txt)");
    tempY_POS+=2;
    gotoxy(X_POS,tempY_POS);
    char *filename = enterData(FILE_NAME_SIZE, tempY_POS, console);
    f = fopen(filename,"r");
    free(filename);

    if(f!= NULL)
    {
        while(!feof(f))
        {
            char temp;
            fscanf(f,"%[^,],",s[count].fname);
            fscanf(f,"%[^,],",s[count].lname);
            fscanf(f,"%d%c",&s[count].DOB.day,&temp);
            fscanf(f,"%d%c",&s[count].DOB.month,&temp);
            fscanf(f,"%d%c",&s[count].DOB.year,&temp);
            fscanf(f,"%[^,],",s[count].address);
            fscanf(f,"%[^,],",s[count].phonum);
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

    tempheight = HEIGHT;
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

            tempheight = checkBoundary(8,tempY_POS,X_POS,tempheight);

            gotoxy(X_POS,++tempY_POS);
            printf("-First name: %s",s[i].fname);

            gotoxy(X_POS,++tempY_POS);
            printf(" Last name: %s",s[i].lname);

            gotoxy(X_POS,++tempY_POS);
            printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);

            gotoxy(X_POS,++tempY_POS);
            printf(" Address: %s",s[i].address);

            gotoxy(X_POS,++tempY_POS);
            printf(" phone number: %s",s[i].phonum);

            gotoxy(X_POS,++tempY_POS);
            printf(" E-mail: %s",s[i].email);
        }
    }
    if(check == 1)
    {
        tempheight = checkBoundary(2, tempY_POS, X_POS, tempheight);
        gotoxy(X_POS,++tempY_POS);
        //  tempheight = checkBoundary(3,tempY_POS,X_POS,tempheight);
        printf("Search is done! Y:%d,h:%d",tempY_POS,tempheight);
    }
    else
    {
        tempheight = checkBoundary(2, tempY_POS, X_POS, tempheight);
        gotoxy(X_POS,++tempY_POS);
        // tempheight = checkBoundary(2,tempY_POS,X_POS,tempheight);
        printf("**Name is not found!!**\a");
    }

    // check if operation will be done again or do another operation
    tempheight = checkBoundary(4, tempY_POS, X_POS, tempheight);
    int redo = again(tempY_POS, tempheight, console);
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
    draw_box(HEIGHT,WIDTH,X_POS-1,Y_POS-1);

    if(f == 404)f = count;

    tempheight = HEIGHT;
    tempY_POS = Y_POS;
    gotoxy(X_POS,tempY_POS);

    char *input;
    printf("First name: ");
    gotoxy(X_POS,++tempY_POS);
    input = enterData(FIRST_NAME_SIZE, tempY_POS, console);
    strcpy(s[f].fname,input);
    free(input);

    gotoxy(X_POS,++tempY_POS);
    printf("Last name: ");
    gotoxy(X_POS,++tempY_POS);
    input = enterData(LAST_NAME_SIZE, tempY_POS, console);
    strcpy(s[f].lname,input);
    free(input);

    do
    {
        if(tempY_POS >= tempheight)
        {
            extend_box(3,WIDTH,X_POS-1,tempY_POS);
            tempheight += 3;
        }
        gotoxy(X_POS,++tempY_POS);
        printf("Date Of Birth(day month year):");
        gotoxy(X_POS,++tempY_POS);
        printf(">");
        scanf("%d%d%d", &s[f].DOB.day, &s[f].DOB.month, &s[f].DOB.year);
    }
    while((s[f].DOB.day < 0 || s[f].DOB.day > 31)||(s[f].DOB.month > 12 || s[f].DOB.month < 0)||(s[f].DOB.year > 2020 || s[f].DOB.year < 1900));

    gotoxy(X_POS,++tempY_POS);
    printf("Address: ");
    gotoxy(X_POS,++tempY_POS);
    input = enterData(ADDRESS_SIZE, tempY_POS, console);
    strcpy(s[f].address,input);
    free(input);

    int z = 0;
    do
    {
        if(tempY_POS >= tempheight)
        {
            extend_box(3,WIDTH,X_POS-1,tempY_POS);
            tempheight += 3;
        }
        gotoxy(X_POS,++tempY_POS);
        printf("Phone number:");
        gotoxy(X_POS,++tempY_POS);
        printf(">");
        scanf("%11s", s[f].phonum);

        int n = strlen(s[f].phonum);
        z = 0 ;
        int x;
        for ( x =0 ; x < n; x++ )
        {
            if(!isdigit(s[f].phonum[x]))
            {
                z = 1;
                if(tempY_POS >= tempheight)
                {
                    extend_box(3,WIDTH,X_POS-1,tempY_POS);
                    tempheight += 3;
                }
                gotoxy(X_POS,++tempY_POS);
                printf("**Invalid Phone number**\a");
                break;
            }
        }
    }
    while(z);


    int check=0, i=0;
    char x[20],y='@';

    while(check == 0)
    {
        if(tempY_POS >= tempheight)
        {
            extend_box(3,WIDTH,X_POS-1,tempY_POS);
            tempheight += 3;
        }
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
            if(tempY_POS >= tempheight)
            {
                extend_box(3, WIDTH, X_POS-1, tempY_POS);
                tempheight += 3;
            }
            gotoxy(X_POS,++tempY_POS);
            printf("**Invalid email!!**\a");
        }
    }
    if(f == count)
    {
        count++;
        if(tempY_POS >= tempheight)
        {
            extend_box(3, WIDTH, X_POS-1, tempY_POS);
            tempheight += 3;
        }
        gotoxy(X_POS,++tempY_POS);
        printf("**Add is done successfully!**");

        // check if operation will be done again or do another operation
        int redo = again(tempY_POS, tempheight, console);
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
    drawTitle("Modify");
    ShowConsoleCursor(0, console);
    draw_box(HEIGHT, WIDTH, X_POS-1, Y_POS-1);

    tempheight = HEIGHT;
    tempY_POS = Y_POS;
    char *input;
    gotoxy(X_POS,tempY_POS);
    printf("Enter the last name: ");
    gotoxy(X_POS,++tempY_POS);
    input = enterData(EMAIL_SIZE, tempY_POS, console);

    int check=0;
    int i,x = strlen(input);

    char srch[x];
    strcpy(srch,input);
    free(input);
    for(i = 0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            if(check == 0)
            {
                gotoxy(X_POS,++tempY_POS);
                gotoxy(X_POS,++tempY_POS);
                printf("Found contacts :-");
                gotoxy(X_POS,++tempY_POS);
            }
            check++;
            if(i*9 > tempY_POS)
            {
                extend_box(9, WIDTH, X_POS-1, tempY_POS);
                tempheight += 9;
            }
            gotoxy(X_POS,++tempY_POS);
            printf("Contact number %d",i);
            gotoxy(X_POS,++tempY_POS);
            printf(" First name: %s",s[i].fname);
            gotoxy(X_POS,++tempY_POS);
            printf(" Last name: %s",s[i].lname);
            gotoxy(X_POS,++tempY_POS);
            printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
            gotoxy(X_POS,++tempY_POS);
            printf(" Address: %s",s[i].address);
            gotoxy(X_POS,++tempY_POS);
            printf(" phone number: %s",s[i].phonum);
            gotoxy(X_POS,++tempY_POS);
            printf(" E-mail: %s",s[i].email);
            gotoxy(X_POS,++tempY_POS);
        }
    }

    int y[check], z = 0;
    for(i=0; i<count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            y[z] = i;
            z++;
        }
    }

    if(check)
    {
        int n;
        z = 1;
        tempY_POS += 3;
        draw_box(3, WIDTH, X_POS-1, tempY_POS);
        while(z && !isdigit(n))
        {
            gotoxy(X_POS,tempY_POS+2);
            ShowConsoleCursor(1, console);
            printf("Please a Enter valid Contact number: ");
            scanf("%d",&n);
            ShowConsoleCursor(0, console);
            for(i=0; i<check; i++)
            {
                if(n == y[i]) z = 0;
            }
            tempY_POS -= 2;
        }
        system("cls");
        Add(n, console);
        if(tempY_POS >= tempheight)
        {
            extend_box(3, WIDTH, X_POS-1, tempY_POS);
            tempheight += 3;
        }
        gotoxy(X_POS,++tempY_POS);
        printf("Modification complete!");
    }
    else
    {

        gotoxy(X_POS,++tempY_POS);
        printf("**Name is not found!!**\a");
    }

    // check if operation will be done again or do another operation
    int redo = again(tempY_POS, tempheight, console);
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

    tempheight = HEIGHT;
    tempY_POS = Y_POS;


    gotoxy(X_POS,tempY_POS);
    printf("Enter the last name: ");
    gotoxy(X_POS,++tempY_POS);
    char *input = enterData(LAST_NAME_SIZE, tempY_POS, console);

    int check = 0;
    int i,x = strlen(input);

    char srch[x];
    strcpy(srch,input);
    free(input);
    for(i = 0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            if(check == 0)
            {
                gotoxy(X_POS,++tempY_POS);
                printf("Found contacts :-");
            }
            check++;
            if(i*9 > tempY_POS)
            {
                extend_box(9,WIDTH,X_POS-1,tempY_POS);
                tempheight += 10;
            }

            gotoxy(X_POS,++tempY_POS);
            printf("No.%d",i);
            gotoxy(X_POS,++tempY_POS);
            printf(" First name: %s",s[i].fname);
            gotoxy(X_POS,++tempY_POS);
            printf(" Last name: %s",s[i].lname);
            gotoxy(X_POS,++tempY_POS);
            printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
            gotoxy(X_POS,++tempY_POS);
            printf(" Address: %s",s[i].address);
            gotoxy(X_POS,++tempY_POS);
            printf(" phone number: %s",s[i].phonum);
            gotoxy(X_POS,++tempY_POS);
            printf(" E-mail: %s",s[i].email);
            gotoxy(X_POS,++tempY_POS);
            printf("\n");
        }
    }

    int y[check], z = 0;
    for(i=0; i<count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            y[z] = i;
            z++;
        }
    }

    if(check)
    {
        int n;
        z = 1;
        gotoxy(X_POS,++tempY_POS);
        while(z)
        {
            gotoxy(X_POS,tempY_POS);
            printf("Please a Enter valid Contact number: ");
            ShowConsoleCursor(1, console);
            scanf("%d",&n);
            ShowConsoleCursor(0, console);

            for(i=0; i<check; i++)
            {
                if(n == y[i]) z = 0;
                else
                {
                    break;
                    --tempY_POS;
                    printf("/a");

                }
            }
        }
        count--;
        contact p[count];
        z = 0;
        for(i = 0; i < count + 1; i++)
        {
            if(i != n )
            {
                p[z] = s[i];
                z++;
            }
        }
        for(i = 0; i < count + 1; i++)
        {
            s[i] = p[i];
        }
        if(tempY_POS+5 >= tempheight)
        {
            extend_box(5,WIDTH,X_POS-1,tempY_POS);
            tempheight += 4;
        }
        gotoxy(X_POS,++tempY_POS);
        printf("Delete complete!");
    }
    else
    {
        gotoxy(X_POS,++tempY_POS);
        printf("Name is not found!!\a");
    }

    // check if operation will be done again or do another operation
    int redo = again(tempY_POS, tempheight, console);
    if(redo == 0)
    {
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
    drawTitle("Print");
    draw_box(HEIGHT,WIDTH,X_POS-1,Y_POS-1);

    tempheight = HEIGHT;
    tempY_POS = Y_POS;
    Sort(X_POS, tempY_POS, console);
    tempY_POS += 3;
    int i;
    for(i = 0; i < count; i++)
    {
        if((tempY_POS+8) >= tempheight)
        {
            extend_box(8,WIDTH,X_POS-1,tempY_POS);
            tempheight += 8;
        }
        gotoxy(X_POS,++tempY_POS);
        printf("No.%d",i+1);
        gotoxy(X_POS,++tempY_POS);
        printf(" First Name: %s",s[i].fname);
        gotoxy(X_POS,++tempY_POS);
        printf(" Last Name: %s",s[i].lname);
        gotoxy(X_POS,++tempY_POS);
        printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
        gotoxy(X_POS,++tempY_POS);
        printf(" Address: %s",s[i].address);
        gotoxy(X_POS,++tempY_POS);
        printf(" Phone Nember: %s",s[i].phonum);
        gotoxy(X_POS,++tempY_POS);
        printf(" E-mail: %s",s[i].email);
        gotoxy(X_POS,++tempY_POS);
    }
    if((tempY_POS+6) >= tempheight)
    {
        extend_box(6, WIDTH, X_POS-1, tempY_POS);
        tempheight += 6;
    }
    gotoxy(X_POS,++tempY_POS);
    printf("Print is done!");
    gotoxy(X_POS,++tempY_POS);

    // check if operation will be done again or do another operation
    int redo = again(tempY_POS, tempheight, console);
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



void Sort(int x, int y, HANDLE console)
{
    ShowConsoleCursor(0, console);
    int j;
    char choise;
    gotoxy(x,y);
    printf("A/Sort by last name\n");
    gotoxy(x,++y);
    printf("B/Sort by date of birth\n");
    do
    {
        gotoxy(x,++y);
        printf("Enter your choise: ");
        ShowConsoleCursor(1, console);
        scanf("%s",&choise);
        int i;
        int j;
        contact temp;
        if (choise == 'A' || choise == 'a')
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
        else if (choise == 'B' || choise == 'b')
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
        else
        {
            gotoxy(x,++y);
            printf("**please enter 'A' or 'B' only**\a");
            y -= 2;
            gotoxy(x,y);
        }
    }
    while(choise != 'A' && choise != 'B' && choise != 'a' && choise != 'b');
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


        tempheight = HEIGHT;
        tempY_POS = Y_POS;
        gotoxy(X_POS,tempY_POS);
        printf("Any unsaved data will be lost!");
        gotoxy(X_POS,++tempY_POS);
        printf("Are you sure you want to exit?y/n");


        char    again = ' ';
        do
        {
            if((tempY_POS+4) >= tempheight)
            {
                extend_box(4, WIDTH, X_POS-1, tempY_POS);
                tempheight += 4;
            }
            gotoxy(X_POS,++tempY_POS);
            ShowConsoleCursor(1, console);
            scanf("%s",&again);
            ShowConsoleCursor(0, console);

            if(again == 'y')
            {
                gotoxy(X_POS,++tempY_POS);
                printf("program exit successfully!");
                tempY_POS = HEIGHT/2 + 2;
                gotoxy(X_POS, ++tempY_POS);
                exit(0);
            }
            else if(again == 'n')
            {
                break;
            }
            else
            {
                if(tempY_POS >= tempheight)
                {
                    extend_box(3, WIDTH, X_POS-1, tempY_POS);
                    tempheight += 3;
                }
                gotoxy(X_POS, ++tempY_POS);
                printf("Enter 'y' or 'n' only!\a");
                tempY_POS -= 2;
            }
        }
        while(again != 'y' && again != 'n');
        mainMenu("File is loaded","Not saved!");
    }
}



void save_data(HANDLE console)
{

    check_file_exist();
    drawTitle("Save");
    ShowConsoleCursor(0, console);
    draw_box(HEIGHT/2, WIDTH, X_POS-1, Y_POS-1);

    tempheight = HEIGHT/2;
    tempY_POS = Y_POS;

    gotoxy(X_POS,tempY_POS);
    printf("Name the file in which data will be saved: ");
    gotoxy(X_POS,++tempY_POS);
    printf("e.g(filename.txt)");

    gotoxy(X_POS,++tempY_POS);
    char *input = enterData(FILE_NAME_SIZE, tempY_POS, console);
    FILE*f;
    f = fopen(input,"w");
    free(input);
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

    int redo = again(tempY_POS, tempheight, console);
    if(redo == 1)
    {
        mainMenu("File is loaded","Saved!");
    }
    else
    {
        tempY_POS +=5;

        int tempX_POS = WIDTH/2;
        gotoxy(tempX_POS, tempY_POS);
        printf("program exit successfully!");
        tempY_POS = tempheight + 4;
        gotoxy(X_POS,++tempY_POS);
        exit(0);
    }
}

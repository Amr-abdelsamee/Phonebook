#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define FIRST_NAME_SIZE 15
#define LAST_NAME_SIZE 15
#define ADDRESS_SIZE 40
#define PHONE_NO_SIZE 12
#define EMAIL_SIZE 40
#define FILE_NAME_SIZE 20



typedef struct
{
    int day;
    int month;
    int year;
} date;


typedef struct
{
    char fname[15];
    char lname[15];
    date DOB;
    char address[40];
    char phonum[12];
    char email[30];
} contact;


int i,j,x;  // i,j,x are temporary values
int count = 0; //count used to count number of contacts in the entered text file
contact s[100];// max number of conatcts
int file_exist = 0; // if 0 means no file is loaded into the program yet 1 means the opposite
int xpos = 3; // X position of the borders box
int ypos= 4; // Y position of the borders box
int height = 20;// height of borders box
int width = 47; // width of borders box
int tempheight; // holders for height used in function as temporary value
int tempypos = 0; // holders for Y position used in function as temporary value

char* enterData(int max, int tempypos, HANDLE console);


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
    consoleWindow();
    ShowConsoleCursor(0, console);

    drawTitle("Main");
    int tempheight = height - 3;
    int pos = 1;
    char clicked;
    draw_box(height,width,xpos-1,ypos-1);
    gotoxy(3,2);
    printf("%s",loadmessage);
    if(strcmp(savemessage," "))
    {
        printf(" - %s",savemessage);
    }
    do
    {
        SetConsoleTextAttribute(console,15);
        gotoxy(width/2,tempheight/2);
        printf("[1] Load");
        gotoxy(width/2,(tempheight/2)+1);
        printf("[2] Search");
        gotoxy(width/2,(tempheight/2)+2);
        printf("[3] Add");
        gotoxy(width/2,(tempheight/2)+3);
        printf("[4] Delete");
        gotoxy(width/2,(tempheight/2)+4);
        printf("[5] Modify");
        gotoxy(width/2,(tempheight/2)+5);
        printf("[6] Print");
        gotoxy(width/2,(tempheight/2)+6);
        printf("[7] Save");
        gotoxy(width/2,(tempheight/2)+7);
        printf("[8] Exit");

        tempheight = height - 3;
        switch(pos)
        {
        case 1:
            SetConsoleTextAttribute(console,240);
            gotoxy(width/2,tempheight/2);
            printf("[1] Load");
            break;
        case 2:
            SetConsoleTextAttribute(console,240);
            gotoxy(width/2,(tempheight/2)+1);
            printf("[2] Search");
            break;
        case 3:
            SetConsoleTextAttribute(console,240);
            gotoxy(width/2,(tempheight/2)+2);
            printf("[3] Add");
            break;
        case 4:
            SetConsoleTextAttribute(console,240);
            gotoxy(width/2,(tempheight/2)+3);
            printf("[4] Delete");
            break;
        case 5:
            SetConsoleTextAttribute(console,240);
            gotoxy(width/2,(tempheight/2)+4);
            printf("[5] Modify");
            break;
        case 6:
            SetConsoleTextAttribute(console,240);
            gotoxy(width/2,(tempheight/2)+5);
            printf("[6] Print");
            break;
        case 7:
            SetConsoleTextAttribute(console,240);
            gotoxy(width/2,(tempheight/2)+6);
            printf("[7] Save");
            break;
        case 8:
            SetConsoleTextAttribute(console,240);
            gotoxy(width/2,(tempheight/2)+7);
            printf("[8] Exit");
            break;
        }
        clicked = getch();
        if(clicked == 72)//if up arrow
        {
            pos--;
            if(pos == 0) pos = 8;
        }
        if(clicked == 80)// if down arrow
        {
            pos++;
            if(pos == 9)
                pos = 1;
        }
    }
    while(clicked != 13);// 13 represent the enter key
    SetConsoleTextAttribute(console,15);

    switch(pos)
    {
    case 1:
        system("cls");
        load(console);
        break;
    case 2:
        Search(console);
        break;
    case 3:
        Add(404,console);
        break;
    case 4:
        Delete(console);
        break;
    case 5:
        Modify(console);
        break;
    case 6:
        printer(console);
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
    int i;
    //| |
    gotoxy(x,y);
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



void drawTitle(char title[])
{
    draw_box(1,11,38,0);
    gotoxy(42,1);
    printf("%s",title);
}



int checkBoundary(int addToHeight,int currentYpos,int currentXpos, int currentBoxHeight)
{
    int i;
    int temp = currentYpos;
    for(i = 1; i <= addToHeight ; i++)
    {
        if(temp - (ypos-1) > currentBoxHeight) // (ypos-1) here because the y position count from 0
        {
            extend_box(addToHeight, width, currentXpos-1, currentYpos);
            return currentBoxHeight + addToHeight;
        }
        temp++;
    }
    return currentBoxHeight;
}



// check for yes or no respond only
int checkRespond(char x[])
{
    if(strlen(x) > 1)
        return 0;
    else if(x[0] == 'y' || x[0] == 'Y')
        return 1;
    else if(x[0] == 'n' || x[0] == 'N')
        return 2;
    else
        return 0;
}



int again(int tempypos, int tempheight, HANDLE console)
{
    gotoxy(xpos,++tempypos);
    tempheight = checkBoundary(3,tempypos,xpos,tempheight);
    printf("Do the operation again?y/n");
    char respond[44];
    int again;
    gotoxy(xpos,++tempypos);
    do
    {
        gotoxy(xpos,tempypos);
        ShowConsoleCursor(1, console);
        tempheight = checkBoundary(3,tempypos,xpos,tempheight);
        printf(">");
        scanf("%s",&respond);
        ShowConsoleCursor(0, console);
        again = checkRespond(respond);
        if(!again)
        {
            gotoxy(xpos,++tempypos);
            tempheight = checkBoundary(3,tempypos,xpos,tempheight);
            gotoxy(xpos, --tempypos);
            printf("\t\t\t\t\t\t\t\a");
            gotoxy(xpos, tempypos);
        }
    }
    while(!again);
    return again;
}



void check_file_exist(void)
{
    if(file_exist)return;
    else mainMenu("Load a file first!!\a\a"," ");
}



char* enterData(int max, int tempypos, HANDLE console)
{
    ShowConsoleCursor(1, console);
    int i = 0;
    int tempxpos = xpos+1;
    char *entered = malloc(max * sizeof(char));


    int cha;
    gotoxy(xpos,tempypos);
    _cputs(">");
    gotoxy(xpos+1,tempypos);
    do
    {
        ++tempxpos;
        cha = _getch();
        //if to handle the enter button and if backspace is pressed at the beginning
        if(i == 0 && cha == 8)continue;
        if(cha == 13)break;

        // if to handle erasing
        if(cha == 8 && i != 0)
        {
            cha = ' ';
            --tempxpos;
            gotoxy(--tempxpos,tempypos);
            _putch(cha);
            gotoxy(tempxpos,tempypos);
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
    draw_box(height/2,width,xpos-1,ypos-1);
    tempypos = ypos;
    FILE *f;
    gotoxy(xpos,tempypos);
    printf("Please enter file name in format(filename.txt)");
    gotoxy(xpos,++tempypos);
    ShowConsoleCursor(1, console);

    gotoxy(xpos,++tempypos);
    char *filename = enterData(FILE_NAME_SIZE, tempypos, console);
    f = fopen(filename,"r");
    free(filename);
    char temp[20],*token;
    if(f!= NULL)
    {
        while(!feof(f))
        {
            fscanf(f,"%[^,], ",s[count].fname);
            fscanf(f,"%[^,], ",s[count].lname);
            fscanf(f,"%[^,], ",temp);
            token = strtok(temp, "-");
            s[count].DOB.day = atoi(token);
            token = strtok(NULL, "-");
            s[count].DOB.month = atoi(token);
            token = strtok(NULL, ",");
            s[count].DOB.year = atoi(token);
            fscanf(f,"%[^,], ",s[count].address);
            fscanf(f,"%[^,], ",s[count].phonum);
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
        gotoxy(xpos-1,ypos-2);
        printf("Error opening the file!!\a");
        load(console);
    }
}


void Search(HANDLE console)
{
    check_file_exist();
    drawTitle("Search");
    draw_box(height,width,xpos-1,ypos-1);
    tempheight = height;
    tempypos = ypos;
    gotoxy(xpos,tempypos);
    printf("Enter the last name: ");
    gotoxy(xpos,++tempypos);
    char *input = enterData(LAST_NAME_SIZE, tempypos, console);
    int check = 0;
    x = strlen(input);

    char srch[x];
    strcpy(srch,input);
    free(input);
    for(i = 0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            gotoxy(xpos,++tempypos);
            if(check == 0)
            {
                printf("Found contacts:-");
                check = 1;
                gotoxy(xpos,++tempypos);
            }

            tempheight = checkBoundary(8,tempypos,xpos,tempheight);

            gotoxy(xpos,++tempypos);
            printf("-First name: %s",s[i].fname);
            gotoxy(xpos,++tempypos);
            printf(" Last name: %s",s[i].lname);
            gotoxy(xpos,++tempypos);
            printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
            gotoxy(xpos,++tempypos);
            printf(" Address: %s",s[i].address);
            gotoxy(xpos,++tempypos);
            printf(" phone number: %s",s[i].phonum);
            gotoxy(xpos,++tempypos);
            printf(" E-mail: %s",s[i].email);
        }
    }
    if(check == 1)
    {
        gotoxy(xpos,++tempypos);
        tempheight = checkBoundary(3,tempypos,xpos,tempheight);
        printf("Search is done!");
    }
    else
    {
        gotoxy(xpos,++tempypos);
        tempheight = checkBoundary(2,tempypos,xpos,tempheight);
        printf("**Name is not found!!**\a");
    }

    // check if operation will be done again or do another operation
    int redo = again(tempypos, tempheight, console);
    if(redo == 1)
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
    draw_box(height,width,xpos-1,ypos-1);
    if(f == 404)f = count;

    tempheight = height;
    tempypos = ypos;
    gotoxy(xpos,tempypos);

    char *input;
    printf("First name: ");
    gotoxy(xpos,++tempypos);
    input = enterData(FIRST_NAME_SIZE, tempypos, console);
    strcpy(s[f].fname,input);
    free(input);

    gotoxy(xpos,++tempypos);
    printf("Last name: ");
    gotoxy(xpos,++tempypos);
    input = enterData(LAST_NAME_SIZE, tempypos, console);
    strcpy(s[f].lname,input);
    free(input);

    do
    {
        if(tempypos >= tempheight)
        {
            extend_box(3,width,xpos-1,tempypos);
            tempheight += 3;
        }
        gotoxy(xpos,++tempypos);
        printf("Date Of Birth(day month year):");
        gotoxy(xpos,++tempypos);
        printf(">");
        scanf("%d%d%d", &s[f].DOB.day, &s[f].DOB.month, &s[f].DOB.year);
    }
    while((s[f].DOB.day < 0 || s[f].DOB.day > 31)||(s[f].DOB.month > 12 || s[f].DOB.month < 0)||(s[f].DOB.year > 2020 || s[f].DOB.year < 1900));

    gotoxy(xpos,++tempypos);
    printf("Address: ");
    gotoxy(xpos,++tempypos);
    input = enterData(ADDRESS_SIZE, tempypos, console);
    strcpy(s[f].address,input);
    free(input);

    int z = 0;
    do
    {
        if(tempypos >= tempheight)
        {
            extend_box(3,width,xpos-1,tempypos);
            tempheight += 3;
        }
        gotoxy(xpos,++tempypos);
        printf("Phone number:\n>");
        scanf("%11s", s[f].phonum);

        int n = strlen(s[f].phonum);
        z = 0 ;
        for ( x =0 ; x < n; x++ )
        {
            if(!isdigit(s[f].phonum[x]))
            {
                z = 1;
                if(tempypos >= tempheight)
                {
                    extend_box(3,width,xpos-1,tempypos);
                    tempheight += 3;
                }
                gotoxy(xpos,++tempypos);
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
        if(tempypos >= tempheight)
        {
            extend_box(3,width,xpos-1,tempypos);
            tempheight += 3;
        }
        gotoxy(xpos,++tempypos);
        printf("E-mail : ");
        gotoxy(xpos,++tempypos);
        input = enterData(EMAIL_SIZE, tempypos, console);
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
            if(tempypos >= tempheight)
            {
                extend_box(3,width,xpos-1,tempypos);
                tempheight += 3;
            }
            gotoxy(xpos,++tempypos);
            printf("**Invalid email!!**\a");
        }
    }
    if(f == count)
    {
        count++;
        if(tempypos >= tempheight)
        {
            extend_box(3,width,xpos-1,tempypos);
            tempheight += 3;
        }
        gotoxy(xpos,++tempypos);
        printf("**Add is done successfully!**");

        // check if operation will be done again or do another operation
        int redo = again(tempypos, tempheight, console);
        if(redo == 1)
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
    draw_box(height,width,xpos-1,ypos-1);
    tempheight = height;
    tempypos = ypos;
    char *input;
    gotoxy(xpos,tempypos);
    ShowConsoleCursor(1, console);
    printf("Enter the last name: ");
    gotoxy(xpos,++tempypos);
    input = enterData(EMAIL_SIZE, tempypos, console);

    int check=0;
    int x = strlen(input);

    char srch[x];
    strcpy(srch,input);
    free(input);
    for(i = 0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            if(check == 0)
            {
                gotoxy(xpos,++tempypos);
                gotoxy(xpos,++tempypos);
                printf("Found contacts :-");
                gotoxy(xpos,++tempypos);
            }
            check++;
            if(i*9 > tempypos)
            {
                extend_box(9,width,xpos-1,tempypos);
                tempheight += 9;
            }
            gotoxy(xpos,++tempypos);
            printf("Contact number %d",i);
            gotoxy(xpos,++tempypos);
            printf(" First name: %s",s[i].fname);
            gotoxy(xpos,++tempypos);
            printf(" Last name: %s",s[i].lname);
            gotoxy(xpos,++tempypos);
            printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
            gotoxy(xpos,++tempypos);
            printf(" Address: %s",s[i].address);
            gotoxy(xpos,++tempypos);
            printf(" phone number: %s",s[i].phonum);
            gotoxy(xpos,++tempypos);
            printf(" E-mail: %s",s[i].email);
            gotoxy(xpos,++tempypos);
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
        tempypos += 3;
        draw_box(3,width,xpos-1,tempypos);
        while(z && !isdigit(n))
        {
            gotoxy(xpos,tempypos+2);
            ShowConsoleCursor(1, console);
            printf("Please a Enter valid Contact number: ");
            scanf("%d",&n);
            ShowConsoleCursor(0, console);
            for(i=0; i<check; i++)
            {
                if(n == y[i]) z = 0;
            }
            tempypos -= 2;
        }
        system("cls");
        Add(n, console);
        if(tempypos >= tempheight)
        {
            extend_box(3,width,xpos-1,tempypos);
            tempheight += 3;
        }
        gotoxy(xpos,++tempypos);
        printf("Modification complete!");
    }
    else
    {

        gotoxy(xpos,++tempypos);
        printf("**Name is not found!!**\a");
    }

    // check if operation will be done again or do another operation
    int redo = again(tempypos, tempheight, console);
    if(redo == 1)
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
    draw_box(height,width,xpos-1,ypos-1);
    ShowConsoleCursor(0, console);
    tempheight = height;
    tempypos = ypos;

    char entered_name[15];
    gotoxy(xpos,tempypos);
    printf("Enter the last name: ");
    ShowConsoleCursor(1, console);
    gotoxy(xpos,++tempypos);
    printf(">");
    scanf("%s",entered_name);
    ShowConsoleCursor(0, console);
    int check = 0;
    int x = strlen(entered_name);

    char srch[x];
    strcpy(srch,entered_name);

    for(i = 0; i < count; i++)
    {
        if( strcmp(srch, s[i].lname) == 0 )
        {
            if(check == 0)
            {
                gotoxy(xpos,++tempypos);
                printf("Found contacts :-");
            }
            check++;
            if(i*9 > tempypos)
            {
                extend_box(9,width,xpos-1,tempypos);
                tempheight += 10;
            }

            gotoxy(xpos,++tempypos);
            printf("No.%d",i);
            gotoxy(xpos,++tempypos);
            printf(" First name: %s",s[i].fname);
            gotoxy(xpos,++tempypos);
            printf(" Last name: %s",s[i].lname);
            gotoxy(xpos,++tempypos);
            printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
            gotoxy(xpos,++tempypos);
            printf(" Address: %s",s[i].address);
            gotoxy(xpos,++tempypos);
            printf(" phone number: %s",s[i].phonum);
            gotoxy(xpos,++tempypos);
            printf(" E-mail: %s",s[i].email);
            gotoxy(xpos,++tempypos);
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
        gotoxy(xpos,++tempypos);
        while(z)
        {
            gotoxy(xpos,tempypos);
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
                    --tempypos;
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
        if(tempypos+5 >= tempheight)
        {
            extend_box(5,width,xpos-1,tempypos);
            tempheight += 4;
        }
        gotoxy(xpos,++tempypos);
        printf("Delete complete!");
    }
    else
    {
        gotoxy(xpos,++tempypos);
        printf("Name is not found!!\a");
    }

    // check if operation will be done again or do another operation
    int redo = again(tempypos, tempheight, console);
    if(redo == 1)
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
    draw_box(height,width,xpos-1,ypos-1);
    tempheight = height;
    tempypos = ypos;
    Sort(xpos, tempypos, console);
    tempypos += 3;
    for(i=0; i < count; i++)
    {
        if((tempypos+8) >= tempheight)
        {
            extend_box(8,width,xpos-1,tempypos);
            tempheight += 8;
        }
        gotoxy(xpos,++tempypos);
        printf("No.%d",i);
        gotoxy(xpos,++tempypos);
        printf(" First Name: %s",s[i].fname);
        gotoxy(xpos,++tempypos);
        printf(" Last Name: %s",s[i].lname);
        gotoxy(xpos,++tempypos);
        printf(" Date of birth: %d-%d-%d", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
        gotoxy(xpos,++tempypos);
        printf(" Address: %s",s[i].address);
        gotoxy(xpos,++tempypos);
        printf(" Phone Nember: %s",s[i].phonum);
        gotoxy(xpos,++tempypos);
        printf(" E-mail: %s",s[i].email);
        gotoxy(xpos,++tempypos);
    }
    if((tempypos+6) >= tempheight)
    {
        extend_box(6,width,xpos-1,tempypos);
        tempheight += 6;
    }
    gotoxy(xpos,++tempypos);
    printf("Print is done!");
    gotoxy(xpos,++tempypos);

    // check if operation will be done again or do another operation
    int redo = again(tempypos, tempheight, console);
    if(redo == 1)
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
        i = 0;
        j = 0;
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
        draw_box(height/4,width,xpos-1,ypos-1);
        gotoxy((width/3),ypos+2);
        printf("program exit successfully!");
        tempypos = height/2 + 2;
        gotoxy(xpos,++tempypos);
        exit(0);
    }
    else
    {
        drawTitle("Exit");
        draw_box(height/2,width,xpos-1,ypos-1);
        ShowConsoleCursor(0, console);
        tempheight = height;
        tempypos = ypos;
        gotoxy(xpos,tempypos);
        printf("Any unsaved data will be lost!");
        gotoxy(xpos,++tempypos);
        printf("Are you sure you want to exit?y/n");


        char    again = ' ';
        do
        {
            if((tempypos+4) >= tempheight)
            {
                extend_box(4,width,xpos-1,tempypos);
                tempheight += 4;
            }
            gotoxy(xpos,++tempypos);
            ShowConsoleCursor(1, console);
            scanf("%s",&again);
            ShowConsoleCursor(0, console);

            if(again == 'y')
            {
                gotoxy(xpos,++tempypos);
                printf("program exit successfully!");
                tempypos = height/2 + 2;
                gotoxy(xpos,++tempypos);
                exit(0);
            }
            else if(again == 'n')
            {
                break;
            }
            else
            {
                if(tempypos >= tempheight)
                {
                    extend_box(3,width,xpos-1,tempypos);
                    tempheight += 3;
                }
                gotoxy(xpos,++tempypos);
                printf("Enter 'y' or 'n' only!\a");
                tempypos -= 2;
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
    draw_box(height/2,width,xpos-1,ypos-1);
    ShowConsoleCursor(0, console);
    tempheight = height/2;
    tempypos = ypos;
    char  filename[40];
    gotoxy(xpos,tempypos);
    printf("Name the file in which data will be saved: ");
    gotoxy(xpos,++tempypos);
    printf("e.g(filename.txt)");
    ShowConsoleCursor(1, console);
    gotoxy(xpos,++tempypos);
    printf("> ");
    scanf("%s",filename);
    FILE*f;
    f = fopen(filename,"w");
    for(i=0; i<count; i++)
    {
        fprintf(f,"%s,",s[i].fname);
        fprintf(f,"%s,",s[i].lname);
        fprintf(f,"%d-%d-%d,", s[i].DOB.day, s[i].DOB.month, s[i].DOB.year);
        fprintf(f,"%s,",s[i].address);
        fprintf(f,"%s,",s[i].phonum);
        fprintf(f,"%s\n",s[i].email);
    }
    gotoxy(xpos,++tempypos);
    printf("Your work has been saved!");
    fclose(f);
    char respond = ' ';
    do
    {
        gotoxy(xpos,++tempypos);
        printf("back to main menu?y/n");
        gotoxy(xpos,++tempypos);
        printf("> ");
        scanf("%s",&respond);
        if(respond == 'y' || respond == 'Y')mainMenu("File is loaded","Saved!");
        if(respond == 'n' || respond == 'N')
        {
            gotoxy(xpos,++tempypos);
            printf("program exit successfully!");
            tempypos = tempheight + 2;
            gotoxy(xpos,++tempypos);
            exit(0);
        }
        gotoxy(xpos,++tempypos);
        printf("Enter 'y' or 'n' only!\a");
        tempypos-=3;
    }
    while(respond != 'y' && respond != 'n' && respond != 'Y' && respond != 'N');
}

void consoleWindow()
{
    //position the console window
    //Getting the desktop hadle and rectangule
    HWND   hwndScreen = GetDesktopWindow();
    RECT   rectScreen;
    HWND ConsoleWindow = GetForegroundWindow();
    GetWindowRect(hwndScreen,&rectScreen);
    //Set windows size(see the width problem)
    SetWindowPos(ConsoleWindow,NULL,0,0,500,500, SWP_SHOWWINDOW);
    // Get the current width and height of the console
    RECT rConsole;
    GetWindowRect (ConsoleWindow,&rConsole);
    int Width = rConsole.right - rConsole.left;
    int Height = rConsole.bottom - rConsole.top;
    //caculate the window console to center of the screen
    int ConsolePosX = ((rectScreen.right-rectScreen.left)/2-Width/2 );
    int ConsolePosY = ((rectScreen.bottom-rectScreen.top)/2- Height/2 );
    SetWindowPos(ConsoleWindow,NULL,ConsolePosX,ConsolePosY, Width, Height, SWP_SHOWWINDOW || SWP_NOSIZE);

    //close the resize of console window
    hwndScreen = GetConsoleWindow();
    SetWindowLong(hwndScreen, GWL_STYLE, GetWindowLong(hwndScreen, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void ShowConsoleCursor(int showFlag, HANDLE console)
{

    CONSOLE_CURSOR_INFO cursorinfo;
    GetConsoleCursorInfo(console, &cursorinfo);
    cursorinfo.bVisible = showFlag;
    SetConsoleCursorInfo(console, &cursorinfo);
}

void Console_Font_Size(void)
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 9;
    cfi.dwFontSize.Y = 18;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

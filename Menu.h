#ifndef _MENUH_
#define _MENUH_

int menu(char* elements)//,int height, int width)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    ShowConsoleCursor(0, console); // hide the cursor
    int elements_counter = 0;
    int element_max_length = 0;

    char holder[strlen(elements)+1]; // for first token loop
    char holder2[strlen(elements)+1];  //for second token loop
    strcpy(holder,elements);
    strcpy(holder2,holder);

    char* token;

    //first token loop to count the words in the string
    token = strtok(holder, " ");
    element_max_length = strlen(token);
    while( token != NULL )
    {
        if(strlen(token) > element_max_length)
        {
            element_max_length = strlen(token);
        }
        elements_counter++;
        token = strtok(NULL, " ");
    }

    // array of string to hold the list elements
    char list_element [elements_counter][element_max_length + 1];

    //second token loop to assign the words in elements array
    int i = 0;
    token = strtok(holder2, " ");
    while( token != NULL )
    {
        strcpy(list_element[i],token);
        i++;
        token = strtok(NULL, " ");
    }
// -------------------- getting values of list elements is done --------------------

//printing the menu on the screen

    int pos = 0; // current position on the menu
    char clicked; // save the char that user click
    int tempheight = height - 3; // temp height to protect the original value of height

    do
    {
        SetConsoleTextAttribute(console,15);
        for(i = 0; i < elements_counter; i++)
        {
            gotoxy(width/2,tempheight/2 + (i+1));
            printf("[%d] %s",i+1,list_element[i]);
        }

        tempheight = height - 3;
        SetConsoleTextAttribute(console,240);//240
        gotoxy(width/2,tempheight/2 + (pos+1));
        printf("[%d] %s",pos+1,list_element[pos]);


        clicked = getch();
        if(clicked == 72)//if up arrow
        {
            pos--;
            if(pos == -1) pos = elements_counter-1;
        }
        if(clicked == 80)// if down arrow
        {
            pos++;
            if(pos == elements_counter)
                pos = 0;
        }
    }
    while(clicked != 13);
    SetConsoleTextAttribute(console,15);
    return pos;
}


#endif // MENU_H_INCLUDED

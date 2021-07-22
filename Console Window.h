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

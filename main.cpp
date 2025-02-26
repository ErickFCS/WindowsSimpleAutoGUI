#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#define maxCommandCant 100
#define quitKey '`'

using namespace std;
bool recording = false;

/*
mx,y (move)
c (click)
cu (clickUp)
cd (clickDown)
pText (print)
wText (write)
dTimeInMiliseconds (delay)
l (mark loop start)
L (Loop)
q (quit)
*/

void write(const std::string texto)
{
    for (char print : texto)
    {
        keybd_event((BYTE)VkKeyScan((char)print), 0, 0, 0);
        Sleep(50);
        keybd_event((BYTE)VkKeyScan((char)print), 0, KEYEVENTF_KEYUP, 0);
        Sleep(50);
    }
}

void click()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void clickDown()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void clickUp()
{
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

bool isPressed(char key)
{
    return GetAsyncKeyState((BYTE)VkKeyScan((char)key)) & 0x8000;
}

void move(int x, int y)
{
    SetCursorPos(x, y);
}

void enter()
{
    keybd_event(VK_RETURN, 0, 0, 0);
    Sleep(50);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
    Sleep(50);
}

void record()
{
    cout << "Recording..." << endl;
    ofstream f("record.txt");
    if (!f)
    {
        cout << "Couldn't open the f" << endl;
        return;
    }
    POINT mousePos;
    bool first = true;
    while (true)
    {
        if (GetAsyncKeyState(VkKeyScan('c')))
        {
            GetCursorPos(&mousePos);
            f << "m" << mousePos.x << "," << mousePos.y << std::endl;
            f << "c" << std::endl;
            cout << "m" << mousePos.x << "," << mousePos.y << std::endl;
            cout << "c" << std::endl;
        }
        else if (GetAsyncKeyState(VkKeyScan('m')))
        {
            GetCursorPos(&mousePos);
            f << "m" << mousePos.x << "," << mousePos.y << std::endl;
            cout << "m" << mousePos.x << "," << mousePos.y << std::endl;
        }
        else if (GetAsyncKeyState(VkKeyScan('w')))
        {
            break;
        }
        else if (GetAsyncKeyState(VkKeyScan('l')))
        {
            if (!first)
            {
                f << "L" << std::endl;
                cout << "L" << std::endl;
                break;
            }
            else
            {
                first = false;
                f << "l" << endl;
                cout << "l" << endl;
            }
        }
        else if (GetAsyncKeyState(VkKeyScan('q')))
        {
            break;
        }
        Sleep(100);
    }
    f.close();
}

char readCommandsFromFile(string *commands)
{
    ifstream f("record.txt");
    string line;
    unsigned char commandsCant = 0;
    if (!f)
        return -1;
    while (getline(f, line) && commandsCant < maxCommandCant)
    {
        if (commandsCant == maxCommandCant)
            break;
        commands[commandsCant++] = line;
    }
    f.close();
    return commandsCant == maxCommandCant ? -2 : commandsCant;
}

void play()
{
    cout << "Playing..." << endl;
    string *commands = new string[maxCommandCant];
    char commandsCant = readCommandsFromFile(commands);
    switch (commandsCant)
    {
    case -1:
        cout << "unable to read file" << endl;
        return;
    case -2:
        cout << "max command number exceed" << endl;
        return;
    default:
        break;
    }
    unsigned char currentCommandIndex = 0;
    unsigned char loopPos = 0;
    while (currentCommandIndex < commandsCant)
    {
        string currentCommand = commands[currentCommandIndex];
        if (currentCommand[0] == 'm')
            move(stoi(currentCommand.substr(1, currentCommand.find(","))), stoi(currentCommand.substr(currentCommand.find(",") + 1)));
        else if (currentCommand[0] == 'p')
            cout << currentCommand.substr(1) << endl;
        else if (currentCommand[0] == 'c')
        {
            if (currentCommand[1] == 'd')
                clickDown();
            else if (currentCommand[1] == 'u')
                clickUp();
            else
                click();
        }
        else if (currentCommand[0] == 'w')
            write(currentCommand.substr(1));
        else if (currentCommand[0] == 'd')
            Sleep(stoi(currentCommand.substr(1)));
        else if (currentCommand[0] == 'l')
            loopPos = currentCommandIndex;
        else if (currentCommand[0] == 'L')
            currentCommandIndex = loopPos;
        else if (currentCommand[0] == 'q')
            break;

        if (isPressed(quitKey))
            break;
        currentCommandIndex++;
    }
}

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        cout << "No argument" << endl;
        cout << "Try: " << endl;
        cout << "\t- record" << endl;
        cout << "\t- play" << endl;
        return 1;
    }
    if ((strcmp(argv[1], "play") && strcmp(argv[1], "record")))
    {
        cout << "Invalid argument" << endl;
        cout << "Try: " << endl;
        cout << "\t- record" << endl;
        cout << "\t- play" << endl;
        return 2;
    }
    recording = (!strcmp(argv[1], "record"));
    if (recording)
        record();
    else
        play();
    return 0;
}
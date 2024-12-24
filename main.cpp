#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;
bool recording = false;

/*
mx,y (move)
c (click)
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
        Sleep(100);
        keybd_event((BYTE)VkKeyScan((char)print), 0, KEYEVENTF_KEYUP, 0);
        Sleep(100);
    }
}

void click()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(100);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    Sleep(100);
}

void move(int x, int y)
{
    SetCursorPos(x, y);
}

void enter()
{
    keybd_event(VK_RETURN, 0, 0, 0);
    Sleep(100);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
    Sleep(100);
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

void play()
{
    cout << "Playing..." << endl;
    streampos loopPos;
    ifstream f("record.txt");
    if (!f)
    {
        cout << "Couldn't open the f" << endl;
        return;
    }
    string line;
    while (getline(f, line))
    {
        if (line[0] == 'm')
            move(stoi(line.substr(1, line.find(","))), stoi(line.substr(line.find(",") + 1)));
        else if (line[0] == 'c')
            click();
        else if (line[0] == 'w')
            write(line.substr(1));
        else if (line[0] == 'd')
            Sleep(stoi(line.substr(1)));
        else if (line[0] == 'l')
            loopPos = f.tellg();
        else if (line[0] == 'L')
            f.seekg(loopPos);
        else if (line[0] == 'q')
            break;
        Sleep(100);
    }
    f.close();
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
// otus.lessons.10.01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinUser.h>
#include <strsafe.h>
#include <string>
#include <iostream>
#include <map>
#include <sstream>

void printLastError(const std::string &strFunc)
{
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError();

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPSTR)&lpMsgBuf,
        0, NULL);

    std::cout << strFunc << "failed with error " << dw << ": " << (LPSTR)lpMsgBuf << std::endl;

    LocalFree(lpMsgBuf);
}

struct key2play {
    char c;
    char vk;
    bool ctrl;
    bool alt;
    bool shift;

    key2play() : c(0), vk(0), ctrl(false), alt(false), shift(false) {}
    key2play(char c_, bool ctrl_, bool alt_, bool shift_) : c(c_), vk((BYTE)VkKeyScanA(c_)), ctrl(ctrl_), alt(alt_), shift(shift_) {}
    key2play(const key2play &k2p) : c(k2p.c), vk(k2p.vk), ctrl(k2p.ctrl), alt(k2p.alt), shift(k2p.shift) {}
    key2play& operator=(const key2play &k2p) {
        c = k2p.c; 
        vk = k2p.vk;
        ctrl = k2p.ctrl; 
        alt = k2p.alt; 
        shift = k2p.shift;
        return *this;
    }
    void play(DWORD msec) {
        if (msec < 100)
            return;

        if (ctrl)
            keybd_event(VK_CONTROL, 0, 0, 0);
        if (alt)
            keybd_event(VK_MENU, 0, 0, 0);
        if (shift)
            keybd_event(VK_SHIFT, 0, 0, 0);
        keybd_event(vk, 0, 0, 0);

        Sleep(100);

        keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
        if (shift)
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        if (alt)
            keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
        if (ctrl)
            keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

        Sleep(msec - 100);
//        std::cout << "play: " << c << "(" << (uint32_t)vk << ")" << "; ctrl: " << ctrl << "; alt: " << alt << "; shift: " << shift << "; wait: " << msec << std::endl;
    }
};

std::map<std::string, key2play> get_layout();
size_t read_num(std::istream& in, size_t d_);

int main(int argv, char** argc)
{
    std::cout << "Hello" << std::endl;

    auto layout = get_layout();

    HWND hwnd = FindWindowA("FFXIVGAME", NULL);
    std::cout << "Found: " << hwnd << std::endl;
    BOOL bRes = SetForegroundWindow(hwnd);
    DWORD lastError = bRes ? 0 : GetLastError();
    std::cout << "move to top: " << bRes << std::endl;
    if (!bRes)
        printLastError("BringWindowToTop");
    if(0)
    {
        DWORD temp = 300;
        std::stringstream ss;
        // All Star (Smashmouth) - Intro
        ss << "F#-1, F# Bb-1 Bb-1 G#-1 F#-1 F#-1 B-1 Bb-1 Bb-1 G#-1 G#-1 F#-1 F#-1 C# Bb-1 Bb-1 G#-1 G#-1 F#-1 F#-1 Eb-1 C#-1";
        // Mortal combat
        if (0) {
            temp = 200;
            ss << "A-1 A-1 A-1 A-1 A-1 G-1 C-1 A-1 G-1 E-1 A-1 G-1 C-1 A-1 G-1 E-1 A-1 G-1 C-1 A-1 G-1 E-1 A-1 G-1 C-1 "
                << "A-1,,,,, A-1,,,,, A-1 "
                << "A-1 E C Bb C A-1 E C Bb C A-1 E C Bb C A-1 E C Bb C A-1 E C Bb C "
                << "A-1 A-1 A-1 "
                << std::endl;
        }
        // Fire Emblem Main Theme
        if (0) {
            ss << "D G B B E E F# G A G F# D G B D+1, C+1 B E "
                << "F# G A A D G B B E E F# G A G F# D G B D+1, C+1 B E "
                << "F# G A G G F# E C+1 A G F# D+1 B G G F E E+1 D+1 A B "
                << "E C+1 A G F# D+1 B G+1 G+1 F#+1 E+1 E+1 E+1 F#+1 G+1 A+1 G+1 F#+1 "
                << std::endl;
        }
        // Song of Storms (The Legend of Zelda: Ocarina of Time)
        if (0) {
            ss << "D F D+1 D F D+1 E+1 F+1 E+1 F+1 E+1 C+1 A A D F G A A D F G E "
                << "D F D+1 D F D+1 E+1 F+1 E+1 F+1 E+1 C+1 A A D F G A "
                << "A D F G A A D "
                << "D F D+1 D F D+1 E+1 F+1 E+1 F+1 E+1 C+1 A "
                << "A D D F G E D"
                << std::endl;
        }
        //    ss << "D F E G G F G A D A B G A G F G A D D D E E F F E C C D D";
        //    ss << "G-1 E E D C C B-1 A-1 B-1 C D G-1 G-1 A-1 B-1 C A-1 A-1 G-1 C E F E C E D E G A G F F E E D D E F E D C G C C D E F D G-1 E E D C C B-1 A-1 B-1 C D G-1 G-1 A-1 B-1 C A-1 A-1 G-1 C E F E C E D E G G A G F F E E D D E F E D D C C C D E D C F E D C B-1 C D E G F E D C# D E F E D E F E D C B-1 C E G F D G-1 C D E B-1 B-1 C B-1 G-1 A-1 F E E D D C A-1 B-1 C D C D E G E B-1 A-1 F E E D D E G F F E E D E F G E F E B D D C";
        //    ss << "F# G A D F# G# A C#+1 B C#+1 D+1 D+1 C#+1 B A A A G F# G G G F# F F# F# F# F D E F# F# G A D F# G# A C#+1 B C#+1 D+1 D+1 C#+1 B A A A G F# G G G F# F F# F# F# E D E D";
        //    ss << "C+1, C+1, C+1, C+1, G#, Bb, C+1, Bb, C+1";
        //    ss << "C D E C D E E E F G E C A C D E C D E E E F G E B C+1 C+1 D+1 E+1 C+1 D+1 E+1 E+1 E+1 F+1 G+1 E+1 C+1 A+1 F+1 G+1 F+1 D+1 E+1 F+1 G+1 F+1 D+1 E+1 F+1 G+1 F+1 D+1 E+1 F+1 G+1 F+1 D+1 E+1 F+1 G+1 F+1 D+1 E+1 F+1 G+1 F+1 D+1 E+1";
        //    ss << "D-1 A-1 G-1 F-1 E-1 F-1 G-1 G-1 F-1 E-1 D-1 D-1 A-1 G-1 F-1 E-1 F-1 G-1 C G-1 Bb-1 A-1 D-1 A-1 G-1 F-1 E-1 F-1 G-1 G-1 F-1 E-1 D-1 D-1 E-1 F-1 E-1 D-1 C-1 C-1 D-1 E-1 F-1 C-1 E-1 D-1 D-1 F-1 A-1 D-1 F-1 A-1 D+1 A+1 A+1 G+1 F+1 F+1 E+1 F+1 G+1 G+1 F+1 E+1 D+1 D+1 A+1 A+1 G+1 F+1 F+1 E+1 F+1 G+1 G+1 C+2 G+1 Bb+1 A+1 D+1 A+1 A+1 G+1 F+1 F+1 E+1 F+1 G+1 G+1 F+1 E+1 D+1 D+1 E+1 F+1 F+1 E+1 D+1 C+1 C+1 D+1 E+1 F+1 C+1 E+1 D+1 F+1 E+1 F+1 A+1 G+1 F+1 G+1 Bb+1 A+1 F+1 E+1 F+1 A+1 G+1 C+2 G+1 Bb+1 A+1 A+1 C+2 G+1 Bb+1 F+1 A+1 G+1 E+1 F+1 A+1 G+1 E+1 C+1 D+1 D+1 D+1 D+1 C+1 C+1 Bb Bb A A D+1 D+1 C+1 C+1 Bb Bb A A D+1 D+1 B C+1 C+1 A Bb Bb G A A F D+1 D+1 B C+1 C+1 A Bb Bb G AA F D+1 F+1 D+1 C+1 E+1 C+1 Bb D+1 Bb A C+1 A D+1 F+1 D+1 C+1 E+1 C+1 Bb D+1 Bb AA A-1 A-1 D E A A-1 A-1 D E A D A A G F E F G G F E D D A A G F E F G G C+1 G Bb A D A A G F E F G G F E D D E F F E D C D E F C E D G D E F F E D C E D G D E F F G F G C+1 A G D E F F E D C E D D E F F E D C D E F C E D D E F G F D C D E F G G A Bb C+1 D+1 D+1 D+1 B C+1 C+1 A Bb Bb G A A F D+1 D+1 B C+1 C+1 A Bb Bb G A A F D+1 D+1 B C+1 C+1 A Bb Bb D D Bb D+1 D+1 B C+1 C+1 A Bb Bb A D D D A A G F F E F G G F E D D A A G F F E F G G C+1 G Bb A D A A G F F E F G G F E D D E F E D C C D E F C E D D E F E D C C D E F C E D";
        //    ss << "A C+1 D+1 A C+1 D+1 D+1 E+1 F+1 D+1 A G A C+1 D+1 A C+1 D+1 D+1 E+1 F+1 D+1 G+1 A+1 A C+1 D+1 A C+1 D+1 D+1 E+1 F+1 D+1 A G E+1 F+1 E+1 C+1 D+1 E+1 F+1 E+1 C+1 D+1 E+1 F+1 E+1 C+1 D+1";
        //    ss << "F Bb Bb C+1 C+1 D+1, Bb D+1 D+1, D+1 Eb+1 D+1 C+1" << std::endl;
        //    ss << "F G G A A Bb C+1 D+1 F D+1 Eb+1 C+1 Bb Bb" << std::endl;
        //    ss << "Bb G+1 G+1 Eb+1 G+1 F+1 Bb, Bb G+1 G+1 G+1 Eb+1 G+1 F+1" << std::endl;
        //    ss << "F G G A A Bb C+1 D+1 Bb D+1 Eb+1 C+1 Bb Bb" << std::endl;

        std::string note;
        while (!ss.eof()) {
            ss >> note;

            DWORD msec = temp;
            while (note.length() > 0 && note[note.length() - 1] == ',') {
                msec += temp;
                note = note.substr(0, note.length() - 1);
            }

            std::cout << "note: " << note << "; msec: " << msec << std::endl;
            auto f = layout.find(note);
            if (f != layout.end())
                f->second.play(msec);
            else
                std::cout << "\tUNKNOWN" << std::endl;
        }

        Sleep(1000);
    }

    if(1)
    {
        std::stringstream ss;
        if (1) {
            ss << "r1.l8r.<a>>ee<e>e<ad>ff<d>f<a<g>>gg<c->g<gc>e<ge>e<g<a>>ee<e>e<ad>ff<d"
                << ">f<ac->ee<e>ddn45ee<e>e<a<a>>ee<e>e<ad>ff<d>f<a<g>>gg<c->g<gc>e<ge>e<g"
                << "<a>>ee<e>e<ad>ff<d>f<ac->ee<e>ddn45ee<e>e<a<a>>ee<e>e<ad>ff<d>f<a<g>>g"
                << "g<c->g<gc>e<ge>e<g<a>>ee<e>e<ad>ff<d>f<ac->ee<e>ddn45ee<e>e<a<a>>ee<e>"
                << "e<ad>ff<d>f<a<g>>gg<c->g<gc>e<ge>e<g<a>>ee<e>e<ad>ff<d>f<ac->ee<e>ddn4"
                << "5ee<e>e<a<a>>ee<e>e<ad>ff<d>f<a<g>>gg<c->g<gc>e<ge>e<g<a>>ee<e>e<ad>ff"
                << "<d>f<ac->ee<e>ddn45ee<e>e<a";
        }
        if (0) {
            ss << "r1r1r4d2f4.d4d8g4d4c4d2a4.d4d8a+4a4f4d4a4>d4<d8c4c8<a4>e4d1&d1&d4d2f4."
                << "d4d8g4d4c4d2a4.d4d8a+4a4f4d4a4>d4<d8c4c8<a4>e4d1&d1&d4d2f4.d4d8g4d4c4d2a4."
                << "d4d8a+4a4f4d4a4>d4<d8c4c8<a4>e4d1&d1&d4d2f4.d4d8g4d4c4d2a4.d4d8a+4a4f4d4a4>"
                << "d4<d8c4c8<a4>e4<d4>d1&d1<d2>d4.<c4>c4.<a4>c4<d2>d2&d8<g8a4>c4d4<<a+2>a+4.c4>"
                << "c4.c4<d4>d1&d8c8<a4g4f4d2>d4.<c4>c4.<a4>c4<d2>d2&d8<g8a4>c4d4<<a+2>a+4.c4>c4."
                << "c4<d4>d1&d1<d2>d4.>d4d8g4d4c4d2a4.d4d8a+4a4f4d4a4>d4<d8c4c8<a4>e4d2";
        }
        if (1) {
            ss << "r1r1rd2f.dd8gdcd2a.dd8a+afda>d<d8cc8<a>ed1&d1&dd2f.dd8gdcd2a.dd8a+afda" << std::endl;
            ss << ">d<d8cc8<a>ed1&d1&dd2f.dd8gdcd2a.dd8a+afda>d<d8cc8<a>ed1&d1&dd2f.dd8gd" << std::endl;
            ss << "cd2a.dd8a+afda>d<d8cc8<a>el1d&d&d&d&d&d&d&d&d&d&d&d&d&d&d&d&d&dl4&dd2f" << std::endl;
            ss << ".dd8gdcd2a.dd8a+afda>d<d8cc8<a>ed2" << std::endl;
        }

        key2play brk('s', false, false, false);
        int t = 80 * 4;
        int o = 0;
        int l = 1;
        while (!ss.eof()) {
            int c = tolower(ss.get());
//            std::cout << "c: '" << (char)c << "'" << std::endl;
            if (c == '<')
                --o;
            else if (c == '>')
                ++o;
            else if (c == '.')
                brk.play(t);
            else if (c == 'r' || c == 'p')
                Sleep(t / read_num(ss, 1));
            else if (c == '&') {
                c = ss.get();
                Sleep(t / read_num(ss, 1));
            }
            else if (c == '0')
                o = read_num(ss, 0);
            else if (c == 't')
                t = read_num(ss, 120) * 4;
            else if (c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'a' || c == 'b') {
                bool sharp = false;
                if (ss.peek() == '+') {
                    ss.get();
                    sharp = true;
                }
                size_t l = read_num(ss, 1);
                std::string note("");
                if (sharp) {
                    char add;
                    if (c == 'c' || c == 'f' || c == 'g') {
                        add = '#';
                    }
                    else 
                    {
                        add = 'b';
                        if (c == 'd')
                            c = 'e';
                        else if (c == 'a')
                            c = 'b';
                    }
                    note += (char)toupper(c);
                    note += add;
                }
                else
                    note += (char)toupper(c);
                if (o == -1)
                    note += "-1";
                else if (o == 0)
                    ;
                else if (o == +1)
                    note += "+1";
                else
                    std::cout << "n/a octave " << o << std::endl;
                std::cout << "note: " << note << "; length: " << l << std::endl;
                auto f = layout.find(note);
                if (f != layout.end())
                    f->second.play(t / l);
                else
                    std::cout << "\tUNKNOWN" << std::endl;
            }
        }
        Sleep(1000);
    }

    return 0;
}

size_t read_num(std::istream& in, size_t d_)
{
    size_t d = 0;
    bool has_digits = false;
    while (!in.eof() && isdigit(in.peek())) {
        d = d * 10 + (in.get() - '0');
        has_digits = true;
    }
    return has_digits ? d : d_;
}

std::map<std::string, key2play> get_layout() {
    std::map<std::string, key2play> layout;

    layout["C-1"] = key2play('1', false, false, false);
    layout["C"] = key2play('1', true, false, false);
    layout["C+1"] = key2play('1', false, true, false);

    layout["C#-1"] = key2play('2', false, false, false);
    layout["C#"] = key2play('2', true, false, false);
    layout["C#+1"] = key2play('2', false, true, false);

    layout["D-1"] = key2play('3', false, false, false);
    layout["D"] = key2play('3', true, false, false);
    layout["D+1"] = key2play('3', false, true, false);

    layout["Eb-1"] = key2play('4', false, false, false);
    layout["Eb"] = key2play('4', true, false, false);
    layout["Eb+1"] = key2play('4', false, true, false);

    layout["E-1"] = key2play('5', false, false, false);
    layout["E"] = key2play('5', true, false, false);
    layout["E+1"] = key2play('5', false, true, false);

    layout["F-1"] = key2play('6', false, false, false);
    layout["F"] = key2play('6', true, false, false);
    layout["F+1"] = key2play('6', false, true, false);

    layout["F#-1"] = key2play('7', false, false, false);
    layout["F#"] = key2play('7', true, false, false);
    layout["F#+1"] = key2play('7', false, true, false);

    layout["G-1"] = key2play('8', false, false, false);
    layout["G"] = key2play('8', true, false, false);
    layout["G+1"] = key2play('8', false, true, false);

    layout["G#-1"] = key2play('9', false, false, false);
    layout["G#"] = key2play('9', true, false, false);
    layout["G#+1"] = key2play('9', false, true, false);

    layout["A-1"] = key2play('0', false, false, false);
    layout["A"] = key2play('0', true, false, false);
    layout["A+1"] = key2play('0', false, true, false);

    layout["Bb-1"] = key2play('-', false, false, false);
    layout["Bb"] = key2play('-', true, false, false);
    layout["Bb+1"] = key2play('-', false, true, false);

    layout["B-1"] = key2play('=', false, false, false);
    layout["B"] = key2play('=', true, false, false);
    layout["B+1"] = key2play('=', false, true, false);

    return layout;
}
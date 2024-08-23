#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <string>
#include <random>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "stb_image.h"

GLuint LoadTexture(const char* filename);
std::string getRandomString(unsigned int lenght);
std::vector<std::string> split(const std::string& line, char delimiter);
std::string getLineFromImVec4(ImVec4 vec);

struct date {
    int day, month, year;
    int dayOfWeek;

    std::string getMonthText();
    void setDate(std::string s);
    void setDate(int d, int m, int y);
    void setToday();
    std::string getDate();
    void operator++(int);
    bool operator==(date &other);
    bool operator!=(date &other);
};

struct Rect {
    int x, y, w, h;
};

struct calendarDay {
    date date;
    Rect rect;
};

struct month {
    std::vector<calendarDay> days;
};

struct Event {
    std::string text;
    ImVec4 color;
};

struct eventDay {
    std::vector<Event> events;
    date day;

    void addEvent(std::string s);
    std::string getCSVLine();
};

struct Note {
    std::string name;
    std::string randomName;
    std::string password;
};

struct GUI {

    int windowWidth;
    int windowHeight;

    std::vector<month> months;
    unsigned int actualMonth;

    date today;

    void previousMonth();
    void nextMonth();

    bool coordsInsideRect(int x, int y, Rect rect);
    double mouseX;
    double mouseY;
    void mouseClick();
    bool clicked;
    calendarDay* clickedDay;

    int optionsWindowX;
    int optionsWindowY;

    bool addEventWindow;
    std::string addEventText;
    ImVec4 color;

    void addEvent();
    std::vector<eventDay> events;
    int getEventDayPos(date d, std::vector<eventDay> events);
    int pos;

    bool getRect(date d, Rect &rect);
    void drawEvents();

    void resize(int width, int height);

    GLuint squareTexture;

    std::vector<Note> notes;
    unsigned int selected;

    std::string addFileName;
    std::string text;

    bool addMode;
    bool deleteMode;    bool sureToDelete; int eventToDelete;
    bool openMode;      bool openNote;

    bool kernelModified; bool saveChanges;

    void loadKernel();
    void saveKernel();

    void init();
    void draw();
};
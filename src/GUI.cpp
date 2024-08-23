#define STB_IMAGE_IMPLEMENTATION
#include "GUI.h"

GLuint LoadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr)
    {
        std::cerr << "Failed to load texture";
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}

std::string date::getMonthText() {
    if(month == 1) {
        return "January";
    }
    if(month == 2) {
        return "February";
    }
    if(month == 3) {
        return "March";
    }
    if(month == 4) {
        return "April";
    }
    if(month == 5) {
        return "May";
    }
    if(month == 6) {
        return "June";
    }
    if(month == 7) {
        return "July";
    }
    if(month == 8) {
        return "August";
    }
    if(month == 9) {
        return "September";
    }
    if(month == 10) {
        return "October";
    }
    if(month == 11) {
        return "November";
    }
    if(month == 12) {
        return "December";
    }
}

void date::setDate(int d, int m, int y) {
    day = d;
    month = m;
    year = y;
}

void date::setDate(std::string s) {
    std::vector<std::string> tokens = split(s, '/');
    day = std::stoi(tokens[0]);
    month = std::stoi(tokens[1]);
    year = std::stoi(tokens[2]);
}

void date::setToday() {
    // Structure to hold date and time
    SYSTEMTIME st;
    // Get the current local time
    GetLocalTime(&st);

    day = st.wDay;
    month = st.wMonth;
    year = st.wYear;
}

std::string date::getDate() {
    return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
}

void date::operator++(int) {
    day++;
    dayOfWeek++;
    if(dayOfWeek == 7) {
        dayOfWeek = 0;
    }

    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        if(day == 32) {
            day = 1;
            month++;
        }
    }
    if(month == 4 || month == 6 || month == 9 || month == 11) {
        if(day == 31) {
            day = 1;
            month++;
        }
    }
    if(month == 2) {
        if(year % 4 == 0) {
            if(day == 30) {
                day = 1;
                month ++;
            }
        } else {
            if(day == 29) {
                day = 1;
                month ++;
            }
        }
    }

    if(month == 13) {
        month = 1;
        year++;
    }

}

bool date::operator==(date &other) {
    return day == other.day && month == other.month && year == other.year;
}

bool date::operator!=(date &other) {
    return !(*this == other);
}

std::string getLineFromImVec4(ImVec4 vec) {
    return std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + "," + std::to_string(vec.w);
}

void eventDay::addEvent(std::string s) {
    std::vector<std::string> tokens;
    tokens = split(s, '|');
    std::vector<std::string> colorTokens;
    colorTokens = split(tokens[0], ',');
    ImVec4 vec;
    vec.x = std::stof(colorTokens[0]);
    vec.y = std::stof(colorTokens[1]);
    vec.z = std::stof(colorTokens[2]);
    vec.w = std::stof(colorTokens[3]);
    Event e;
    e.color = vec;
    e.text = tokens[1];
    events.push_back(e);
}

std::string eventDay::getCSVLine() {
    std::string line;
    line = day.getDate() + ";";
    for(unsigned int i = 0; i < events.size(); i++) {
        line += getLineFromImVec4(events[i].color) + "|" + events[i].text + ";";
    }
    return line;
}

std::string getRandomString(unsigned int lenght) {
    std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    // Define the distribution for integers
    std::uniform_int_distribution<> dis_int(0, chars.size() - 1); // Range
    std::string rs = "";
    for(unsigned int i = 0; i < lenght; i++) {
        rs += chars[dis_int(gen)];
    }
    return rs;
}

std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::string token;
    
    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

void GUI::loadKernel() {
    std::filesystem::path filePath = "events.txt";
    if (std::filesystem::exists(filePath)) {

        std::ifstream file("events.txt");
        if (!file.is_open()) {
            std::cerr << "Failed to open events file" << std::endl;
            return;
        }

        std::string line;
        while (getline(file, line)) {
            std::vector<std::string> tokens = split(line, ';');
            eventDay ed;
            ed.day.setDate(tokens[0]);
            for(unsigned int i = 1; i < tokens.size(); i++) {
                ed.addEvent(tokens[i]);
            }
            events.push_back(ed);
         }

        file.close();
    }
}

void GUI::saveKernel() {
    std::ofstream file;
    file.open("events.txt", std::ios::out | std::ios::trunc);

    // Check if the file was successfully opened
    if (!file) {
        std::cerr << "Failed to create the events file." << std::endl;
        return;
    }

    // Write data to the file
    for(unsigned int i = 0; i < events.size(); i++) {
        file << events[i].getCSVLine() + "\n";
    }

    // Close the file
    file.close();
}

void GUI::init() {

    windowWidth = 640;
    windowHeight = 640;

    addEventText = "New event";

    loadKernel();

    color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

    squareTexture = LoadTexture("images/square.png");

    date d;
    d.setDate(1, 8, 2024);
    d.dayOfWeek = 3;
    int weekNumber = 0;
    month month;
    while(d.month == 8) {
        calendarDay cd;
        cd.date = d;
        cd.rect.x = (windowWidth / 7) * cd.date.dayOfWeek;
        cd.rect.y = (windowWidth / 7) * weekNumber + 100;
        cd.rect.w = windowWidth / 7;
        cd.rect.h = windowWidth / 7;
        month.days.push_back(cd);
        d++;
        if(d.dayOfWeek == 0) {
            weekNumber++;
        }
    }
    months.push_back(month);
    actualMonth = 0;

    today.setToday();
    while(  months[actualMonth].days.front().date.month != today.month ||
            months[actualMonth].days.front().date.year != today.year) {

        nextMonth();
    }

    kernelModified = false;
    sureToDelete = false;
    saveChanges = false;
}

bool GUI::coordsInsideRect(int x, int y, Rect rect) {
    return x > rect.x && x < rect.x + rect.w &&
           y > rect.y && y < rect.y + rect.h;
}

void GUI::mouseClick() {
    if(!clicked) {
        for(unsigned int i = 0; i < months[actualMonth].days.size(); i++) {
            if(coordsInsideRect(mouseX, mouseY, months[actualMonth].days[i].rect)) {
                clickedDay = &months[actualMonth].days[i];
                clicked = true;
                optionsWindowX = mouseX;
                optionsWindowY = mouseY;
            }
        }
    }
}

void GUI::resize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    for(unsigned int i = 0; i < months.size(); i++) {
        int weekNumber = 0;
        date d = months[i].days.front().date;
        for(unsigned int j = 0; j < months[i].days.size(); j++) {
            months[i].days[j].rect.x = (windowWidth / 7) * months[i].days[j].date.dayOfWeek;
            months[i].days[j].rect.y = (windowWidth / 7) * weekNumber + 100;
            months[i].days[j].rect.w = windowWidth / 7;
            months[i].days[j].rect.h = windowWidth / 7;
            if(months[i].days[j].date.dayOfWeek == 6) {
                weekNumber++;
            }
        }
    }
}

void GUI::previousMonth() {
    if(actualMonth != 0) {
        actualMonth--;
    }
}

void GUI::nextMonth() {
    if(actualMonth == months.size() - 1){
        date d = months[actualMonth].days.back().date;
        int weekNumber = 0;
        d++;
        int monthNumber = d.month;
        month month;
        while(d.month == monthNumber) {
            calendarDay cd;
            cd.date = d;
            cd.rect.x = (windowWidth / 7) * cd.date.dayOfWeek;
            cd.rect.y = (windowWidth / 7) * weekNumber + 100;
            cd.rect.w = windowWidth / 7;
            cd.rect.h = windowWidth / 7;
            month.days.push_back(cd);
            d++;
            if(d.dayOfWeek == 0) {
                weekNumber++;
            }
        }
        months.push_back(month);
        actualMonth++;
    } else {
        actualMonth++;
    }
}

int GUI::getEventDayPos(date d, std::vector<eventDay> events) {
    for(unsigned int i = 0; i < events.size(); i++) {
        if(d == events[i].day)
            return i;
    }
    return -1;
}

void GUI::addEvent() {
    int pos = getEventDayPos(clickedDay->date, events);
    if(pos == -1) {
        eventDay ed;
        ed.day = clickedDay->date;
        events.push_back(ed);
        pos = events.size() - 1;
    }
    Event e;
    e.text = addEventText;
    e.color = color;
    if(addEventText != "") {
        events[pos].events.push_back(e);
        kernelModified = true;
    }
}

bool GUI::getRect(date d, Rect &rect) {
    for(unsigned int i = 0; i < months[actualMonth].days.size(); i++) {
        if(months[actualMonth].days[i].date == d) {
            rect = months[actualMonth].days[i].rect;
            return true;
        }
    }
    return false;
}

void GUI::drawEvents() {
    ImDrawList* list = ImGui::GetBackgroundDrawList();
    Rect rect;
    //list->AddText(ImVec2(months[actualMonth].days[i].rect.x + 5, months[actualMonth].days[i].rect.y + 5), IM_COL32(0, 0, 0, 255), std::to_string(months[actualMonth].days[i].date.day).c_str());
    for(unsigned int i = 0; i < events.size(); i++) {
        for(unsigned int j = 0; j < events[i].events.size(); j++) {
            if(getRect(events[i].day, rect)) {
                list->AddText(ImVec2(rect.x + 5, rect.y + 20 + 15 * j), IM_COL32(events[i].events[j].color.x * 255, events[i].events[j].color.y * 255, events[i].events[j].color.z * 255, events[i].events[j].color.w * 255), events[i].events[j].text.c_str());
            }
        }
    }
}

void GUI::draw() {

    if(clicked && !saveChanges) {
        ImGui::SetNextWindowPos(ImVec2(optionsWindowX, optionsWindowY));
        ImGui::Begin("Options");
        if(ImGui::Button("Add event")) {
            addEventWindow = true;
        }
        if(ImGui::Button("Delete event")) {
            deleteMode = true;
            pos = getEventDayPos(clickedDay->date, events);
        }
        if(ImGui::Button("Close")) {
            clicked = false;
        }
        ImGui::End();
    }

    if(addEventWindow) {
        ImGui::Begin("Add event");
        ImGui::InputText("Name", &addEventText);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::ColorEdit3("Color Picker", (float*)&color);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        if(ImGui::Button("Add")) {
            addEvent();
            addEventWindow = false;
            clicked = false;
            addEventText = "New event";
            color.x = 1.0f;
            color.y = 0.0f;
            color.z = 0.0f;
            color.w = 1.0f;
        }
        ImGui::SameLine();
        if(ImGui::Button("Close")) {
            addEventWindow = false;
            clicked = false;
        }

        ImGui::End();
    }
    
    if(deleteMode) {
        if(pos != -1) {
            ImGui::Begin("Delete event");
            for(unsigned int i = 0; i < events[pos].events.size(); i++) {
                if(ImGui::Button(std::to_string(i).c_str())) {
                    sureToDelete = true;
                    eventToDelete = i;
                }
                ImGui::SameLine();
                ImGui::Text(events[pos].events[i].text.c_str());
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            if(ImGui::Button("Close")) {
                deleteMode = false;
                clicked = false;
            }
            ImGui::End();
        }
    }

    if(sureToDelete) {
        ImGui::Begin("Sure to delete?");
        if(ImGui::Button("Yes")) {
            events[pos].events.erase(events[pos].events.begin() + eventToDelete);
            if(events[pos].events.size() == 0) {
                events.erase(events.begin() + pos);
            }
            sureToDelete = false;
            deleteMode = false;
            clicked = false;
            kernelModified = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("No")) {
            sureToDelete = false;
            deleteMode = false;
            clicked = false;
        }
        ImGui::End();
    }

    if(kernelModified) {
        ImGui::Begin("Unsaved changes");
        if(ImGui::Button("Save")) {
            saveKernel();
            kernelModified = false;
        }
        ImGui::End();
    }

    if(saveChanges) {
        ImGui::Begin("Save changes");
        ImGui::Text("Exit saving changes?");
        if(ImGui::Button("Yes")) {
            saveKernel();
            kernelModified = false;
        }
        ImGui::SameLine();
        if(ImGui::Button("Discard")) {
            kernelModified = false;
        }
        ImGui::End();
    }

    ImDrawList* list = ImGui::GetBackgroundDrawList();
    for(unsigned int i = 0; i < months[actualMonth].days.size(); i++) {
        list->AddImage((void*)(intptr_t)squareTexture, ImVec2(months[actualMonth].days[i].rect.x, months[actualMonth].days[i].rect.y), ImVec2(months[actualMonth].days[i].rect.x + months[actualMonth].days[i].rect.w, months[actualMonth].days[i].rect.y + months[actualMonth].days[i].rect.h));
        list->AddText(ImVec2(months[actualMonth].days[i].rect.x + 5, months[actualMonth].days[i].rect.y + 5), IM_COL32(0, 0, 0, 255), std::to_string(months[actualMonth].days[i].date.day).c_str());
    }

    list->AddText(ImVec2(windowWidth / 7 * 0 + windowWidth / 14 - ImGui::CalcTextSize("Monday").x / 2, 75), IM_COL32(255, 255, 255, 255), "Monday");
    list->AddText(ImVec2(windowWidth / 7 * 1 + windowWidth / 14 - ImGui::CalcTextSize("Tuesday").x / 2, 75), IM_COL32(255, 255, 255, 255), "Tuesday");
    list->AddText(ImVec2(windowWidth / 7 * 2 + windowWidth / 14 - ImGui::CalcTextSize("Wednesday").x / 2, 75), IM_COL32(255, 255, 255, 255), "Wednesday");
    list->AddText(ImVec2(windowWidth / 7 * 3 + windowWidth / 14 - ImGui::CalcTextSize("Thursday").x / 2, 75), IM_COL32(255, 255, 255, 255), "Thursday");
    list->AddText(ImVec2(windowWidth / 7 * 4 + windowWidth / 14 - ImGui::CalcTextSize("Friday").x / 2, 75), IM_COL32(255, 255, 255, 255), "Friday");
    list->AddText(ImVec2(windowWidth / 7 * 5 + windowWidth / 14 - ImGui::CalcTextSize("Saturday").x / 2, 75), IM_COL32(255, 255, 255, 255), "Saturday");
    list->AddText(ImVec2(windowWidth / 7 * 6 + windowWidth / 14 - ImGui::CalcTextSize("Sunday").x / 2, 75), IM_COL32(255, 255, 255, 255), "Sunday");

    drawEvents();

    ImGui::Begin("Calendar");
    if(ImGui::Button("<-")) {
        previousMonth();
    }
    ImGui::SameLine();
    if(ImGui::Button("->")) {
        nextMonth();
    }
    ImGui::SameLine();
    ImGui::Text((months[actualMonth].days.back().date.getMonthText() + " - " + std::to_string(months[actualMonth].days.back().date.year)).c_str());

    ImGui::End();

}
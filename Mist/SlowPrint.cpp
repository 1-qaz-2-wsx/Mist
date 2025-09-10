#include "slowPrint.h"

void slowPrint(const std::string& text, unsigned int delayMillis) {
    for (char c : text) {
        std::cout << c;
        if (c != '\n') {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis));
        }
        if (_kbhit()) {
            while (_kbhit()) {
                _getch();
            }
            std::cout << text.substr(text.find(c) + 1) << std::endl;
            return;
        }
    }
    std::cout.flush();
}
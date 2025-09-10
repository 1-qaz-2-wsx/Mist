#pragma once
#ifndef SLOWPRINT_H
#define SLOWPRINT_H

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdio>
#include <conio.h>

void slowPrint(const std::string& text, unsigned int delayMillis);

#endif

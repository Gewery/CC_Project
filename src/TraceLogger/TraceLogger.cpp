#include <stdio.h>
#include <ostream>
#include <iostream>

#include "TraceLogger.h"

std::string TraceLogger::Indent;

TraceLogger::TraceLogger(const char* fileName, const char* funcName, int lineNumber) {
    _fileName = fileName;
    _funcName = funcName;
    std::cout << Indent << " Entering " << _funcName << "() - (" << _fileName << ":" << lineNumber << ")" << std::endl;
    Indent.append("..");
}

TraceLogger::~TraceLogger() {
    Indent.resize(Indent.length() - 2);
    std::cout << Indent << " Leaving  " << _funcName << "() - (" << _fileName << ")" << std::endl;
}
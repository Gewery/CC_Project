class TraceLogger {
public:
    TraceLogger(const char* fileName, const char* funcName, int lineNumber);
    ~TraceLogger();

private:
    const char* _fileName;
    const char* _funcName;
    static std::string Indent;
};

#define LOG_TRACE TraceLogger traceLogger(__FILE__, __FUNCTION__, __LINE__);
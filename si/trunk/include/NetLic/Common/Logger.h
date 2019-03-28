#pragma once

class LoggerWriter{
public:
	virtual void print(const char* log) = 0;
};

class StdConsoleLogger : public LoggerWriter{
public:
	virtual void print(const char* log){
		fprintf(stdout,log);
	}
};

class FileLoggerWriter : public LoggerWriter{
public:
    FileLoggerWriter(char* logFileName);
	virtual void print(const char* log);	

	~FileLoggerWriter(){
		if(fp){
			fclose(fp);
			fp = NULL;
		}
	}
private:
    std::string _logPath;
	FILE* fp;
};

class Logger
{
public:
	
	enum LogLevel{
		LEVEL_DEBUG = 1,
		LEVEL_INFO,
		LEVEL_WARN,
		LEVEL_ERROR,
		LEVEL_FATAL
	};
	Logger();

	static Logger& Get();

    void Debug(const char* format, ...);
    void Info(const char* format, ...);
    void Warning(const char* format, ...);
    void Fatal(const char* format, ...);

    void SetLevel(int level) {m_level = level;}
	int  GetLevel() const {return m_level;}

	void SetLogWriter(LoggerWriter* pLoggerWriter){m_pLoggerWriter = pLoggerWriter;}


private:
	void DoPrintLine(const char* line);
	void DoPrint(LogLevel level,const char* format, va_list ap,bool newLine = true);
	std::string GetModeDescription(LogLevel level);
	int		m_level;
	LoggerWriter* m_pLoggerWriter;
};


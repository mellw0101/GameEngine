#pragma once

#include "Global.hpp"

#include <cstdint>
#include <string>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <X11/X.h>
#include <vector>
#include <xcb/xproto.h>
#include <mutex>
#include <queue>
#include <cstring> // For strerror
#include <cerrno>  // For errno

using namespace std;

// ANSI escape codes for colors
#define log_RED 		"\033[1;31m"
#define log_GREEN 		"\033[1;32m"
#define log_YELLOW 		"\033[1;33m"
#define log_BLUE 		"\033[1;34m"
#define log_MEGENTA 	"\033[1;35m"
#define log_CYAN 		"\033[1;36m"
#define log_WHITE 		"\033[1;37m"
#define log_BOLD 		"\033[1m"
#define log_UNDERLINE 	"\033[4m"
#define log_RESET 		"\033[0m"


class TIME
{
public:
    static const string get()
    {
        // Get the current time
        const auto & now = chrono::system_clock::now();
        const auto & in_time_t = chrono::system_clock::to_time_t(now);

        // Convert time_t to tm as local time
        tm buf{};
        localtime_r(&in_time_t, &buf);

        // Use stringstream to format the time
        ostringstream ss;
        ss << "[" << put_time(&buf, "%Y-%m-%d %H:%M:%S") << "]";
        return ss.str();
    }

    static string mili()
    {
        // Get the current time point
        auto now = chrono::system_clock::now();

        // Convert to time_t for seconds and tm for local time
        auto in_time_t = chrono::system_clock::to_time_t(now);
        tm buf{};
        localtime_r(&in_time_t, &buf);

        // Use stringstream to format the time
        std::ostringstream ss;
        ss << "[" << put_time(&buf, "%Y-%m-%d %H:%M:%S");

        // Calculate milliseconds (now time since epoch minus time_t converted back to time since epoch)
        auto since_epoch = now.time_since_epoch();
        auto s = chrono::duration_cast<chrono::seconds>(since_epoch);
        since_epoch -= s;
        auto ms = chrono::duration_cast<chrono::milliseconds>(since_epoch);

        // Append milliseconds to the formatted string, correctly placing the closing square bracket
        ss << "." << setfill('0') << setw(3) << ms.count() << "]";

        return ss.str();
    }
};

enum LogLevel
{
    INFO,
    INFO_PRIORITY,
    WARNING,
    ERROR,
    FUNC
};

typedef struct {
    string value;
} event_type_obj_t;

typedef struct {
    string value;
} FuncNameWrapper;

typedef struct {
    string value;
} file_name_obj_t;

typedef struct {
    int line;
} line_obj_t;

typedef struct {
    LogLevel level;
    string function;
    int line;
    string message;
    // Include a timestamp if you prefer logging it to be handled by the logger rather than each log call
} LogMessage;

typedef struct {
    uint32_t value;
} window_obj_t;

typedef struct {
    string value;
} errno_msg_t;

class lout
{
public:
    template<typename T>
    typename enable_if<is_arithmetic<T>::value, lout&>::type
    operator<<(T value)
    {
        buffer << value;
        return *this;
    }

    template<typename T>
    typename enable_if<!is_arithmetic<T>::value, lout&>::type
    operator<<(const T& message)
    {
        buffer << message;
        return *this;
    }

    lout& operator << ( const FuncNameWrapper& funcName )
    {
        currentFunction = funcName.value;
        return *this;
    }

    lout& operator << ( const file_name_obj_t& fileName )
    {
        current_file = fileName.value;
        return *this;
    }

    lout& operator << ( const line_obj_t& line ) {
        current_line = line.line;
        return *this;
    }

    lout& operator << ( const LogLevel& level )
    {
        currentLevel = level;
        return *this;
    }

    lout& operator << ( const char c )
    {
        if ( c == '\n' ) {
            logMessage();
            buffer.str("");
            return *this;
        }
        buffer << c;
        return *this;
    }

    void setFileToStdout ( const string& path ) {
        if ( path == "" ) {
            state = 1;
        }
        file.open(path, ios::app);
    }

    void setStdinToFile ( const string& path ) {
        if (freopen(path.c_str(), "a", stdin) == nullptr)
        {
            cerr << "Failed to redirect stdin to " << path << ": " << strerror(errno) << endl;
        }
    }

private:
    /* Variabels */
    LogLevel currentLevel;
    string currentFunction;
    string current_file;
    int current_line;
    ostringstream buffer;
    mutex log_mutex;

    u32 state;

    ofstream file;

    /* Methods 	 */
    void logMessage () {
        lock_guard<mutex> guard(log_mutex);
        if ( state == 1 ) {
            cout << TIME::mili() << ":" << getLogPrefix(currentLevel) << ":" << log_YELLOW << "[Line:" << current_line << "]" << log_RESET << ":" << log_MEGENTA << "[" << currentFunction << "]" << log_RESET << ": " << buffer.str() << "\n";
        }
        if ( file ) {
            file << TIME::mili() << ":" << getLogPrefix(currentLevel) << ":" << log_YELLOW << "[Line:" << current_line << "]" << log_RESET << ":" << log_MEGENTA << "[" << currentFunction << "]" << log_RESET << ": " << buffer.str() << "\n";
        }
    }
    string getLogPrefix(LogLevel level) const
    {
        switch (level)
        {
            case INFO:
            {
                return log_GREEN 	"[INFO]" 		  log_RESET;
            }
            case INFO_PRIORITY:
            {
                return log_CYAN		"[INFO_PRIORITY]" log_RESET;
            }
            case WARNING:
            {
                return log_YELLOW 	"[WARNING]" 	  log_RESET;
            }
            case ERROR:
            {
                return log_RED 		"[ERROR]" 		  log_RESET;
            }
            case FUNC:
            {
                return log_MEGENTA	"[FUNC]"		  log_RESET;
            }
            default:
            {
                return to_string(level);
            }
        }
    }
};
static class lout lout;

FORCE_INLINE FuncNameWrapper get_func_name ( const char* funcName ) {
    return { funcName };
}
FORCE_INLINE file_name_obj_t get_file_name ( const char* fileName ) {
    return { fileName };
}
FORCE_INLINE line_obj_t get_line ( int line ) {
    return { line };
}

#define loutNUM(x) #x << " = " << x
#define loutI lout << INFO << get_func_name(__func__) << get_line(__LINE__)
#pragma once

#include "Global.hpp"

namespace Engine {
    class Log {
        FORCE_INLINE Log* Inst () {
            if (instance == nullptr) {
                instance = new Log();
            }
            return instance;
        }
    public:
        Log(const Log&) = delete;
        Log& operator=(const Log&) = delete;

        FORCE_INLINE template <typename... Args>
        void log ( const Args...& args) {
            cout << getCurrentTime() << " - " << message << endl;
        }
    };
}
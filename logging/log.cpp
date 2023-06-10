#include <exception>
#include <iostream>
#include <fstream>


class Logger {
    Logger(const std::string& filename) : filename_(filename), log_file_(filename, std::ofstream::out){
        if (!log_file_.is_open()) {
            std::cout << "ERROR: file cannot be open\n";
            throw 1;
        }
    }   

    static Logger* logger_ptr_;
    std::ofstream log_file_;
    std::string filename_ = "";

    ~Logger() {
        log_file_.close();
        delete logger_ptr_;
    }

public:
    Logger(const Logger&) = delete;
    Logger() = delete;
    static Logger& GetLogger(const std::string& filename) {
        if (!logger_ptr_)
            logger_ptr_ = new Logger(filename);
        return *logger_ptr_; 
    }

    std::ostream& operator<< (const std::string& msg) {
        log_file_ << msg << std::endl;
        return log_file_;
    }
};



Logger* Logger::logger_ptr_ = nullptr;

int main() {
    Logger& LOG = Logger::GetLogger("../log/out.txt");

    LOG << "Test Log!";
    std::cout << "Test Log!";

    return 0;
}
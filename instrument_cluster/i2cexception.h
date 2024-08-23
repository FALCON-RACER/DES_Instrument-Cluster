#ifndef I2CEXCEPTION_H
#define I2CEXCEPTION_H

#include <exception>
#include <string>

class I2CException : public std::exception {

public:
    explicit I2CException(const std::string& message) : message_(message) {}

    virtual const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};
#endif // I2CEXCEPTION_H

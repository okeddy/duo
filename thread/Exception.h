#ifndef DUO_BASE_EXCEPTION_H
#define DUO_BASE_EXCEPTION_H

#include <exception>
#include <string>

namespace duo {
    class Exception : public std::exception
    {
    public:
        explicit Exception(const char* what);
        virtual ~Exception() throw();
        virtual const char* what() const throw();
        const char* stackTrace() const throw();

    private:
        std::string message_;
        std::string stack_;
    };
} // namespace duo


#endif  // DUO_BASE_EXCEPTION_H

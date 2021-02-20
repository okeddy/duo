/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：Exception.h
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#ifndef DUO_BASE_EXCEPTION_H
#define DUO_BASE_EXCEPTION_H

#include <exception>
#include <string>

namespace duo {
    class Exception : public std::exception {
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

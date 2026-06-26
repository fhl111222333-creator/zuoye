#ifndef CAMPUS_EXCEPTION_H
#define CAMPUS_EXCEPTION_H

#include <stdexcept>
#include <string>

// 项目自定义异常。
// TODO: 在更多文件读取和解析场景中使用该异常，而不是静默跳过错误。
class CampusException : public std::runtime_error {
public:
    CampusException(std::string code, std::string message)
        : std::runtime_error(message), code_(std::move(code)) {}

    const std::string& code() const { return code_; }

private:
    std::string code_;
};

#endif

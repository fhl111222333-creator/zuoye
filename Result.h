#ifndef RESULT_H
#define RESULT_H

#include <string>

// 请求处理状态。
// 注意：输出格式必须严格稳定，否则自动测试会失败。
enum class Status {
    ACCEPT,
    REJECT,
    PENDING,
    CANCELLED,
    ERROR
};

struct Result {
    Status status;
    std::string requestId;
    std::string reason;

    Result(Status s = Status::ERROR,
           std::string id = "UNKNOWN",
           std::string r = "")
        : status(s), requestId(std::move(id)), reason(std::move(r)) {}

    std::string statusText() const;
    std::string toLine() const;
};

#endif

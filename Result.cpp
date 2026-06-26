#include "Result.h"

std::string Result::statusText() const {
    switch (status) {
        case Status::ACCEPT: return "ACCEPT";
        case Status::REJECT: return "REJECT";
        case Status::PENDING: return "PENDING";
        case Status::CANCELLED: return "CANCELLED";
        case Status::ERROR: return "ERROR";
    }
    return "ERROR";
}

std::string Result::toLine() const {
    if (reason.empty()) {
        return statusText() + "," + requestId;
    }
    return statusText() + "," + requestId + "," + reason;
}

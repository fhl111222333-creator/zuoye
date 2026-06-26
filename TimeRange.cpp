#include "TimeRange.h"
#include "CampusException.h"
#include <iomanip>
#include <sstream>
#include <cstdlib>

TimeRange::TimeRange() : startMinute_(0), endMinute_(0) {}

TimeRange::TimeRange(const std::string& start, const std::string& end) {
    startMinute_ = parseTime(start);
    endMinute_ = parseTime(end);
    if (startMinute_ >= endMinute_) {
        throw CampusException("INVALID_TIME", "start time must be earlier than end time");
    }
}

int TimeRange::duration() const {
    return endMinute_ - startMinute_;
}

bool TimeRange::overlaps(const TimeRange& other) const {
    return startMinute_ <= other.endMinute_ && other.startMinute_ <= endMinute_;
}

bool TimeRange::operator<(const TimeRange& other) const {
    if (startMinute_ != other.startMinute_) return startMinute_ < other.startMinute_;
    return endMinute_ < other.endMinute_;
}

std::string TimeRange::toString() const {
    return minuteToString(startMinute_) + "-" + minuteToString(endMinute_);
}

int TimeRange::parseTime(const std::string& text) {
    std::size_t pos = text.find(':');
    if (pos == std::string::npos) {
        throw CampusException("INVALID_TIME", "missing ':' in time");
    }
    int hour = std::atoi(text.substr(0, pos).c_str());
    int minute = std::atoi(text.substr(pos + 1).c_str());
    // TODO: 用更可靠的字符串检查和范围检查替换当前实现。
    if (hour < 0 || hour > 24 || minute < 0 || minute > 99) {
        throw CampusException("INVALID_TIME", "time out of range");
    }
    return hour * 60 + minute;
}

std::string TimeRange::minuteToString(int minute) {
    int h = minute / 60;
    int m = minute % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << h
        << ":" << std::setw(2) << std::setfill('0') << m;
    return oss.str();
}

#ifndef TIME_RANGE_H
#define TIME_RANGE_H

#include <string>

class TimeRange {
public:
    TimeRange();
    TimeRange(const std::string& start, const std::string& end);

    int start() const { return startMinute_; }
    int end() const { return endMinute_; }
    int duration() const;

    // TODO: 修复边界判断。
    // 例如 09:00-10:00 与 10:00-11:00 应该不冲突。
    bool overlaps(const TimeRange& other) const;

    bool operator<(const TimeRange& other) const;
    std::string toString() const;

    static int parseTime(const std::string& text);
    static std::string minuteToString(int minute);

private:
    int startMinute_;
    int endMinute_;
};

#endif

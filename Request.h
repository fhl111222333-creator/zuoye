#ifndef REQUEST_H
#define REQUEST_H

#include "TimeRange.h"
#include <memory>
#include <string>

class Request {
public:
    Request(std::string id, std::string userId, std::string resourceId)
        : id_(std::move(id)), userId_(std::move(userId)), resourceId_(std::move(resourceId)) {}

    const std::string& id() const { return id_; }
    const std::string& userId() const { return userId_; }
    const std::string& resourceId() const { return resourceId_; }

    virtual std::string type() const = 0;
    ~Request() = default;

protected:
    std::string id_;
    std::string userId_;
    std::string resourceId_;
};

class BookingRequest : public Request {
public:
    BookingRequest(std::string id,
                   std::string userId,
                   std::string resourceId,
                   TimeRange range);

    std::string type() const override;
    const TimeRange& range() const { return range_; }
    // TODO: 应包含请求类型、开始时间和结束时间。
    std::string duplicateKey() const;

private:
    TimeRange range_;
};

class RepairRequest : public Request {
public:
    RepairRequest(std::string id,
                  std::string userId,
                  std::string resourceId,
                  std::string description);

    std::string type() const override;
    const std::string& description() const { return description_; }

private:
    std::string description_;
};

// TODO: 未来可以加入 CancelRequest / ApproveRequest。

#endif

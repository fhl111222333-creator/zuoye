#include "Request.h"

BookingRequest::BookingRequest(std::string id,
                               std::string userId,
                               std::string resourceId,
                               TimeRange range)
    : Request(std::move(id), std::move(userId), std::move(resourceId)),
      range_(range) {}

std::string BookingRequest::type() const { return "booking"; }

std::string BookingRequest::duplicateKey() const {
    return userId_ + "|" + resourceId_;
}

RepairRequest::RepairRequest(std::string id,
                             std::string userId,
                             std::string resourceId,
                             std::string description)
    : Request(std::move(id), std::move(userId), std::move(resourceId)),
      description_(std::move(description)) {}

std::string RepairRequest::type() const { return "repair"; }

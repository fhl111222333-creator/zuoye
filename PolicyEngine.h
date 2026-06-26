#ifndef POLICY_ENGINE_H
#define POLICY_ENGINE_H

#include "Request.h"
#include "Resource.h"
#include "Result.h"
#include "User.h"
#include <set>
#include <string>
#include <utility>
#include <vector>

struct MaintenanceWindow {
    std::string resourceId;
    TimeRange range;
};

struct PolicyContext {
    const std::vector<BookingRequest>* acceptedBookings;
    const std::vector<MaintenanceWindow>* maintenanceWindows;
    const std::set<std::string>* duplicateKeys;
};

class PolicyEngine {
public:
    Result evaluateBooking(const BookingRequest& request,
                           const User& user,
                           const Resource& resource,
                           const PolicyContext& context) const;

    Result evaluateRepair(const RepairRequest& request,
                          const User& user,
                          const Resource& resource) const;

private:
    bool hasResourceConflict(const BookingRequest& request,
                             const Resource& resource,
                             const std::vector<BookingRequest>& accepted) const;

    bool isUnderMaintenance(const BookingRequest& request,
                            const std::vector<MaintenanceWindow>& windows) const;

    bool hasUserConflict(const BookingRequest& request,
                         const std::vector<BookingRequest>& accepted) const;
};

#endif

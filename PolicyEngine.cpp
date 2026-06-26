#include "PolicyEngine.h"

Result PolicyEngine::evaluateBooking(const BookingRequest& request,
                                      const User& user,
                                      const Resource& resource,
                                      const PolicyContext& context) const {
    if (context.duplicateKeys && context.duplicateKeys->count(request.duplicateKey())) {
        return Result(Status::REJECT, request.id(), "DUPLICATE_REQUEST");
    }

    // TODO: maintenance 规则应在预约通过前检查。
    // 当前没有调用 isUnderMaintenance，因此维护时间段内也可能被接受。

    if (user.role() == "student" && resource.isAdvanced()) {
        return Result(Status::PENDING, request.id(), "NEED_APPROVAL");
    }

    if (context.acceptedBookings && hasResourceConflict(request, resource, *context.acceptedBookings)) {
        return Result(Status::REJECT, request.id(), "RESOURCE_CONFLICT");
    }

    // TODO: 新规则可能要求“同一用户不能同时占用多个资源”。
    // 当前 hasUserConflict 没有被使用。

    return Result(Status::ACCEPT, request.id());
}

Result PolicyEngine::evaluateRepair(const RepairRequest& request,
                                     const User& user,
                                     const Resource& resource) const {
    (void)resource;
    // TODO: repair 请求应该不占用时间，但需要权限判断和 repair_log 输出。
    // 当前版把 repair 视为未支持功能。
    if (!user.canSubmitRepair()) {
        return Result(Status::REJECT, request.id(), "PERMISSION_DENIED");
    }
    return Result(Status::ERROR, request.id(), "TODO_REPAIR_NOT_IMPLEMENTED");
}

bool PolicyEngine::hasResourceConflict(const BookingRequest& request,
                                       const Resource& resource,
                                       const std::vector<BookingRequest>& accepted) const {
    (void)resource;
    for (const auto& oldReq : accepted) {
        if (oldReq.resourceId() == request.resourceId() && oldReq.range().overlaps(request.range())) {
            return true;
        }
    }
    return false;
}

bool PolicyEngine::isUnderMaintenance(const BookingRequest& request,
                                      const std::vector<MaintenanceWindow>& windows) const {
    for (const auto& w : windows) {
        if (w.resourceId == request.resourceId() && w.range.overlaps(request.range())) {
            return true;
        }
    }
    return false;
}

bool PolicyEngine::hasUserConflict(const BookingRequest& request,
                                   const std::vector<BookingRequest>& accepted) const {
    for (const auto& oldReq : accepted) {
        if (oldReq.userId() == request.userId() && oldReq.range().overlaps(request.range())) {
            return true;
        }
    }
    return false;
}

#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "PolicyEngine.h"
#include "Repository.h"
#include "Resource.h"
#include "Result.h"
#include "User.h"
#include <memory>
#include <set>
#include <string>
#include <vector>

struct LoadedRequest {
    std::string requestId;
    std::string rawType;
    std::unique_ptr<Request> request;
    bool parseOk = false;
    std::string errorCode;
};

class SystemManager {
public:
    void loadUsers(const std::string& path);
    void loadResources(const std::string& path);
    void loadMaintenance(const std::string& path);
    void loadRequests(const std::string& path);

    void processAll();
    void writeOutputs(const std::string& outputDir) const;

private:
    Result processOne(const LoadedRequest& loaded);
    void addResult(const Result& r);
    void writeResultFile(const std::string& path) const;
    void writeErrorLog(const std::string& path) const;
    void writeRepairLog(const std::string& path) const;
    void writeSummary(const std::string& path) const;

    Repository<User> users_;
    Repository<Resource> resources_;

    std::vector<LoadedRequest> requests_;
    std::vector<BookingRequest> acceptedBookings_;
    std::vector<MaintenanceWindow> maintenance_;
    std::set<std::string> duplicateKeys_;
    std::vector<Result> results_;
    std::vector<Result> errors_;
    std::vector<std::string> repairLines_;

    PolicyEngine policy_;
};

#endif

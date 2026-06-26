#include "SystemManager.h"
#include "CampusException.h"
#include "CsvReader.h"
#include <fstream>
#include <iostream>
#include <map>

void SystemManager::loadUsers(const std::string& path) {
    auto rows = CsvReader::read(path);
    for (const auto& row : rows) {
        // TODO: 坏格式用户行不应静默跳过，应写入错误日志或加载报告。
        if (row.size() != 3) continue;
        try {
            users_.add(row[0], createUser(row[0], row[1], row[2]));
        } catch (const CampusException&) {
            continue;
        }
    }
}

void SystemManager::loadResources(const std::string& path) {
    auto rows = CsvReader::read(path);
    for (const auto& row : rows) {
        // TODO: 未来 vehicle 可能有第 5 列 capacity。
        if (row.size() < 4) continue;
        try {
            resources_.add(row[0], createResource(row[0], row[1], row[2], row[3]));
        } catch (const CampusException&) {
            continue;
        }
    }
}

void SystemManager::loadMaintenance(const std::string& path) {
    auto rows = CsvReader::read(path);
    for (const auto& row : rows) {
        if (row.size() != 3) continue;
        try {
            maintenance_.push_back({row[0], TimeRange(row[1], row[2])});
        } catch (const CampusException&) {
            // TODO: 维护时间格式错误也应记录。
        }
    }
}

void SystemManager::loadRequests(const std::string& path) {
    auto rows = CsvReader::read(path);
    for (const auto& row : rows) {
        LoadedRequest loaded;
        if (row.empty()) continue;
        loaded.requestId = row[0];
        if (row.size() < 4) {
            loaded.parseOk = false;
            loaded.errorCode = "BAD_FORMAT";
            requests_.push_back(std::move(loaded));
            continue;
        }

        loaded.rawType = row[3];
        try {
            if (row[3] == "booking") {
                if (row.size() != 6) {
                    loaded.parseOk = false;
                    loaded.errorCode = "BAD_FORMAT";
                } else {
                    loaded.request = std::make_unique<BookingRequest>(
                        row[0], row[1], row[2], TimeRange(row[4], row[5]));
                    loaded.parseOk = true;
                }
            } else if (row[3] == "repair") {
                if (row.size() != 5) {
                    loaded.parseOk = false;
                    loaded.errorCode = "BAD_FORMAT";
                } else {
                    // TODO: repair 的处理逻辑尚不完整。
                    loaded.request = std::make_unique<RepairRequest>(row[0], row[1], row[2], row[4]);
                    loaded.parseOk = true;
                }
            } else {
                loaded.parseOk = false;
                loaded.errorCode = "UNSUPPORTED_REQUEST";
            }
        } catch (const CampusException& e) {
            loaded.parseOk = false;
            loaded.errorCode = e.code();
        }
        requests_.push_back(std::move(loaded));
    }
}

void SystemManager::processAll() {
    for (const auto& r : requests_) {
        Result result = processOne(r);
        addResult(result);
    }
}

Result SystemManager::processOne(const LoadedRequest& loaded) {
    if (!loaded.parseOk) {
        return Result(Status::ERROR, loaded.requestId.empty() ? "UNKNOWN" : loaded.requestId, loaded.errorCode);
    }

    Request* base = loaded.request.get();
    User* user = users_.find(base->userId());
    if (!user) {
        return Result(Status::ERROR, base->id(), "UNKNOWN_USER");
    }

    Resource* resource = resources_.find(base->resourceId());
    if (!resource) {
        return Result(Status::ERROR, base->id(), "UNKNOWN_RESOURCE");
    }

    if (base->type() == "booking") {
        auto* booking = dynamic_cast<BookingRequest*>(base);
        PolicyContext ctx{&acceptedBookings_, &maintenance_, &duplicateKeys_};
        Result result = policy_.evaluateBooking(*booking, *user, *resource, ctx);

        // 注意：这里在评估后才记录 duplicateKey，因此同一请求的第一次不会被判重。
        duplicateKeys_.insert(booking->duplicateKey());

        if (result.status == Status::ACCEPT) {
            acceptedBookings_.push_back(*booking);
        }
        return result;
    }

    if (base->type() == "repair") {
        auto* repair = dynamic_cast<RepairRequest*>(base);
        Result result = policy_.evaluateRepair(*repair, *user, *resource);
        // TODO: repair 成功时应写入 repair_log.txt，而不是只返回结果。
        return result;
    }

    return Result(Status::ERROR, base->id(), "UNSUPPORTED_REQUEST");
}

void SystemManager::addResult(const Result& r) {
    results_.push_back(r);
    if (r.status == Status::ERROR) {
        errors_.push_back(r);
    }
}

void SystemManager::writeOutputs(const std::string& outputDir) const {
    std::string prefix = outputDir;
    if (!prefix.empty() && prefix.back() != '/' && prefix.back() != '\\') prefix += "/";
    writeResultFile(prefix + "result.txt");
    writeErrorLog(prefix + "error_log.txt");
    writeRepairLog(prefix + "repair_log.txt");
    writeSummary(prefix + "summary.txt");
}

void SystemManager::writeResultFile(const std::string& path) const {
    std::ofstream fout(path);
    for (const auto& r : results_) {
        fout << r.toLine() << "\n";
    }
}

void SystemManager::writeErrorLog(const std::string& path) const {
    std::ofstream fout(path);
    for (const auto& e : errors_) {
        fout << e.toLine() << "\n";
    }
}

void SystemManager::writeRepairLog(const std::string& path) const {
    std::ofstream fout(path);
    for (const auto& line : repairLines_) {
        fout << line << "\n";
    }
}

void SystemManager::writeSummary(const std::string& path) const {
    std::map<std::string, int> counts;
    counts["ACCEPT"] = 0;
    counts["REJECT"] = 0;
    counts["PENDING"] = 0;
    counts["CANCELLED"] = 0;
    counts["ERROR"] = 0;

    for (const auto& r : results_) {
        counts[r.statusText()]++;
    }

    std::ofstream fout(path);
    fout << "ACCEPT=" << counts["ACCEPT"] << "\n";
    fout << "REJECT=" << counts["REJECT"] << "\n";
    fout << "PENDING=" << counts["PENDING"] << "\n";
    fout << "CANCELLED=" << counts["CANCELLED"] << "\n";
    fout << "ERROR=" << counts["ERROR"] << "\n";

    // TODO: 根据需求增加 MOST_USED_RESOURCE，并处理并列情况。
}

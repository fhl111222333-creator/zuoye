#ifndef RESOURCE_H
#define RESOURCE_H

#include <memory>
#include <string>

class Resource {
public:
    Resource(std::string id, std::string name, std::string level)
        : id_(std::move(id)), name_(std::move(name)), level_(std::move(level)) {}

    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }
    const std::string& level() const { return level_; }
    bool isAdvanced() const { return level_ == "advanced"; }

    virtual std::string type() const = 0;
    virtual bool isShareable() const { return false; }
    virtual int capacity() const { return 1; }

    ~Resource() = default;

protected:
    std::string id_;
    std::string name_;
    std::string level_;
};

class RoomResource : public Resource {
public:
    RoomResource(std::string id, std::string name, std::string level);
    std::string type() const override;
};

class DeviceResource : public Resource {
public:
    DeviceResource(std::string id, std::string name, std::string level);
    std::string type() const override;
};

// TODO for emergency change: 未来可能新增 VehicleResource，具有 capacity。

std::unique_ptr<Resource> createResource(const std::string& id,
                                         const std::string& type,
                                         const std::string& name,
                                         const std::string& level);

#endif

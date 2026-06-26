#include "Resource.h"
#include "CampusException.h"

RoomResource::RoomResource(std::string id, std::string name, std::string level)
    : Resource(std::move(id), std::move(name), std::move(level)) {}
std::string RoomResource::type() const { return "room"; }

DeviceResource::DeviceResource(std::string id, std::string name, std::string level)
    : Resource(std::move(id), std::move(name), std::move(level)) {}
std::string DeviceResource::type() const { return "device"; }

std::unique_ptr<Resource> createResource(const std::string& id,
                                         const std::string& type,
                                         const std::string& name,
                                         const std::string& level) {
    if (type == "room") return std::make_unique<RoomResource>(id, name, level);
    if (type == "device") return std::make_unique<DeviceResource>(id, name, level);

    // TODO: 支持 vehicle 类型，并处理 capacity 字段。
    throw CampusException("BAD_RESOURCE_TYPE", "unknown resource type: " + type);
}

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <map>
#include <memory>
#include <string>
#include <vector>

// 一个简单的泛型仓库，用于按 id 管理对象。
// TODO: 补充重复 id 处理、异常处理和 const 正确性。
template <class T>
class Repository {
public:
    void add(const std::string& id, std::unique_ptr<T> item) {
        data_[id] = std::move(item);
    }

    T* find(const std::string& id) const {
        auto it = data_.find(id);
        if (it == data_.end()) return nullptr;
        return it->second.get();
    }

    bool contains(const std::string& id) const {
        return data_.find(id) != data_.end();
    }

    std::vector<T*> all() const {
        std::vector<T*> result;
        for (const auto& kv : data_) {
            result.push_back(kv.second.get());
        }
        return result;
    }

    std::size_t size() const { return data_.size(); }

private:
    std::map<std::string, std::unique_ptr<T>> data_;
};

#endif

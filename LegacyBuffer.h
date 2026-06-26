#ifndef LEGACY_BUFFER_H
#define LEGACY_BUFFER_H

#include <cstddef>

// 前同事留下的旧代码：一个手写动态数组。
// 这个类目前没有被主流程大量使用，但隐藏测试可能检查它。
template <class T>
class LegacyBuffer {
public:
    explicit LegacyBuffer(std::size_t n = 0) : size_(n), data_(n ? new T[n] : nullptr) {}

    ~LegacyBuffer() {
        delete[] data_;
    }
    LegacyBuffer(const LegacyBuffer& other) : size_(other.size_), data_(other.data_) {}

    // TODO: 实现深拷贝赋值运算符，并处理自赋值。
    LegacyBuffer& operator=(const LegacyBuffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;  // BUG: shallow copy
        }
        return *this;
    }

    T& operator[](std::size_t i) { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }
    std::size_t size() const { return size_; }

private:
    std::size_t size_;
    T* data_;
};

#endif

#ifndef USER_H
#define USER_H

#include <memory>
#include <string>

class User {
public:
    User(std::string id, std::string name) : id_(std::move(id)), name_(std::move(name)) {}

    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }

    virtual std::string role() const = 0;
    virtual int basePriority() const = 0;
    virtual bool canApprove() const = 0;
    virtual bool canSubmitRepair() const = 0;
    ~User() = default;

protected:
    std::string id_;
    std::string name_;
};

class StudentUser : public User {
public:
    StudentUser(std::string id, std::string name);
    std::string role() const override;
    int basePriority() const override;
    bool canApprove() const override;
    bool canSubmitRepair() const override;
};

class TeacherUser : public User {
public:
    TeacherUser(std::string id, std::string name);
    std::string role() const override;
    int basePriority() const override;
    bool canApprove() const override;
    bool canSubmitRepair() const override;
};

class AdminUser : public User {
public:
    AdminUser(std::string id, std::string name);
    std::string role() const override;
    int basePriority() const override;
    bool canApprove() const override;
    bool canSubmitRepair() const override;
};

// TODO for emergency change: 未来可能新增 VisitorUser。
// 请不要把所有权限规则都写死在 if(role == "student") 中。

std::unique_ptr<User> createUser(const std::string& id,
                                 const std::string& role,
                                 const std::string& name);

#endif

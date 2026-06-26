#include "User.h"
#include "CampusException.h"

StudentUser::StudentUser(std::string id, std::string name) : User(std::move(id), std::move(name)) {}
std::string StudentUser::role() const { return "student"; }
int StudentUser::basePriority() const { return 10; }
bool StudentUser::canApprove() const { return false; }
bool StudentUser::canSubmitRepair() const { return false; }

TeacherUser::TeacherUser(std::string id, std::string name) : User(std::move(id), std::move(name)) {}
std::string TeacherUser::role() const { return "teacher"; }
int TeacherUser::basePriority() const { return 20; }
bool TeacherUser::canApprove() const { return true; }
bool TeacherUser::canSubmitRepair() const { return true; }

AdminUser::AdminUser(std::string id, std::string name) : User(std::move(id), std::move(name)) {}
std::string AdminUser::role() const { return "admin"; }
int AdminUser::basePriority() const { return 30; }
bool AdminUser::canApprove() const { return true; }
bool AdminUser::canSubmitRepair() const { return true; }

std::unique_ptr<User> createUser(const std::string& id,
                                 const std::string& role,
                                 const std::string& name) {
    if (role == "student") return std::make_unique<StudentUser>(id, name);
    if (role == "teacher") return std::make_unique<TeacherUser>(id, name);
    if (role == "admin") return std::make_unique<AdminUser>(id, name);

    // TODO: 更友好地记录非法用户类型，而不是直接抛出。
    throw CampusException("BAD_USER_ROLE", "unknown user role: " + role);
}

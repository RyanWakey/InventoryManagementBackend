#ifndef ROLES_H
#define ROLES_H

#include <string>
#include <vector>

class Roles {
private:
    int id;
    std::string name;

public:
    Roles();
    Roles(int id, const std::string& name);

    int getId() const noexcept { return id; }
    const std::string& getName() const noexcept { return name; }

    void setId(int newId) noexcept { id = newId; }
    void setName(const std::string& newName) noexcept { name = newName; }

    static void createRole(const std::string& roleName);
    static Roles readRole(int roleId);
    static std::vector<Roles> getAllRoles();
    static void updateRole(int roleId, const std::string& newRoleName);
    static void deleteRole(int roleId);
};

#endif // ROLES_H

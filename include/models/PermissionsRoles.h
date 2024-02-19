#ifndef PERMISSIONSROLES_H
#define PERMISSIONSROLES_H

#include <memory>
#include "Roles.h"
#include "Permissions.h"
#include <vector>


class PermissionsRoles {
private:

    int roleId;
    int permissionsId;
    std::shared_ptr<Roles> role;
    std::shared_ptr<Permissions> permissions;

public:

    PermissionsRoles();
    PermissionsRoles(int roleId, int permissionsId);

    // Accessors
    int getRoleId() const noexcept { return roleId; }
    int getPermissionsId() const noexcept { return permissionsId; }
    std::shared_ptr<Roles> getRole() const noexcept { return role; }
    std::shared_ptr<Permissions> getPermissions() const noexcept { return permissions; }

    // Mutators
    void setRoleId(int newRoleId) noexcept { roleId = newRoleId; }
    void setPermissionsId(int newPermissionsId) noexcept { permissionsId = newPermissionsId; }
    void setRole(const std::shared_ptr<Roles>& newRole) noexcept { role = newRole; }
    void setPermissions(const std::shared_ptr<Permissions>& newPermissions) noexcept { permissions = newPermissions; }

    static void createPermissionToRole(int roleId, int permissionId);
    static void deletePermissionFromRole(int roleId, int permissionId);
    static std::vector<int> getPermissionsForRole(int roleId);   
     
};

#endif // PERMISSIONSROLES_H

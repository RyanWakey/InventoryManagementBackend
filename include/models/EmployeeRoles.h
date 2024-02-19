#ifndef EMPLOYEEROLES_H
#define EMPLOYEEROLES_H

#include "Roles.h"
#include "Employee.h"
#include <memory>
#include <vector>

class EmployeeRoles {


private:

    int employeeId;
    int roleId;

    std::shared_ptr<Employee> employee;
    std::shared_ptr<Roles> roles;

public:

    EmployeeRoles();
    EmployeeRoles(int employeeId, int roleId);    

    int getEmplyoeeId() { return employeeId; }
    int getRoleId() const noexcept { return roleId; }
    std::shared_ptr<Employee> getEmployee() { return employee; }
    std::shared_ptr<Roles> getRole() const noexcept { return roles; }
    
    void setRoleId(int newRoleId) noexcept { roleId = newRoleId; }
    void setEmployeeId(int newEmployeeId) noexcept { employeeId = newEmployeeId; }
    void setRole(const std::shared_ptr<Roles>& newRoles) noexcept { roles = newRoles; }
    void setPermissions(const std::shared_ptr<Employee>& newEmployee) noexcept { employee = newEmployee; }

    static void createRoleToEmployee(int employeeId, int roleId);
    static void deleteRoleFromEmployee(int employeeId, int roleId);
    static std::vector<int> readRolesForEmployee(int employeeId);

    static crow::json::wvalue EmployeeRoles::convertRolesToJSON(const std::vector<int>& roleIds);
};

#endif
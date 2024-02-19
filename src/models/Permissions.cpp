#include "../../include/models/Permissions.h"
#include "../../include/database/DB_Manager.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>


Permissions::Permissions()
     : id(0), Description(""),
      canCreateProduct(false), canDeleteProduct(false), canUpdateProduct(false), canViewProduct(false),
      canCreateCategory(false), canDeleteCategory(false), canUpdateCategory(false), canViewCategory(false),
      canCreateCustomer(false), canDeleteCustomer(false), canUpdateCustomer(false), canViewCustomer(false),
      canCreateEmployee(false), canDeleteEmployee(false), canUpdateEmployee(false), canViewEmployee(false),
      canCreatePurchaseOrder(false), canDeletePurchaseOrder(false), canUpdatePurchaseOrder(false), canViewPurchaseOrder(false),
      canCreateRole(false), canDeleteRole(false), canUpdateRole(false), canViewRole(false),
      canCreateSale(false), canDeleteSale(false), canUpdateSale(false), canViewSale(false),
      canCreateSupplier(false), canDeleteSupplier(false), canUpdateSupplier(false), canViewSupplier(false),
      canCreateTransaction(false), canDeleteTransaction(false), canUpdateTransaction(false), canViewTransaction(false),
      canManageSystemSettings(false), canManageEmployeePermissions(false) {}

Permissions::Permissions(int id, const std::string& description, 
            bool canCreateProduct, bool canDeleteProduct, bool canUpdateProduct, bool canViewProduct,
            bool canCreateCategory, bool canDeleteCategory, bool canUpdateCategory, bool canViewCategory, 
            bool canCreateCustomer, bool canDeleteCustomer, bool canUpdateCustomer, bool canViewCustomer, 
            bool canCreateEmployee, bool canDeleteEmployee, bool canUpdateEmployee, bool canViewEmployee,
            bool canCreatePurchaseOrder, bool canDeletePurchaseOrder, bool canUpdatePurchaseOrder, bool canViewPurchaseOrder,
            bool canCreateRole, bool canDeleteRole, bool canUpdateRole, bool canViewRole,
            bool canCreateSale, bool canDeleteSale, bool canUpdateSale, bool canViewSale,
            bool canCreateSupplier, bool canDeleteSupplier, bool canUpdateSupplier, bool canViewSupplier,
            bool canCreateTransaction, bool canDeleteTransaction, bool canUpdateTransaction, bool canViewTransaction,
            bool canManageSystemSettings, bool canManageEmployeePermissions) : 
            id(id), Description(description),
            canCreateProduct(canCreateProduct), canDeleteProduct(canDeleteProduct), canUpdateProduct(canUpdateProduct), canViewProduct(canViewProduct),
            canCreateCategory(canCreateCategory), canDeleteCategory(canDeleteCategory), canUpdateCategory(canUpdateCategory), canViewCategory(canViewCategory),
            canCreateCustomer(canCreateCustomer), canDeleteCustomer(canDeleteCustomer), canUpdateCustomer(canUpdateCustomer), canViewCustomer(canViewCustomer),
            canCreateEmployee(canCreateEmployee), canDeleteEmployee(canDeleteEmployee), canUpdateEmployee(canUpdateEmployee), canViewEmployee(canViewEmployee),
            canCreatePurchaseOrder(canCreatePurchaseOrder), canDeletePurchaseOrder(canDeletePurchaseOrder), canUpdatePurchaseOrder(canUpdatePurchaseOrder), canViewPurchaseOrder(canViewPurchaseOrder),
            canCreateRole(canCreateRole), canDeleteRole(canDeleteRole), canUpdateRole(canUpdateRole), canViewRole(canViewRole),
            canCreateSale(canCreateSale), canDeleteSale(canDeleteSale), canUpdateSale(canUpdateSale), canViewSale(canViewSale),
            canCreateSupplier(canCreateSupplier), canDeleteSupplier(canDeleteSupplier), canUpdateSupplier(canUpdateSupplier), canViewSupplier(canViewSupplier),
            canCreateTransaction(canCreateTransaction), canDeleteTransaction(canDeleteTransaction), canUpdateTransaction(canUpdateTransaction), canViewTransaction(canViewTransaction),
            canManageSystemSettings(canManageSystemSettings), canManageEmployeePermissions(canManageEmployeePermissions) { }

Permissions Permissions::readPermissions(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT PermissionsID, Description, CanCreateProduct, CanDeleteProduct, CanUpdateProduct, CanViewProduct, "
            "CanCreateCategory, CanDeleteCategory, CanUpdateCategory, CanViewCategory, "
            "CanCreateCustomer, CanDeleteCustomer, CanUpdateCustomer, CanViewCustomer, "
            "CanCreateEmployee, CanDeleteEmployee, CanUpdateEmployee, CanViewEmployee, "
            "CanCreatePurchaseOrder, CanDeletePurchaseOrder, CanUpdatePurchaseOrder, CanViewPurchaseOrder, "
            "CanCreateRole, CanDeleteRole, CanUpdateRole, CanViewRole, "
            "CanCreateSale, CanDeleteSale, CanUpdateSale, CanViewSale, "
            "CanCreateSupplier, CanDeleteSupplier, CanUpdateSupplier, CanViewSupplier, "
            "CanCreateTransaction, CanDeleteTransaction, CanUpdateTransaction, CanViewTransaction, "
            "CanManageSystemSettings, CanManageEmployeePermissions FROM Permissions WHERE PermissionsID = ?"));

        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Permissions permissions(
                res->getInt("PermissionsID"),
                res->getString("Description"),
                res->getBoolean("CanCreateProduct"),
                res->getBoolean("CanDeleteProduct"),
                res->getBoolean("CanUpdateProduct"),
                res->getBoolean("CanViewProduct"),
                res->getBoolean("CanCreateCategory"),
                res->getBoolean("CanDeleteCategory"),
                res->getBoolean("CanUpdateCategory"),
                res->getBoolean("CanViewCategory"),
                res->getBoolean("CanCreateCustomer"),
                res->getBoolean("CanDeleteCustomer"),
                res->getBoolean("CanUpdateCustomer"),
                res->getBoolean("CanViewCustomer"),
                res->getBoolean("CanCreateEmployee"),
                res->getBoolean("CanDeleteEmployee"),
                res->getBoolean("CanUpdateEmployee"),
                res->getBoolean("CanViewEmployee"),
                res->getBoolean("CanCreatePurchaseOrder"),
                res->getBoolean("CanDeletePurchaseOrder"),
                res->getBoolean("CanUpdatePurchaseOrder"),
                res->getBoolean("CanViewPurchaseOrder"),
                res->getBoolean("CanCreateRole"),
                res->getBoolean("CanDeleteRole"),
                res->getBoolean("CanUpdateRole"),
                res->getBoolean("CanViewRole"),
                res->getBoolean("CanCreateSale"),
                res->getBoolean("CanDeleteSale"),
                res->getBoolean("CanUpdateSale"),
                res->getBoolean("CanViewSale"),
                res->getBoolean("CanCreateSupplier"),
                res->getBoolean("CanDeleteSupplier"),
                res->getBoolean("CanUpdateSupplier"),
                res->getBoolean("CanViewSupplier"),
                res->getBoolean("CanCreateTransaction"),
                res->getBoolean("CanDeleteTransaction"),
                res->getBoolean("CanUpdateTransaction"),
                res->getBoolean("CanViewTransaction"),
                res->getBoolean("CanManageSystemSettings"),
                res->getBoolean("CanManageEmployeePermissions")
            );
            return permissions;
        } else {
            throw std::runtime_error("Permissions not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readPermissions: " << e.what() << std::endl;
        throw; // Re-throw the exception for the caller to handle
    }
}
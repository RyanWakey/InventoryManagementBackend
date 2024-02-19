#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <string>

class Permissions {
    
private:

    int id;
    std::string Description;

    bool canCreateProduct;
    bool canDeleteProduct;
    bool canUpdateProduct;
    bool canViewProduct;
    
    
    bool canCreateCategory;
    bool canDeleteCategory;
    bool canUpdateCategory;
    bool canViewCategory;
    
    bool canCreateCustomer;
    bool canDeleteCustomer;
    bool canUpdateCustomer;
    bool canViewCustomer;

    bool canCreateEmployee;
    bool canDeleteEmployee;
    bool canUpdateEmployee;
    bool canViewEmployee;

    bool canCreatePurchaseOrder;
    bool canDeletePurchaseOrder;
    bool canUpdatePurchaseOrder;
    bool canViewPurchaseOrder;

    bool canCreateRole;
    bool canDeleteRole;
    bool canUpdateRole;
    bool canViewRole;  

    bool canCreateSale;
    bool canDeleteSale;
    bool canUpdateSale;
    bool canViewSale;

    bool canCreateSupplier;
    bool canDeleteSupplier;
    bool canUpdateSupplier;
    bool canViewSupplier;

    bool canCreateTransaction;
    bool canDeleteTransaction;
    bool canUpdateTransaction;
    bool canViewTransaction;

    bool canManageSystemSettings;
    bool canManageEmployeePermissions;
    
    

public:

    Permissions();
    Permissions(int id, const std::string& description, 
            bool canCreateProduct, bool canDeleteProduct, bool canUpdateProduct, bool canViewProduct,
            bool canCreateCategory, bool canDeleteCategory, bool canUpdateCategory, bool canViewCategory, 
            bool canCreateCustomer, bool canDeleteCustomer, bool canUpdateCustomer, bool canViewCustomer, 
            bool canCreateEmployee, bool canDeleteEmployee, bool canUpdateEmployee, bool canViewEmployee,
            bool canCreatePurchaseOrder, bool canDeletePurchaseOrder, bool canUpdatePurchaseOrder, bool canViewPurchaseOrder,
            bool canCreateRole, bool canDeleteRole, bool canUpdateRole, bool canViewRole,
            bool canCreateSale, bool canDeleteSale, bool canUpdateSale, bool canViewSale,
            bool canCreateSupplier, bool canDeleteSupplier, bool canUpdateSupplier, bool canViewSupplier,
            bool canCreateTransaction, bool canDeleteTransaction, bool canUpdateTransaction, bool canViewTransaction,
            bool canManageSystemSettings, bool canManageEmployeePermissions);

    

    int getId() const noexcept { return id; }
    const std::string& getDescription() const noexcept { return Description; }

    bool getCanCreateProduct() const noexcept { return canCreateProduct; }
    bool getCanDeleteProduct() const noexcept { return canDeleteProduct; }
    bool getCanUpdateProduct() const noexcept { return canUpdateProduct; }
    bool getCanViewProduct() const noexcept { return canViewProduct; }
    
    bool getCanCreateCategory() const noexcept { return canCreateCategory; }
    bool getCanDeleteCategory() const noexcept { return canDeleteCategory; }
    bool getCanUpdateCategory() const noexcept { return canUpdateCategory; }
    bool getCanViewCategory() const noexcept { return canViewCategory; }

    bool getCanCreateCustomer() const noexcept { return canCreateCustomer; }
    bool getCanDeleteCustomer() const noexcept { return canDeleteCustomer; }
    bool getCanUpdateCustomer() const noexcept { return canUpdateCustomer; }
    bool getCanViewCustomer() const noexcept { return canViewCustomer; }

    bool getCanCreateEmployee() const noexcept { return canCreateEmployee; }
    bool getCanDeleteEmployee() const noexcept { return canDeleteProduct; }
    bool getCanUpdateEmployee() const noexcept { return canUpdateEmployee; }
    bool getCanViewEmployee() const noexcept { return canViewEmployee; }

    bool getCanCreatePurchaseOrder() const noexcept { return canCreatePurchaseOrder; }
    bool getCanDeletePurchaseOrder() const noexcept { return canDeletePurchaseOrder; }
    bool getCanUpdatePurchaseOrder() const noexcept { return canUpdatePurchaseOrder; }
    bool getCanViewPurchaseOrder() const noexcept { return canViewPurchaseOrder; }

    bool getCanCreateRole() const noexcept { return canCreateRole; }
    bool getCanDeleteRole() const noexcept { return canDeleteRole; }
    bool getCanUpdateRole() const noexcept { return canUpdateRole; }
    bool getCanViewRole() const noexcept { return canViewRole; }

    bool getCanCreateSale() const noexcept { return canCreateSale; }
    bool getCanDeleteSale() const noexcept { return canDeleteSale; }
    bool getCanUpdateSale() const noexcept { return canUpdateSale; }
    bool getCanViewSale() const noexcept { return canViewSale; }

    bool getCanCreateSupplier() const noexcept { return canCreateSupplier; }
    bool getCanDeleteSupplier() const noexcept { return canDeleteSupplier; }
    bool getCanUpdateSupplier() const noexcept { return canUpdateSupplier; }
    bool getCanViewSupplier() const noexcept { return canViewSupplier; }

    bool getCanCreateTransaction() const noexcept { return canCreateTransaction; }
    bool getCanDeleteTransaction() const noexcept { return canDeleteTransaction; }
    bool getCanUpdateTransaction() const noexcept { return canUpdateTransaction; }
    bool getCanViewTransaction() const noexcept { return canViewTransaction; }

    bool getCanManageSystemSettings() const noexcept { return canManageSystemSettings; }
    bool getCanManageEmployeePermissions() const noexcept { return canManageEmployeePermissions; }
 


    void setId(int newId) noexcept { id = newId; }
    void setDescription(const std::string& newValue) { Description = newValue; }

    void setCanCreateProduct(bool newValue) noexcept { canCreateProduct = newValue; }
    void setCanDeleteProduct(bool newValue) noexcept { canDeleteProduct = newValue; }
    void setCanUpdateProduct(bool newValue) noexcept { canUpdateProduct = newValue; }
    void setCanViewProduct(bool newValue) noexcept { canViewProduct = newValue; }

    void setCanCreateCategory(bool newValue) noexcept { canCreateCategory = newValue; }
    void setCanDeleteCategory(bool newValue) noexcept { canDeleteCategory = newValue; }
    void setCanUpdateCategory(bool newValue) noexcept { canUpdateCategory = newValue; }
    void setCanViewCategory(bool newValue) noexcept { canViewCategory = newValue; }

    void setCanCreateCustomer(bool newValue) { canCreateCustomer = newValue; }
    void setCanDeleteCustomer(bool newValue) { canDeleteCustomer = newValue; }
    void setCanUpdateCustomer(bool newValue) { canUpdateCustomer = newValue; }
    void setCanViewCustomer(bool newValue) { canViewCustomer = newValue; }

    void setCanCreateEmployee(bool newValue) noexcept { canCreateEmployee = newValue; }
    void setCanDeleteEmployee(bool newValue) noexcept { canDeleteEmployee = newValue; }
    void setCanUpdateEmployee(bool newValue) noexcept { canUpdateEmployee = newValue; }
    void setCanViewEmployee(bool newValue) noexcept { canViewEmployee = newValue; }

    void setCanCreatePurchaseOrder(bool newValue) { canCreatePurchaseOrder = newValue; }
    void setCanDeletePurchaseOrder(bool newValue) { canDeletePurchaseOrder = newValue; }
    void setCanUpdatePurchaseOrder(bool newValue) { canUpdatePurchaseOrder = newValue; }
    void setCanViewPurchaseOrder(bool newValue) { canViewPurchaseOrder = newValue; }

    void setCanCreateRole(bool newValue) { canCreateRole = newValue; }
    void setCanDeleteRole(bool newValue) { canDeleteRole = newValue; }
    void setCanUpdateRole(bool newValue) { canUpdateRole = newValue; }
    void setCanViewRole(bool newValue) { canViewRole = newValue; }

    void setCanCreateSale(bool newValue) { canCreateSale = newValue; }
    void setCanDeleteSale(bool newValue) { canDeleteSale = newValue; }
    void setCanUpdateSale(bool newValue) { canUpdateSale = newValue; }
    void setCanViewSale(bool newValue) { canViewSale = newValue; }

    void setCanCreateSupplier(bool newValue) { canCreateSupplier = newValue; }
    void setCanDeleteSupplier(bool newValue) { canDeleteSupplier = newValue; }
    void setCanUpdateSupplier(bool newValue) { canUpdateSupplier = newValue; }
    void setCanViewSupplier(bool newValue) { canViewSupplier = newValue; }

    void setCanCreateTransaction(bool newValue) { canCreateTransaction = newValue; }
    void setCanDeleteTransaction(bool newValue) { canDeleteTransaction = newValue; }
    void setCanUpdateTransaction(bool newValue) { canUpdateTransaction = newValue; }
    void setCanViewTransaction(bool newValue) { canViewTransaction = newValue; }

    void setCanManageSystemSettings(bool newValue) { canManageSystemSettings = newValue; }
    void setCanManageEmployeePermissions(bool newValue) { canManageEmployeePermissions = newValue; }


    static Permissions readPermissions(int id);
};

#endif 

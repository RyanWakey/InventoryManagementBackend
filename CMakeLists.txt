cmake_minimum_required(VERSION 3.10)
project(InventoryManagementSystem)

# Set the required C++ standard
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Set overall binary dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Specify actual exec subdir
set(EXEC_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/debug)

# Define the Windows version for Boost.Asio if you are using it
add_definitions(-D_WIN32_WINNT=0x0A00)

# Inform CMake to find Crow (which will also pull in Boost if needed)
find_package(crow CONFIG REQUIRED)
find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")

# Include the 'include' directory
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/include")
include_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/include/jdbc")
include_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/include/jbdc/cppconn")
include_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/include/mysql")
include_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/include/mysqlx")
include_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/include/mysqlx/common")
include_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/include/mysqlx/common/devapi")
include_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/include/mysqlx/common/devapi/detail")

include_directories("C:/Users/Ryan/Documents/Projects/Inventory Based Managment Code/InvProject/external/libbcrypt-master/include")



# Linking all the lib files which are located in the following
link_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/lib64")
link_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/lib64/vs14")
link_directories("C:/Program Files/MySQL/mysql-connector-c++-8.0.33-winx64/lib64/vs14/debug")

add_subdirectory(external/libbcrypt-master)
# Define the executable target
add_executable(${PROJECT_NAME} src/main.cpp src/database/DB_Manager.cpp src/models/Category.cpp src/models/Customer.cpp
                               src/models/Employee.cpp src/models/EmployeeRoles.cpp src/models/Permissions.cpp 
                               src/models/PermissionsRoles.cpp src/models/Product.cpp src/models/PurchaseOrder.cpp 
                               src/models/Roles.cpp src/models/Sale.cpp src/models/Supplier.cpp src/models/Transaction.cpp
                               src/models/PurchaseOrderDetails.cpp
                               src/validation/Validator.cpp
                               src/routes/ProductRoutes.cpp src/routes/CategoryRoutes.cpp src/routes/EmployeeRoutes.cpp
                               src/routes/EmployeeRolesRoutes.cpp src/routes/SaleRoutes.cpp src/routes/TransactionRoutes.cpp
                               src/routes/PurchaseOrderRoutes.cpp src/routes/SupplierRoutes.cpp src/routes/PurchaseOrderDetailsRoutes.cpp
                               src/auth/AuthenticationService.cpp src/routes/AuthenticationServiceRoutes.cpp src/routes/CustomerRoutes.cpp
                               )

# Link libraries to the target
target_link_libraries(${PROJECT_NAME} PRIVATE Crow::Crow)
target_link_libraries(${PROJECT_NAME} PRIVATE mysqlcppconn mysqlcppconn8 mysqlcppconn8-static mysqlcppconn-static libcrypto libssl)
target_link_libraries(${PROJECT_NAME} PRIVATE bcrypt)
target_include_directories(${PROJECT_NAME} PRIVATE ${JWT_CPP_INCLUDE_DIRS})
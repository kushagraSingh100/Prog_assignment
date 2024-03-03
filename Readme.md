# Car Rental System

## Overview

This is a simple Car Rental System implemented in C++ using MySQL for database management. The system consists of three main entities: Cars, Customers, and Employees. Additionally, there is a Manager who has administrative privileges.

## Database Setup

Before running the program, make sure to set up the required MySQL databases. Modify the server name, username, and password in the code accordingly (lines 20-22).

```cpp
// Update these lines with your MySQL server details
const string server = "your_server_name";
const string username = "your_username";
const string password = "your_password";

# Entities and Authentication

## Customers
Already existing customers have IDs (1, 2, 3, 4, 5) and passwords respectively (yyfq, yuyv, oppw, jppo, pqop). Name verification is not used; only ID and password are required for user verification.

## Employees
Already existing employees have IDs (1, 2, 3, 4, 5) and passwords respectively (hvea, efde, esqe, tyiq, tqoi). Name verification is not used; only ID and password are required for user verification.

## Manager
The manager has ID 1 and password (oljio). Only the provided password will work.

# Usage

1. Run the program, and it will automatically create new databases for Cars, Customers, and Employees. Existing databases will be deleted.

2. The program initializes with 5 cars, 5 employees, 5 customers, and 1 manager.

3. **For Customers and Employees:**
   - Upon selection, you will have three options:
     - See available cars.
     - See cars currently rented.
     - Rent a car from available cars (based on customer_record for customers).

4. **For the Manager:**
   - If the correct details are provided, you can perform administrative tasks:
     - Add/Update/Delete/Show cars.
     - Add/Update/Delete/Show customers.
     - Add/Update/Delete/Show employees.

# Note

- The program uses MySQL as the database management system.
- Customers and Employees are authenticated based on their IDs and passwords.
- The system provides different functionalities for Customers, Employees, and Managers.
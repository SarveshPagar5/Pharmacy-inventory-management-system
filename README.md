# Pharmacy Inventory Management System

## Overview
The **Pharmacy Inventory Management System** is a C-based program designed to manage and automate pharmacy inventory operations. It provides comprehensive functionality for medication tracking, supplier management, sales monitoring, and inventory control. The system uses linked lists to efficiently manage medication records, supplier information, and reorder details.

## Features

1. **Medication Management**:
   * Add new medications to the inventory database
   * Update existing medication quantities and details
   * Delete medications from the inventory
   * Search medications by name or supplier ID

2. **Expiration Tracking**:
   * Monitor medication expiration dates
   * Generate alerts for medications approaching expiry (within one month)
   * Sort medications by expiration date

3. **Stock Level Monitoring**:
   * Track inventory levels for all medications
   * Set reorder levels for each medication
   * Generate automatic notifications when stock falls below reorder level

4. **Sales Tracking**:
   * Record sales transactions
   * Update inventory quantities automatically
   * Track sales performance by medication

5. **Supplier Management**:
   * Register and update supplier information
   * Track supplier performance metrics (turnover, unique medicines supplied)
   * Identify top-performing suppliers
   * Search and display supplier details

## Data Structures

* **Medicine Details (`med_node`)**: Represents a medication record with attributes like name, batch number, supplier ID, quantity, price, expiry date, medication ID, and total sales.

* **Supplier Information (`supplier_node`)**: Manages supplier records with details like supplier ID, name, contact information, turnover, and unique medicines supplied.

* **Reorder Details (`reorder_node`)**: Tracks reorder information for medications including medicine name, medication ID, reorder level, and total quantity.

* **Unique Medicine (`unique_node`)**: Maintains lists of unique medications supplied by each supplier.

## How It Works

1. **Initialization**:
   * The system initializes empty linked lists for medications, suppliers, and reorder information.

2. **Medication Entry**:
   * When a new medication is added, the system checks if it already exists in the database.
   * If not, it registers the medication and creates or updates associated supplier and reorder information.
   * The system also checks stock levels against reorder thresholds.

3. **Sales Processing**:
   * When a sale occurs, the system updates inventory quantities and sales statistics.
   * It checks stock levels and generates alerts if needed.

4. **Supplier Evaluation**:
   * The system tracks supplier performance based on turnover and variety of medicines supplied.
   * It can sort and identify top-performing suppliers based on different metrics.

5. **Expiration Monitoring**:
   * The system regularly checks expiration dates and alerts for medications nearing expiry.
   * Medications can be sorted by expiry date for better inventory management.

## Compilation and Execution

**On Windows, macOS, and Linux:**

1. **Open a terminal or command prompt**
2. **Navigate to the directory containing `pharmacy_inventory.c`**
3. **Compile and run the code:**

   * **On Windows:**
   ```
   gcc pharmacy_inventory.c -o pharmacy_inventory.exe
   pharmacy_inventory.exe
   ```

   * **On macOS/Linux:**
   ```
   gcc pharmacy_inventory.c -o pharmacy_inventory
   ./pharmacy_inventory
   ```

## System Requirements

* C compiler (GCC recommended)
* Standard C libraries
* Minimum 512MB RAM
* 1MB disk space for program and data storage
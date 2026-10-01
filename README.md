# Car Sales Management System

A command-line car sales management application developed in C as a university programming project. The system manages vehicle stock, customer purchases, discounts, sales records, feedback, and persistent CSV data.

## Features

- View available vehicles and current stock
- Sort vehicles by year in descending order
- Purchase one or more vehicles
- Validate customer names and numeric input
- Apply an age-based discount to eligible customers
- Automatically update vehicle stock after a purchase
- Record customer feedback and ratings
- Calculate revenue for individual sales
- Sort sales by revenue
- Save and load sales records using a CSV file
- Store purchase dates automatically
- Dynamically allocate memory for customer sales records

## Technologies and Concepts

- C
- Structures (`struct`)
- Arrays
- Pointers
- Dynamic memory allocation (`realloc` / `free`)
- File I/O
- CSV persistence
- Input validation
- Sorting algorithms
- Functions and constants
- Date and time handling

## Project Structure

```text
car-sales-management-system/
├── src/
│   └── car_sales.c
├── data/
├── .gitignore
├── LICENSE
└── README.md
```

## Compile and Run

### GCC

From the repository root:

```bash
gcc src/car_sales.c -o car_sales
./car_sales
```

On Windows Command Prompt or PowerShell, run the generated executable with:

```text
car_sales.exe
```

## Data Storage

The application uses `billgrace.csv` for persisted sales records. If the file does not exist on the first run, the program reports that no previous sales data is available. After sales are made, the application writes the sales records to the CSV file.

Because the current source code uses a relative filename, the CSV file is created in the program's working directory when the application runs.

## Main Menu

The application provides options to:

- View available cars
- Buy a car
- View sales
- View customer feedback
- Exit and save sales data

## Notes

This repository presents the original university project in a clean GitHub-friendly structure. The program logic has been retained rather than rewritten for the repository.

## Author

Bill Grace

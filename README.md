# Mini Database Engine 🗄️

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)

</div>

A lightweight, file-based relational database engine implemented in C++ with SQL-like query support. This project provides a simple yet powerful database management system that supports CRUD operations through an interactive SQL command-line interface.

> **Educational Project**: Built as part of the ITI Object-Oriented Programming course to demonstrate database management system concepts, design patterns, and advanced C++ programming techniques.

## 🌟 Features

- **SQL-like Command Interface**: Interactive CLI with familiar SQL syntax
- **Data Types Support**: 
  - Nullable (`NullType`)
  - Integer (`INT`)
  - Double precision floating point (`DOUBLE`)
  - Single character (`CHAR`)
  - Variable-length text (`VARCHAR`, `TEXT`)
  - Date (`DATE`)
- **Core SQL Operations**:
  - `CREATE TABLE` - Define tables with columns and constraints
  - `INSERT INTO` - Add records to tables
  - `SELECT` - Query data with filtering and conditions
  - `UPDATE` - Modify existing records
  - `DELETE` - Remove records from tables
- **Advanced Features**:
  - Primary key constraints (single and composite)
  - WHERE clause filtering with comparison operators (`=`, `!=`, `<`, `>`, `<=`, `>=`)
  - Logical operators (`AND`, `OR`)
  - Persistent file-based storage
  - NULL value support
  - Automatic table metadata management

## 🎯 Project Highlights

### 💡 Technical Excellence

- **Modern C++ Implementation**: Leverages C++17 features including `std::variant`, `std::filesystem`, and lambda expressions for clean, type-safe code
- **Zero External Dependencies**: Completely self-contained using only STL, making it highly portable and easy to deploy
- **Robust Parser Design**: Hand-crafted SQL parser with comprehensive syntax validation and error handling
- **Memory Efficient**: Smart use of move semantics and RAII patterns for optimal resource management
- **Modular Architecture**: Clean separation of concerns with dedicated parsers, models, and utilities

### 🏆 Key Achievements

- **Full SQL Lifecycle**: Complete CRUD operations matching industry-standard SQL syntax
- **Type System**: Rich data type support including custom Date class with proper comparison operators
- **Query Engine**: Sophisticated WHERE clause evaluation with complex boolean logic
- **Persistence Layer**: Reliable file-based storage with automatic serialization/deserialization
- **User Experience**: Interactive CLI with help system and intuitive error messages
- **Data Integrity**: Primary key enforcement and NULL value handling

### 🎓 Educational Value

- Demonstrates **advanced OOP concepts** (encapsulation, abstraction, polymorphism)
- Showcases **design patterns** (Parser pattern, Catalog/Registry pattern)
- Implements **data structures** (variant types, AST)
- Explores **file I/O and serialization** techniques
- Practices **string parsing and tokenization** algorithms
- Applies **software engineering principles** (SOLID, DRY)

## 📋 Prerequisites

- C++ compiler with C++17 support or higher (GCC, Clang, MSVC)
- Standard C++ libraries

## 🚀 Getting Started

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/mini_db_engine.git
   cd mini_db_engine
   ```

2. **Compile the project**

   **Using g++ (Linux/macOS/MinGW)**
   ```bash
   g++ -std=c++17 src/main.cpp -o mini_db
   ```

   **Using clang++**
   ```bash
   clang++ -std=c++17 src/main.cpp -o mini_db
   ```

   **Using MSVC (Windows)**
   ```cmd
   cl /EHsc /std:c++17 src/main.cpp /Fe:mini_db.exe
   ```

   **Using CMake (Recommended)**
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

### Running the Database

**Linux/macOS:**
```bash
./mini_db
```

**Windows:**
```cmd
mini_db.exe
```

You'll be greeted with an interactive SQL prompt:
```
Welcome to mini database engine
Type 'help' for commands
SQL>
```

### Quick Test

Try these commands to get started:
```sql
CREATE TABLE students (id INT PRIMARY KEY, name VARCHAR(50), grade DOUBLE);
INSERT INTO students (id, name, grade) VALUES (1, 'Alice', 95.5);
SELECT * FROM students;
```

## 📖 Usage Examples

### Creating a Table

```sql
CREATE TABLE users (
    id INT PRIMARY KEY,
    name VARCHAR(100),
    email VARCHAR(255),
    age INT,
    created_at DATE
);
```

### Composite Primary Key

```sql
CREATE TABLE enrollments (
    student_id INT,
    course_id INT,
    grade DOUBLE,
    PRIMARY KEY (student_id, course_id)
);
```

### Inserting Data

```sql
INSERT INTO users (id, name, email, age, created_at) 
VALUES (1, 'John Doe', 'john@example.com', 30, '2024-01-15');

INSERT INTO users (id, name, email, age) 
VALUES (2, 'Jane Smith', 'jane@example.com', 28);
```

### Querying Data

```sql
-- Select all records
SELECT * FROM users;

-- Select specific columns
SELECT name, email FROM users;

-- Filtering with WHERE clause
SELECT * FROM users WHERE age > 25;

-- Complex conditions
SELECT name, email FROM users 
WHERE age >= 25 AND age <= 35;

-- Multiple conditions
SELECT * FROM users 
WHERE age > 20 OR name = 'John Doe';
```

### Updating Records

```sql
UPDATE users SET age = 31 WHERE id = 1;

UPDATE users SET email = 'newemail@example.com', age = 29 
WHERE name = 'Jane Smith';
```

### Deleting Records

```sql
DELETE FROM users WHERE id = 2;

DELETE FROM users WHERE age < 18;
```

## 🗂️ Project Structure

```
mini_db_engine/
├── README.md                  # Project documentation
├── LICENSE                    # License file
├── src/
│   ├── main.cpp              # Entry point and CLI loop
│   ├── setup_test_data.cpp   # Test data setup utilities
│   └── README.md             # Source documentation
├── include/
│   ├── models.cpp            # Core data structures (Table, Column, Row, etc.)
│   ├── Helper.cpp            # Utility functions for parsing and file I/O
│   ├── CreateParse.cpp       # CREATE TABLE parser
│   ├── InsertParser.cpp      # INSERT INTO parser
│   ├── SelectParser.cpp      # SELECT query parser
│   ├── UpdateParser.cpp      # UPDATE statement parser
│   ├── DeleteParser.cpp      # DELETE statement parser
│   └── README.md             # Include documentation
└── data/
    └── README.md             # Data directory documentation
```

## 🏗️ Architecture

### Design Principles

- **Separation of Concerns**: Each parser handles one SQL command type
- **Object-Oriented Design**: Leverages classes, inheritance, and polymorphism
- **Type Safety**: Uses C++17 `std::variant` for type-safe value storage
- **File-Based Persistence**: Simple, portable storage without external dependencies

### Core Components

1. **Models** ([models.cpp](include/models.cpp))
   - `Value`: Variant type supporting multiple data types (Int, Double, Char, Text, Date, Null)
   - `Column`: Table column definition with name, type, and constraints
   - `Row`: Table row representation as a vector of values
   - `Table`: Complete table structure with metadata and rows
   - `Catalog`: Database-wide table registry and management
   - `AST`: Abstract syntax tree for parsed queries

2. **Parsers**
   - **CreateParser**: Handles table creation with column definitions and constraints
   - **InsertParser**: Processes INSERT statements with value validation
   - **SelectParser**: Executes SELECT queries with WHERE clause evaluation
   - **UpdateParser**: Modifies existing records based on conditions
   - **DeleteParser**: Removes records matching WHERE criteria
   - Each parser validates syntax, converts queries to AST, and executes operations

3. **Helper Utilities** ([Helper.cpp](include/Helper.cpp))
   - String manipulation (trim, split, case conversion)
   - SQL parsing (parentheses matching, quote handling)
   - File I/O for persistent storage
   - Table serialization/deserialization
   - Query validation and formatting
   - Help text and error message generation

4. **Storage Engine**
   - File-based persistence in `data/` directory
   - Each table stored in a separate text file
   - Automatic loading of existing tables on startup
   - Human-readable format for easy debugging
   - Atomic writes to prevent data corruption

## 🔧 Commands Reference

| Command | Description |
|---------|-------------|
| `help` or `?` | Display available commands and syntax |
| `exit` or `quit` | Exit the database engine |
| `CREATE TABLE ...` | Create a new table |
| `INSERT INTO ...` | Insert data into a table |
| `SELECT ...` | Query data from a table |
| `UPDATE ...` | Update existing records |
| `DELETE FROM ...` | Delete records from a table |

## 📊 Supported Data Types

| Type | Description | Example |
|------|-------------|---------|
| `NullType` | nullable values | NULL |
| `INT` | Integer numbers | `42`, `-100` |
| `DOUBLE` | Floating-point numbers | `3.14`, `-0.5` |
| `CHAR` | Single character | `'A'`, `'x'` |
| `VARCHAR(n)` | Variable-length string (max n chars) | `'Hello'` |
| `TEXT` | Unlimited text | `'Long text...'` |
| `DATE` | Date in YYYY-MM-DD format | `'2024-01-15'` |

## 🔍 WHERE Clause Operators

- **Comparison**: `=`, `!=`, `<`, `>`, `<=`, `>=`
- **Logical**: `AND`, `OR`
- **Value Types**: Numbers, strings (quoted), dates (quoted), NULL

## 🛡️ Constraints

- **Primary Key**: Ensures unique identification of records
  - Single column: `id INT PRIMARY KEY`
  - Composite: `PRIMARY KEY (col1, col2)`

## 💾 Data Persistence

All data is automatically persisted to files in the `data/` directory:
- **Automatic Saving**: Tables are saved after each modification (INSERT, UPDATE, DELETE)
- **Auto-Loading**: Existing tables are loaded automatically on startup
- **File-Per-Table**: Each table is stored in a separate file for isolation
- **Human-Readable Format**: Data files can be inspected and manually edited if needed
- **Metadata Storage**: Column definitions and constraints are stored with the data

### Storage Format Example
```
TableName: users
Columns: id:INT:PK, name:VARCHAR(100), email:VARCHAR(255), age:INT
Rows:
1, "John Doe", "john@example.com", 30
2, "Jane Smith", "jane@example.com", 28
```

## 🤝 Contributing

Contributions are welcome! Here are some areas for improvement:

- Add support for JOIN operations
- Implement indexes for faster queries
- Add more constraints (FOREIGN KEY, UNIQUE, NOT NULL)
- Support for aggregate functions (COUNT, SUM, AVG, etc.)
- Transaction support
- Multi-user concurrency control
- Query optimization

## 📝 License

This project is licensed under the terms specified in the [LICENSE](LICENSE) file.

## 👥 Authors

Developed as part of the **ITI Object-Oriented Programming** course.

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/Ali-Elshorpagi">
        <img src="https://github.com/Ali-Elshorpagi.png" width="100px;" alt="Ali Elshorpagi"/><br />
        <sub><b>Ali Elshorpagi</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/3moJunioR">
        <img src="https://github.com/3moJunioR.png" width="100px;" alt="Mohamed Wahba"/><br />
        <sub><b>Mohamed Wahba</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/Hazem191">
        <img src="https://github.com/Hazem191.png" width="100px;" alt="Hazem Ibrahim"/><br />
        <sub><b>Hazem Ibrahim</b></sub>
      </a>
    </td>
  </tr>
</table>

## 🐛 Known Limitations

As an educational project focused on core database concepts, certain advanced features are intentionally simplified:

- **Single-User Mode**: Designed for learning, not concurrent access
- **Immediate Persistence**: Changes committed instantly (educational approach to understand persistence)
- **Schema Simplicity**: Fixed table structures after creation (demonstrates core CREATE operation)
- **Query Scope**: Focused on fundamental single-table operations for clarity
- **Optimization**: Emphasis on correctness over performance for educational clarity

These design decisions allow the codebase to remain readable and educational while demonstrating fundamental database concepts effectively.

##  Resources

- [SQL Tutorial](https://www.w3schools.com/sql/) - Learn SQL basics
- [Database Design](https://en.wikipedia.org/wiki/Database_design) - Database design principles
- [C++ Reference](https://en.cppreference.com/) - C++ language reference

## 💬 Support

If you encounter any issues or have questions:

1. Check the [Known Limitations](#-known-limitations) section
2. Review the [Usage Examples](#-usage-examples)
3. Open an issue on GitHub with:
   - Your SQL command
   - Expected behavior
   - Actual behavior
   - Error messages (if any)

---

<div align="center">

### **Built with ❤️ and passion for excellence**

*A testament to the power of object-oriented design and modern C++ in building complex systems*

**Perfect for students, educators, and developers learning database internals**

</div>
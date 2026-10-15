# Product Management System (C++ & Qt)

A desktop application developed in **Modern C++** and the **Qt Framework** for managing product inventories and shopping baskets[cite: 2, 6, 10]. The project demonstrates Object-Oriented Programming (OOP) principles, a layered architecture, design patterns, and generic STL algorithms[cite: 1, 4, 10, 13].

## Key Features & Architecture

* **Layered Architecture & Dual UI:** Decoupled Domain (`Product`), Persistence (`AbstractRepo`), Business Logic (`ProductService`), Validation (`ProductValidator`), and Presentation layers, supporting both a console UI (`ProductUi`) and a Qt Graphical User Interface (`ProductGUI`)[cite: 1, 5, 6, 10, 12, 14].
* **Qt Model-View Architecture:** Custom data model (`ProductModel` inheriting from `QAbstractTableModel`) for rendering and updating product lists across `QListView` and `QTableView` components[cite: 2, 6, 7].
* **Observer Design Pattern:** Real-time synchronization between the main application and multiple secondary shopping basket windows (`BasketGUI` for CRUD operations and `BasketReadOnlyGUI` for custom graphical rendering using `QPainter`)[cite: 2, 3, 4].
* **Command Design Pattern (Polymorphic Undo):** Full Undo support for add, delete, and modify operations managed via `std::unique_ptr`[cite: 10, 13].
* **Polymorphic Storage Layer:** Abstract repository interface (`AbstractRepo`) with interchangeable implementations using `std::vector` (`ProductRepo`) and `std::unordered_map` with probabilistic fault injection (`RepoDict`)[cite: 1, 8, 9].
* **Generic STL Algorithms & Data Export:** Template-based sorting (`std::sort`) and filtering (`std::copy_if`) by name, type, price, or manufacturer, category reporting (`std::map`), and shopping basket export to **CSV** and **HTML** formats[cite: 5, 10].
* **Validation, Custom Exceptions & Automated Testing:** Input validation (`ProductValidator`), custom exception classes (`RepoException`, `ValidatorException`), and unit tests (`runTests`)[cite: 8, 11, 14].

## Technologies Used
* **Language:** C++ (STL, Smart Pointers, Templates, Exception Handling)[cite: 8, 10, 14]
* **GUI Framework:** Qt (`QWidget`, `QAbstractTableModel`, `QTableView`, `QListView`, `QPainter`)[cite: 2, 3, 6, 7]

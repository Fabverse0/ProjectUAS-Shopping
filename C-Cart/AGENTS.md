# C-Cart Project Rules

## Project Overview

C-Cart is a desktop e-commerce checkout simulation project built using:

* C++17
* Qt Widgets
* CMake

This project is developed for a Data Structures course final project.

---

## Scope

Only implement the following features:

1. Product Catalog
2. Shopping Cart
3. Checkout
4. Order Queue
5. Confirmation & Delivery Route

Do NOT implement:

* Login
* Register
* Admin Dashboard
* Database
* Payment Gateway
* Priority Queue
* Circular Queue
* Deque
* Voucher System

---

## Required Data Structures

The following data structures must be implemented manually:

* AVL Tree
* Binary Search Tree
* Binary Search
* Single Linked List
* Double Linked List
* Circular Linked List
* Stack (Linked List)
* Queue
* Graph
* BFS
* DFS

Do not replace them with STL containers if the structure itself is the assignment requirement.

---

## Architecture

models/
Contains data models only.

structures/
Contains all data structure implementations.

services/
Contains business logic and communication between UI and structures.

ui/
Contains Qt Widgets UI files only.

---

## Coding Rules

* Use C++17.
* Keep header and implementation separated.
* Use .h and .cpp files.
* Use descriptive class names.
* Add comments for public functions.
* Avoid global variables.
* Do not modify folder structure without permission.

---

## Product Source

Products are hardcoded.

Use approximately 10–15 products.

Product fields:

* id
* nama
* kategori
* harga

---

## Graph Simulation

Cities:

* Jakarta
* Bekasi
* Depok
* Bogor
* Tangerang

Graph is used for delivery route simulation.

BFS:
Find shortest route.

DFS:
Audit all connected cities.

---

## UI Rules

The UI must contain only:

* Catalog Screen
* Cart Screen
* Checkout Screen
* Order Queue Screen
* Route Screen

Do not add extra screens.

---

## Team Responsibilities

Product/Search Module:
AVL Tree, BST, Binary Search.

Cart/Navigation Module:
Linked Lists and Stack.

Checkout/Route Module:
Queue, Graph, BFS, DFS.

UI Module:
Qt Widgets interface and integration.

Always respect module boundaries.

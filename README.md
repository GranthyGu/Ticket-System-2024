# 🚄 Train Ticket Management System

This project is a fully-featured train ticket management system written in C++. It supports ticket booking, refunding, order queries, train schedule management, and user account functionalities. All data is persistently stored on disk, and the system uses custom-built STL-like containers and a generic B+ tree for indexing.

---

## 📁 Project Structure

```text
src/
├── STL/                           # Custom STL-like containers (no use of standard STL)
│   ├── list.hpp
│   ├── map.hpp
│   ├── unordered_map.hpp
│   ├── vector.hpp
│   └── priority_queue.hpp
├── b_plus_tree.hpp                # Generic B+ tree with file storage support
├── LRU.hpp                        # LRU cache for B+ tree node buffering
├── ticket_management.hpp/cpp     # Core module for ticket operations
├── train_management.hpp/cpp      # Train management module
├── account_management.hpp/cpp     # User account system
├── time_calculator.hpp/cpp       # Utilities for time and date processing
├── token_scanner.hpp/cpp         # Command-line token parser
└── main.cpp                       # Program entry point
```
---

## 🎯 Main Features

- **User System:**
  - Register / Login / Logout
  - Query / Modify personal information

- **Train Management:**
  - Add / Delete / Release train schedules
  - Query train schedule and route

- **Ticket System:**
  - Query tickets (supports direct and transfer connections)
  - Buy tickets / Refund tickets / Handle waiting list
  - View personal orders

- **Admin Operations:**
  - Clear all system data
  - Manage other users (requires admin privileges)

---

## ⚙️ Technical Highlights

- **No STL Usage:** All containers like `vector`, `map`, `unordered_map`, `list` are implemented manually.
- **Persistent Storage:** All data is saved to disk files and survives program termination.
- **Generic B+ Tree:** High-performance key-value indexing supporting range queries and disk-based storage.
- **LRU Cache:** Caching layer to reduce disk I/O during B+ tree operations.
- **Command Parser:** `token_scanner` handles user command-line input parsing, ensuring consistent parameter handling.
- **Time Utility:** `time_calculator` supports time arithmetic, formatting, and conversion between strings and internal representations.

---

## 📌 Core Interfaces

The `ticket_management` class includes the following key functions:

| Method | Description |
|--------|-------------|
| `buy_ticket` | Allows a logged-in user to purchase a ticket for a released train. Handles seat availability and waiting list. |
| `query_order` | Returns a list of the user's current and pending orders. |
| `refund_ticket` | Processes ticket refunds and updates the waiting list if necessary. |
| `exit` | Terminates the program. |
| `clear` | Clears all system data (admin/debug use only). |
| `add_train` | Adds a new train schedule with route and seat info. |
| `delete_train` | Deletes a non-released train schedule. |
| `release_train` | Publishes a train, making it available for booking. |
| `query_train` | Displays the full schedule and station info for a train. |
| `query_ticket` | Shows available direct trains between two cities on a specific date. |
| `query_transfer` | Suggests optimal routes that require one transfer between trains. |
| `add_user` | Creates a new user account. Can be used by admin to add users with privileges. |
| `log_in` | Authenticates and logs in a user. |
| `log_out` | Logs out the current user. |
| `query_profile` | Queries user profile info (self or others with sufficient privileges). |
| `modify_profile` | Updates user profile information. |

---

## 🧰 Utility Components

### `token_scanner`

A lightweight parser for tokenizing command-line input. It splits input strings into tokens by whitespace or delimiters, and supports:

- `next_token()` – Retrieve the next token
- `has_more_tokens()` – Check if more tokens are available
- `reset(input)` – Reset the parser with new input

It is used by all major subsystems to standardize how input parameters are parsed.

### `time_calculator`

A utility for handling dates and times, including:

- Time and date arithmetic (e.g., add minutes to a time)
- Parsing strings into date/time objects
- Comparing and formatting times

Used extensively in train scheduling and ticket date validation.

---

## 🧣 Acknowlegements

Monkey_Lee, JaneZ, Istina, TAs.
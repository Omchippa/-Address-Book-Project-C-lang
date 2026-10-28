# -Address-Book-Project-C-lang
# 📒 Address Book in C

A console-based address book application written in C. It lets you create, search, edit, delete, and list contacts, and saves everything to a CSV file so your data persists between runs.

---

## ✨ Features

- **Create** a contact with name, phone, and email (duplicate names are rejected)
- **Search** by name, phone, or email using case-insensitive partial matching
- **Edit** an existing contact (press Enter to keep a field unchanged)
- **Delete** a contact with a confirmation prompt
- **List** all contacts in a formatted table, sorted alphabetically by name
- **Persistent storage** in `contacts.csv`, loaded on startup and saved on exit
- **Sample data** loaded automatically on first run if no saved file exists
- **Windows-friendly**: includes its own case-insensitive helpers, so it builds with MinGW without `strcasecmp` or `strcasestr`

---

## 📁 Project Structure

```
addressbook/
├── main.c         # Menu loop and program entry point
├── contact.c      # Create, search, edit, delete, list, initialize
├── contact.h      # Contact and AddressBook structs, function declarations
├── file.c         # Save/load contacts to and from CSV
├── file.h
├── populate.c     # Dummy contacts for first run
├── populate.h
└── contacts.csv   # Saved contact data (created on first save)
```

---

## 🧱 Data Model

| Field | Max Length |
|---|---|
| Name | 50 characters |
| Phone | 20 characters |
| Email | 50 characters |

The address book holds up to **100 contacts**.

---

## 🚀 Getting Started

### Prerequisites

A C compiler such as GCC (MinGW on Windows).

### Build

```bash
gcc main.c contact.c file.c populate.c -o addressbook
```

### Run

```bash
# Linux / macOS
./addressbook

# Windows
addressbook.exe
```

---

## 🖥️ Usage

```
========================================
          ADDRESS BOOK MENU
========================================
  1. Create contact
  2. Search contact
  3. Edit contact
  4. Delete contact
  5. List all contacts
  6. Save and Exit
========================================
  Enter your choice:
```

> **Note:** Changes are only written to `contacts.csv` when you choose **6. Save and Exit**.

### Example: Listing Contacts

```
  +-----+----------------------+--------------------+------------------------------+
  | No. | Name                 | Phone              | Email                        |
  +-----+----------------------+--------------------+------------------------------+
  | 1   | Alice Smith          | 0987654321         | alice@example.com            |
  | 2   | Bob Johnson          | 1112223333         | bob@company.com              |
  +-----+----------------------+--------------------+------------------------------+
  Total contacts: 2
```

---

## 💾 File Format

Contacts are stored as plain CSV:

```csv
Name,Phone,Email
John Doe,1234567890,john@example.com
Alice Smith,0987654321,alice@example.com
```

---

## ⚠️ Known Limitations

- Commas inside a name, phone, or email will break the CSV format.
- Entering non-numeric text at the main menu is not handled.
- Contacts are only saved on exit, so closing the program any other way loses unsaved changes.

---

## 🔮 Future Improvements

- [ ] Input validation for phone numbers and email addresses
- [ ] Proper CSV quoting to support commas in fields
- [ ] Auto-save after every change
- [ ] Sort by phone or email
- [ ] Dynamic memory allocation to remove the 100-contact limit

---

## 🧠 Concepts Demonstrated

- Structs and arrays of structs
- Pointers and passing structs by reference
- File I/O with `fopen`, `fgets`, `fprintf`
- String handling with `strtok`, `strncpy`, `strcspn`
- Modular programming with header files
- Sorting with an index array and bubble sort

---

## 📄 License

Add your license here (e.g., MIT).

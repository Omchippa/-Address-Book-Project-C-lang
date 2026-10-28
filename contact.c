#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

/* ============================================================
   WINDOWS-COMPATIBLE HELPER FUNCTIONS
   (Windows / MinGW does not have strcasecmp or strcasestr)
   ============================================================ */

/* Case-insensitive string comparison */
static int str_icmp(const char *a, const char *b)
{
    while (*a && *b) {
        char ca = (char)tolower((unsigned char)*a);
        char cb = (char)tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

/* Case-insensitive substring search — returns 1 if needle found in haystack */
static int str_icontains(const char *haystack, const char *needle)
{
    if (!needle || !*needle) return 1;
    size_t nlen = strlen(needle);
    for (; *haystack; haystack++) {
        size_t k = 0;
        while (k < nlen &&
               tolower((unsigned char)haystack[k]) ==
               tolower((unsigned char)needle[k]))
            k++;
        if (k == nlen) return 1;
    }
    return 0;
}

/* Flush the input buffer after scanf */
static void flush_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Read a line from stdin, strip the trailing newline */
static void read_line(char *buf, int size)
{
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\r\n")] = '\0';
}

/* ============================================================
   listContacts  –  show all contacts sorted by name (A-Z)
   ============================================================ */
void listContacts(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0) {
        printf("\n  [!] No contacts in the address book.\n");
        return;
    }

    /* Build a sorted index array using bubble sort */
    int idx[MAX_CONTACTS];
    int n = addressBook->contactCount;
    for (int i = 0; i < n; i++) idx[i] = i;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (str_icmp(addressBook->contacts[idx[j]].name,
                         addressBook->contacts[idx[j + 1]].name) > 0) {
                int tmp      = idx[j];
                idx[j]       = idx[j + 1];
                idx[j + 1]   = tmp;
            }
        }
    }

    printf("\n");
    printf("  +-----+----------------------+--------------------+------------------------------+\n");
    printf("  | %-3s | %-20s | %-18s | %-28s |\n", "No.", "Name", "Phone", "Email");
    printf("  +-----+----------------------+--------------------+------------------------------+\n");

    for (int i = 0; i < n; i++) {
        Contact *c = &addressBook->contacts[idx[i]];
        printf("  | %-3d | %-20s | %-18s | %-28s |\n",
               i + 1, c->name, c->phone, c->email);
    }

    printf("  +-----+----------------------+--------------------+------------------------------+\n");
    printf("  Total contacts: %d\n", n);
}

/* ============================================================
   initialize  –  set up the address book
   ============================================================ */
void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;

    /* Try loading saved contacts first */
    loadContactsFromFile(addressBook);

    /* If no saved contacts exist, load dummy data */
    if (addressBook->contactCount == 0) {
        populateAddressBook(addressBook);
    }
}

/* ============================================================
   createContact  –  add a new contact
   ============================================================ */
void createContact(AddressBook *addressBook)
{
    if (addressBook->contactCount >= MAX_CONTACTS) {
        printf("\n  [!] Address book is full (max %d contacts).\n", MAX_CONTACTS);
        return;
    }

    Contact newContact;
    memset(&newContact, 0, sizeof(Contact));

    printf("\n  --- Add New Contact ---\n");

    flush_input();

    printf("  Enter Name  : ");
    read_line(newContact.name, sizeof(newContact.name));
    if (strlen(newContact.name) == 0) {
        printf("  [!] Name cannot be empty.\n");
        return;
    }

    /* Duplicate name check */
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (str_icmp(addressBook->contacts[i].name, newContact.name) == 0) {
            printf("  [!] A contact named '%s' already exists.\n", newContact.name);
            return;
        }
    }

    printf("  Enter Phone : ");
    read_line(newContact.phone, sizeof(newContact.phone));

    printf("  Enter Email : ");
    read_line(newContact.email, sizeof(newContact.email));

    addressBook->contacts[addressBook->contactCount++] = newContact;
    printf("\n  [OK] Contact '%s' added successfully!\n", newContact.name);
}

/* ============================================================
   searchContact  –  search by name, phone, or email
   ============================================================ */
void searchContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0) {
        printf("\n  [!] No contacts to search.\n");
        return;
    }

    char query[50];
    printf("\n  --- Search Contact ---\n");
    printf("  Enter name / phone / email to search: ");
    flush_input();
    read_line(query, sizeof(query));

    if (strlen(query) == 0) {
        printf("  [!] Search query cannot be empty.\n");
        return;
    }

    int found = 0;
    printf("\n  --- Search Results ---\n");

    for (int i = 0; i < addressBook->contactCount; i++) {
        Contact *ct = &addressBook->contacts[i];
        if (str_icontains(ct->name,  query) ||
            str_icontains(ct->phone, query)  ||
            str_icontains(ct->email, query)) {

            printf("\n  Name  : %s\n", ct->name);
            printf("  Phone : %s\n",  ct->phone);
            printf("  Email : %s\n",  ct->email);
            printf("  ----------------------------------------\n");
            found++;
        }
    }

    if (found == 0)
        printf("  No contacts found matching '%s'.\n", query);
    else
        printf("  Found %d result(s).\n", found);
}

/* ============================================================
   editContact  –  update an existing contact's details
   ============================================================ */
void editContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0) {
        printf("\n  [!] No contacts to edit.\n");
        return;
    }

    char name[50];
    printf("\n  --- Edit Contact ---\n");
    printf("  Enter the name of the contact to edit: ");
    flush_input();
    read_line(name, sizeof(name));

    for (int i = 0; i < addressBook->contactCount; i++) {
        if (str_icmp(addressBook->contacts[i].name, name) == 0) {
            Contact *ct = &addressBook->contacts[i];

            printf("\n  Editing: %s\n", ct->name);
            printf("  (Press ENTER to keep the existing value)\n\n");

            char buf[50];

            printf("  New Name  [%s]: ", ct->name);
            read_line(buf, sizeof(buf));
            if (strlen(buf) > 0)
                strncpy(ct->name, buf, sizeof(ct->name) - 1);

            printf("  New Phone [%s]: ", ct->phone);
            read_line(buf, sizeof(buf));
            if (strlen(buf) > 0)
                strncpy(ct->phone, buf, sizeof(ct->phone) - 1);

            printf("  New Email [%s]: ", ct->email);
            read_line(buf, sizeof(buf));
            if (strlen(buf) > 0)
                strncpy(ct->email, buf, sizeof(ct->email) - 1);

            printf("\n  [OK] Contact updated successfully!\n");
            return;
        }
    }

    printf("  [!] Contact '%s' not found.\n", name);
}

/* ============================================================
   deleteContact  –  remove a contact by name
   ============================================================ */
void deleteContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0) {
        printf("\n  [!] No contacts to delete.\n");
        return;
    }

    char name[50];
    printf("\n  --- Delete Contact ---\n");
    printf("  Enter the name of the contact to delete: ");
    flush_input();
    read_line(name, sizeof(name));

    for (int i = 0; i < addressBook->contactCount; i++) {
        if (str_icmp(addressBook->contacts[i].name, name) == 0) {

            printf("  Are you sure you want to delete '%s'? (y/n): ",
                   addressBook->contacts[i].name);

            char yn[4];
            read_line(yn, sizeof(yn));

            if (yn[0] != 'y' && yn[0] != 'Y') {
                printf("  Deletion cancelled.\n");
                return;
            }

            /* Shift all contacts after index i one position left */
            for (int j = i; j < addressBook->contactCount - 1; j++)
                addressBook->contacts[j] = addressBook->contacts[j + 1];

            addressBook->contactCount--;
            printf("  [OK] Contact '%s' deleted successfully!\n", name);
            return;
        }
    }

    printf("  [!] Contact '%s' not found.\n", name);
}

#include <stdio.h>
#include "contact.h"

int main()
{
    int choice;
    AddressBook addressBook;
    initialize(&addressBook); /* Initialize the address book */

    do
    {
        printf("\n========================================\n");
        printf("          ADDRESS BOOK MENU             \n");
        printf("========================================\n");
        printf("  1. Create contact\n");
        printf("  2. Search contact\n");
        printf("  3. Edit contact\n");
        printf("  4. Delete contact\n");
        printf("  5. List all contacts\n");
        printf("  6. Save and Exit\n");
        printf("========================================\n");
        printf("  Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:
                listContacts(&addressBook);
                break;
            case 6:
                printf("\n  Saving and Exiting...\n");
                saveContactsToFile(&addressBook);
                printf("  Goodbye!\n");
                break;
            default:
                printf("\n  [!] Invalid choice. Please enter 1-6.\n");
        }

    } while (choice != 6);

    return 0;
}

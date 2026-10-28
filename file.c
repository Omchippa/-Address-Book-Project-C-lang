#include <stdio.h>
#include <string.h>
#include "file.h"

#define FILENAME "contacts.csv"

/* ============================================================
   saveContactsToFile  –  write all contacts to contacts.csv
   ============================================================ */
void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("  [!] Error: Could not open '%s' for saving.\n", FILENAME);
        return;
    }

    /* Write CSV header row */
    fprintf(fp, "Name,Phone,Email\n");

    for (int i = 0; i < addressBook->contactCount; i++) {
        Contact *c = &addressBook->contacts[i];
        fprintf(fp, "%s,%s,%s\n", c->name, c->phone, c->email);
    }

    fclose(fp);
    printf("  [OK] %d contact(s) saved to '%s'.\n",
           addressBook->contactCount, FILENAME);
}

/* ============================================================
   loadContactsFromFile  –  read contacts from contacts.csv
   ============================================================ */
void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        /* File does not exist yet — first run, that is fine */
        return;
    }

    char line[200];

    /* Skip the header line (Name,Phone,Email) */
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL &&
           addressBook->contactCount < MAX_CONTACTS)
    {
        /* Strip Windows-style \r\n or Unix \n */
        line[strcspn(line, "\r\n")] = '\0';

        /* Skip blank lines */
        if (strlen(line) == 0) continue;

        Contact c;
        memset(&c, 0, sizeof(Contact));

        char *token;

        token = strtok(line, ",");
        if (!token) continue;
        strncpy(c.name, token, sizeof(c.name) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(c.phone, token, sizeof(c.phone) - 1);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(c.email, token, sizeof(c.email) - 1);

        addressBook->contacts[addressBook->contactCount++] = c;
    }

    fclose(fp);
}

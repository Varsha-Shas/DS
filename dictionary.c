#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
struct Dictionary
{
    char word[100];
    char meaning[250];
} dictionary[1000];// Declare dictionary as an array of structs
int countwords = 0;
void files();
void searchword();
void insertword();
void deleteword();
int main() {
    int ch;
    char word[100], meaning[250];
    files();
    printf("Simple Dictionary\n");

    while (1) {
        printf("\nMenu:\n");
        printf("1. Search Word\n");
        printf("2. Insert Word\n");
        printf("3. Delete Word\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        //To verify user is giving option in intergers
        while((scanf("%d",&ch)!=1))
        {
            while(getchar()!='\n');
            printf("Error in option ,Please type respective option.\n");
        }

        switch (ch) {
            case 1:
                searchword();
                break;
            case 2:
                insertword();
                break;
            case 3:
                deleteword();
                break;
            case 4:
                printf("exiting the program\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}

void files()
{
    FILE *fp;
    int i = 0;
    countwords=0;
    fp = fopen("dictionary.csv", "r");
    if (fp == NULL)
    {
        printf("Error opening file dictionary\n");
        return;
    }
    char line1[350]; // Increase buffer size to avoid potential overflows
    while (fgets(line1, sizeof(line1), fp)) {
        // Remove trailing newline if present
        if (line1[strlen(line1) - 1] == '\n') {
            line1[strlen(line1) - 1] = '\0';
        }
        if (sscanf(line1, "%[^,],%[^,]", dictionary[i].word, dictionary[i].meaning) == 2) {
            i++;
            countwords++;
        } else {
            printf("Error parsing line: %s\n", line1);
        }

        if (i >= 1000) {
            printf("Dictionary full.\n");
            break;
        }
    }

    fclose(fp);
}
void searchword()
{
    if(countwords==0)
    {
        printf("No words to search\n");
        return;
    }
    char word[100];
    do
    {
        search:
        printf("Type the word to search\n");
        scanf(" %s",word);
        for (int i = 0; word[i] != '\0'; i++)
        {
            if (isalpha(word[i])==0)
                {
                     printf("Invalid input: Contains non-alphabetic characters.\n");
                     goto search;
                 }
        }
        break;
    }while(1);
    printf("Searching for word %s...\n", word);
    for (int i = 0; i < countwords; i++) {
        if (strcasecmp(word, dictionary[i].word) == 0) {
            printf("Word found!\n");
            printf("Word: %s\nMeaning: %s\n", dictionary[i].word, dictionary[i].meaning);
            return;
        }
    }
    printf("Word not found.\n");
}
//to insert a new word to file
void insertword()
{
    char word[100], meaning[250];
    FILE *fp;
    if (countwords >= 1000)
    {
        printf("Dictionary is full. Cannot insert more words.\n");
        return;
    }
    fp = fopen("dictionary.csv", "a"); // Open in append mode
    if (fp == NULL)
    {
        printf("Error: Could not open file dictionary.\n");
        return;
    }
    do
    {
        insert:
        printf("Type the word to insert\n");
        scanf(" %s",word);
        for (int i = 0; word[i] != '\0'; i++)
        {
            if (isalpha(word[i])==0)
                {
                     printf("Invalid input: Contains non-alphabetic characters.\n");
                     goto insert;
                 }
        }
        for (int i = 0; i < countwords; i++)
        {
            if (strcasecmp(word, dictionary[i].word) == 0)
            {
                printf("word already exists. Please type a different word.\n");
                goto insert;
            }
        }
        break;
    }while(1);
    printf("Type the meaning.\n");
    scanf(" %[^\n]",meaning);
    fprintf(fp, "%s,%s\n", word, meaning);
    printf("Word '%s' has been added to the dictionary.\n", word);
    strcpy(dictionary[countwords].word, word); // Add the new word to the in-memory dictionary
    strcpy(dictionary[countwords].meaning, meaning);
    countwords++;
    fclose(fp);
}
//to delete a word from the file
void deleteword()
{
    if(countwords==0)
    {
        printf("No words to delete.\n");
        return;
    }
    int i=0;
    char word[100];
    FILE *fp = fopen("dictionary.csv", "r");
    if (fp==NULL)
    {
        printf("Error: Could not open file dictionary.csv.\n");
        return;
    }
    FILE *fp1 = fopen("temp.csv", "w");
        if (fp1==NULL)
    {
        printf("Error: Could not create temporary file.\n");
        fclose(fp);
        return;
    }
    do
    {
        delete:
        printf("Type the word to delete\n");
        scanf(" %s",word);
        for (int i = 0; word[i] != '\0'; i++)
        {
            if (isalpha(word[i])==0)
                {
                     printf("Invalid input: Contains non-alphabetic characters.\n");
                     goto delete;
                 }
        }
        break;
    }while(1);
    int found = 0;
   for (i = 0; i < countwords; i++)
   {
        if (fscanf(fp, "%[^,],%[^,\n]\n", dictionary[i].word, dictionary[i].meaning) != 2)
        {
            break; // Stop reading if format mismatch
        }
        if (strcasecmp(dictionary[i].word, word) == 0)
        {
            printf("The word '%s' is deleted from the dictionary.\n", word);
            found = 1;
        } else
        {
            fprintf(fp1, "%s,%s\n", dictionary[i].word, dictionary[i].meaning);
        }
    }
    fclose(fp);
    fclose(fp1);
    if (found==1)
    {
     remove("dictionary.csv");
     rename("temp.csv", "dictionary.csv");
     countwords--;
    }
    else
    {
        printf("The word '%s' was not found in the dictionary.\n", word);
        remove("temp.csv");
    }
    files();
}


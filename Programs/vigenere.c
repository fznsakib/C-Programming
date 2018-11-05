#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    //Only accept one command-line argument
    if (argc != 2)
    {
        printf("You must enter one string only\n");
        return 1;
    }

    //Only accept alphabetical characters
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("String must contain alphabetical chararcters only\n");
            return 1;
        }
    }

    //create array with length of keyword
    int key[strlen(argv[1])];

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isupper(argv[1][i]) != 0)
        {
            key[i] = 25 - (90 - argv[1][i]);
        }
        else
        {
            key[i] = 25 - (122 - argv[1][i]);
        }
    }

    // Input word to be encrypted
    char message[100];
    scanf("%s", message);
    int length = strlen(message);
    int arrayCounter = 0;

    for (int i = 0; i < length; i++)
    {
        if (arrayCounter == (strlen(argv[1])) )
        {
            arrayCounter = 0;
        }
        // Translate ASCII using key
        if (isalpha(message[i]) != 0)
        {
            if (isupper(message[i]) != 0)
            {
                for (int j = 0; j < key[arrayCounter]; j++)
                {
                    // If letter is 'Z' then return to 'A'
                    if (message[i] == 90)
                    {
                        message[i]= 65;
                    }
                    // Else increment ASCII value to next letter
                    else
                    {
                        message[i]++;
                    }
                }
                printf("%c", message[i]);
            }
            else
            {
                for (int j = 0; j < key[arrayCounter]; j++)
                {
                    if (message[i] == 122)
                    {
                        message[i] = 97;
                    }
                    else
                    {
                        message[i]++;
                    }
                }
                printf("%c", message[i]);
            }
            arrayCounter++;
        }
        else
        {
            printf("%c", message[i]);
        }
    }
    printf("\n");
    return 0;
}

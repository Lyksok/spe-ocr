#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024

// Function to check lines based on the criteria (from the previous example)
void check_lines(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int previous_empty_line = -1; // Stores the line number of the last empty line

    printf("\nChecking file: %s\n", file_path);

    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        line_number++;
        int length = strlen(line);

        // Check for lines with more than 80 columns
        if (length > 80)
        {
            printf("Line %d has more than 80 columns.\n", line_number);
        }

        // Check for trailing spaces
        if (length > 1 && (line[length - 1] == '\n' || line[length - 1] == '\r'))
        {
            length--; // Ignore the newline character
        }
        if (length > 0 && line[length - 1] == ' ')
        {
            printf("Line %d has a trailing space.\n", line_number);
        }

        // Check for consecutive empty lines
        if (length == 0 || (length == 1 && line[0] == '\n') || (length == 2 && line[0] == '\r' && line[1] == '\n'))
        {
            if (previous_empty_line == line_number - 1)
            {
                printf("Line %d has two consecutive empty lines.\n", line_number - 1);
            }
            previous_empty_line = line_number;
        }
        else
        {
            previous_empty_line = -1;
        }
    }

    fclose(file);
}

// Recursive function to process directories and apply check_lines to each .c file
void process_directory(const char *path)
{
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        char full_path[1024];
        struct stat entry_info;

        // Ignore "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Skip the specified exclude directory
        if (strcmp(entry->d_name, "coding-style") == 0)
        {
            continue;
        }

        // Construct the full path to the entry
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Get information about the entry
        if (stat(full_path, &entry_info) == -1)
        {
            perror("stat");
            continue;
        }

        // If the entry is a directory, recursively process it
        if (S_ISDIR(entry_info.st_mode))
        {
            process_directory(full_path);
        }
        // If the entry is a file and ends with ".c", apply check_lines to it
        else if (S_ISREG(entry_info.st_mode) && strstr(entry->d_name, ".c") != NULL)
        {
            check_lines(full_path);
        }
    }

    closedir(dp);
}

int main()
{
    // Start processing from the provided directory
    process_directory("../");

    return 0;
}


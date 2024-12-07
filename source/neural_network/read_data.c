#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convertion.h"
#include "neural_structures.h"
#include "setup.h"

TrainingData *ReadDirectory(const char *directory)
{
	DIR *dir = opendir(directory);
	if (dir == NULL)
	{
		perror("opendir");
		return NULL;
	}
	struct dirent *entry;
	size_t p_len = strlen(directory);
	TrainingData *first = NULL;
	TrainingData *cur = NULL;

	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type != DT_DIR
		&& strcmp(entry->d_name, ".")
		&& strcmp(entry->d_name, ".."))
		{
			size_t len = strlen(entry->d_name);
//			printf("png is %s\n", entry->d_name);
			//char name[len+10];
			//strcpy(name, entry->d_name);
			if ((len > 4)
			&& (entry->d_name[len-4] == '.') && (entry->d_name[len-3] == 'p')
			&& (entry->d_name[len-2] == 'n') && (entry->d_name[len-1] == 'g'))
//			&& (name[len-4] == '.') && (name[len-3] == 'p')
//			&& (name[len-2] == 'n') && (name[len-1] == 'g'))
			{
				// process image
				char path[len + p_len + 10];
				snprintf(path, len + p_len + 10,
				"%s/%s", directory, entry->d_name);
//				printf("path is %s\n", path);
				SDL_Surface *surface = toSDL(path);
				if (surface != NULL)
				{
					// if surface == NULL => error
					TrainingData *data = CreateData();
					char letter = entry->d_name[0];
					if (letter >= 'a' && letter <= 'z')
						letter -= 32;
//					printf("letter is %c, had %c\n", letter, entry->d_name[0]);
					data->expected = letter;
					SDL_to_list(surface, nInputs, &(data->inputs));
					cur = LinkData(cur, data);
					if (first == NULL)
					{
						first = data;
					}
//					printf("letter is %c\n", data.expected);
					cur = cur->next;
//					printf("finished processing this data\n");
				}
			}
		}
	}
	if (closedir(dir) == -1)
	{
		perror("closedir");
	}

	return first;
}

TrainingData *ParseDirectory()
{
	TrainingData *first = NULL;
	TrainingData *prev = first;
	char *base = "data/dataset/training";
	char directory[35];
	for (char i = 0; i < 26; i++)
	{
//		printf("----- PROCESSING LETTER %c -----\n", (char)i+'A');
		snprintf(directory, 25, "%s/%c", base, ('a' + i));
		TrainingData *one = ReadDirectory(directory);
		prev = LinkData(prev, one);
//		if (prev == NULL) printf("WTH\n");
		if (first == NULL)
		{
			first = one;
		}
//		printf("--- CUR ITERATION ---\n");
		for (; prev->next != NULL; prev = prev->next)
		{
			if (prev->next == NULL) printf("WTH\n");
//			prev->expected = i + 'A';
//			printf("here : expected is %c\n", prev->expected);
		}
		snprintf(directory, 25, "%s/%c", base, ('A' + i));
		TrainingData *two = ReadDirectory(directory);
		prev = LinkData(prev, two);
//		printf("--- CUR ITERATION ---\n");
		for (; prev->next != NULL; prev = prev->next)
		{}
//		printf("----- END PROCESS LETTER %c -----\n", (char)i+'A');
	}
//	printf("--- RETURN ---\n");
	return first;
}

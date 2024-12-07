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
		return;
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
			char name[len] = entry->d_name;
			if ((len > 4)
			&& (name[len-4] == '.') && (name[len-3] == 'p')
			&& (name[len-2] == 'n') && (name[len-1] == 'g'))
			{
				// process image
				char path[len + p_len];
				snprintf(path, len + p_len,
				"%s/%s", directory, name);
				SDL_Surface *surface = toSDL(path);
				// if surface == NULL => error
				TrainingData data = CreateData();
				if (first == NULL)
				{
					first = &data;
					cur = first;
				}
				else
				{
					LinkData(cur, &data);
				}
				SDL_to_list(surface, nInputs, cur->inputs);
				cur = cur->next;
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
	TrainingData *prev = first:
	char *base = "data/dataset/training";
	char directory[25];
	for (char i = 0; i < 26; i++)
	{
		snprintf(directory, 25, "%s/%c", base, ('a' + i));
		TrainingData *cur = ReadDirectory(directory);
		LinkData(prev, cur);
		if (first == NULL)
			first = cur;
		for (prev = cur; prev->next != NULL; prev = prev->next)
		{}
		snprintf(directory, 25, "%s/%c", base, ('A' + i));
		cur = ReadDirectory(directory);
		LinkData(prev, cur);
		for (prev = cur; prev->next != NULL; prev = prev->next)
		{}
	}
	return first;
}

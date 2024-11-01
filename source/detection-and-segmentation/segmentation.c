#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "structures.h"
#include "detection.h"

/* Cuts the surface into a smaller surface representing
 * a bounding box
*/
SDL_Surface *Cut(SDL_Surface *surface, BoundingBox *box)
{
	int x_up = box -> p1.x;
	int y_up = box -> p1.y;
	int x_down = box -> p2.x;
	int y_down = box -> p2.y;
	int w = x_down - x_up;
	int h = y_down - y_up;

	SDL_Rect area;
	area.x = x_up;
	area.y = y_up;
	area.w = w;
	area.h = h;

	SDL_Surface* new = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	int blit = SDL_BlitSurface(surface, &area, new, NULL);
	if (blit)
	{
		printf("Failure to blit the surface\n");
	}

	return new;
}

/* Saves a bounding box as an image and renames it using average coordinates
*/
void save_bounding_box(SDL_Surface* surface, BoundingBox* box)
{
	surface = Cut(surface, box);
	Point p = get_bounding_box_center(box);
	char buffer[128];
	char* dir_path = "./source/detection-and-segmentation/segmentation-results/";
	sprintf(buffer, "%s%i-%i.png",dir_path , p.x, p.y);
	IMG_SavePNG(surface, buffer);
	printf("Save bounding box: %s\n", buffer);
}

/* Saves all bounding boxes as images 
 * and renames them using average coordinates
*/
void save_bounding_boxes(SDL_Surface* surface, BoundingBox** boxes, int len)
{
	for(int i=0; i<len; i++)
	{
		save_bounding_box(surface, boxes[i]);
	}
}

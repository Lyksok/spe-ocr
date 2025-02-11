#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>

#include "../image_processing/detection/structures.h"
#include "detection.h"

/* Cuts the surface into a smaller surface representing
 * a bounding box
 */
SDL_Surface *cut(SDL_Surface *surface, BoundingBox *box) {
  int x_up = box->p1.x;
    int y_up = box->p1.y;
    int x_down = box->p2.x;
    int y_down = box->p2.y;
    int w = x_down - x_up;
    int h = y_down - y_up;

    SDL_Rect area;
    area.x = x_up;
    area.y = y_up;
    area.w = w;
    area.h = h;

    SDL_Surface *newSurface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    if (!newSurface) {
        printf("Failed to create cropped surface: %s\n", SDL_GetError());
        return NULL;
    }

    if (SDL_BlitSurface(surface, &area, newSurface, NULL) != 0) {
        printf("Failed to blit surface: %s\n", SDL_GetError());
        SDL_FreeSurface(newSurface);
        return NULL;
    }

    SDL_Surface *paddedSurface = SDL_CreateRGBSurface(0, w + 10, h + 10, 32, 0, 0, 0, 0);
    if (!paddedSurface) {
        printf("Failed to create padded surface: %s\n", SDL_GetError());
        SDL_FreeSurface(newSurface);
        return NULL;
    }
    Uint32 black = SDL_MapRGB(paddedSurface->format, 0, 0, 0);
    SDL_FillRect(paddedSurface, NULL, black);

    SDL_Rect destRect;
    destRect.x = 5;
    destRect.y = 5;
    destRect.w = w;
    destRect.h = h;

    if (SDL_BlitSurface(newSurface, NULL, paddedSurface, &destRect) != 0) {
        printf("Failed to blit cropped surface onto padded surface: %s\n", SDL_GetError());
        SDL_FreeSurface(newSurface);
        SDL_FreeSurface(paddedSurface);
        return NULL;
    }

    SDL_FreeSurface(newSurface);
    return paddedSurface;
}

/* Saves a bounding box as an image and renames it using average coordinates
 */
void save_bounding_box(SDL_Surface *surface, BoundingBox *box) {
  surface = cut(surface, box);
  Point p = get_bounding_box_center(box);
  char buffer[128];
  char *dir_path = "./bin/";
  sprintf(buffer, "%s%i-%i.png", dir_path, p.x, p.y);
  IMG_SavePNG(surface, buffer);
  printf("Save bounding box: %s\n", buffer);
  SDL_FreeSurface(surface);
}

/* Saves all bounding boxes as images
 * and renames them using average coordinates
 */
void save_bounding_boxes(SDL_Surface *surface, BoundingBox **boxes, int len) {
  for (int i = 0; i < len; i++) {
    save_bounding_box(surface, boxes[i]);
  }
}

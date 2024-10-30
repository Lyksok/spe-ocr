#ifndef CONVERTING_H
#define CONVERTING_H

#include <SDL2/SDL.h>

void image_to_grayscale(SDL_Surface* surface);
void image_to_binarized(SDL_Surface* surface, size_t threshold);
void convert_image(SDL_Surface* surface, 
	Uint32 (*func)(SDL_PixelFormat*, Uint32));
void convert_image_with_threshold(SDL_Surface* surface, size_t threshold, 
	Uint32 (*func)(SDL_PixelFormat*, Uint32, size_t));
void convert_pixel_with_threshold(SDL_Surface* surface, int threshold, 
	size_t i);

#endif /* CONVERTING_H */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bounding-box.h"
#include "cluster-detection.h"
#include "detection.h"
#include "histograms.h"
#include "structures.h"

void draw_rect(SDL_Renderer *renderer, int rect[4]) {
  if (SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE))
    errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
  SDL_Rect r;
  r.x = rect[0];
  r.y = rect[1];
  r.w = rect[2];
  r.h = rect[3];
  if (SDL_RenderDrawRect(renderer, &r))
    errx(EXIT_FAILURE, "Failed to fill the rectangle: %s", SDL_GetError());
}

void draw_rects(SDL_Renderer *renderer, int rect[][4], size_t len) {
  for (size_t i = 0; i < len; i++) {
    draw_rect(renderer, rect[i]);
  }
}

void draw_line(SDL_Renderer *renderer, Point p1, Point p2) {
  if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE))
    errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
  if (SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y))
    errx(EXIT_FAILURE, "Failed to draw line: %s", SDL_GetError());
}

void draw_lines(SDL_Renderer *renderer, Point *src, Point *dest, int len) {
  for (int i = 0; i < len; i++) {
    draw_line(renderer, src[i], dest[i]);
  }
}

Point get_bounding_box_center(BoundingBox *b) {
  int x = (b->p1.x + b->p2.x) / 2;
  int y = (b->p1.y + b->p2.y) / 2;
  Point res = {x, y};
  return res;
}

Point get_points_average(Point p1, Point p2) {
  return (Point){(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
}

int ascending_bounding_box(BoundingBox *b1, BoundingBox *b2) {
  // Define the tolerance for the grid disparity
  int tolerance = 5;
  Point p1 = get_bounding_box_center(b1);
  Point p2 = get_bounding_box_center(b2);

  if (abs(p1.y - p2.y) <= tolerance) {
    if (abs(p1.x - p2.x) <= tolerance) {
      return 1;
    }
    if (p1.x <= p2.x)
      return 1;
    else
      return 0;
  }
  if (p1.y <= p2.y)
    return 1;
  else
    return 0;
}

/* Sort bounding boxes by ascending Y then X
 */
void sort_bounding_boxes(BoundingBox ***boxes, int len,
                         int (*asc)(BoundingBox *, BoundingBox *)) {
  int swapped = 0;
  for (int i = 0; i < len - 1; i++) {
    swapped = 0;
    for (int j = 0; j < len - i - 1; j++) {
      BoundingBox *b1 = (*boxes)[j];
      BoundingBox *b2 = (*boxes)[j + 1];
      if (!asc(b1, b2)) {
        BoundingBox *tmp = (*boxes)[j];
        (*boxes)[j] = (*boxes)[j + 1];
        (*boxes)[j + 1] = tmp;
        swapped = 1;
      }
    }
    if (!swapped)
      return;
  }
}

double dist2(Point p1, Point p2) {
  return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
}

double dist(Point p1, Point p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

int is_already_closest_of(Point src_box, Point dest_box, Point *src,
                          Point *dest, int l) {
  for (int i = 0; i < l; i++) {
    if ((are_equal_point(src_box, src[i]) &&
         are_equal_point(dest_box, dest[i])) ||
        (are_equal_point(dest_box, src[i]) &&
         are_equal_point(src_box, dest[i]))) {
      return 1;
    } else
      return 0;
  }
  return 0;
}

BoundingBox *closest_bounding_boxes(BoundingBox *box, BoundingBox **boxes,
                                    int len) {
  Point p = get_bounding_box_center(box);
  // Get the first distance
  int i = 0;
  while (i < len && are_equal_box(*box, *(boxes[i]))) {
    i++;
  }
  Point q = get_bounding_box_center(boxes[i]);
  BoundingBox *closest = boxes[i];
  double min_dist = dist2(p, q);

  // Get the minimal distance
  for (int j = i + 1; j < len; j++) {
    if (!are_equal_box(*box, *(boxes[j]))) {
      q = get_bounding_box_center(boxes[j]);
      double dist = dist2(p, q);
      if (dist < min_dist) {
        min_dist = dist;
        closest = boxes[j];
      }
    }
  }
  return closest;
}

void get_all_links(BoundingBox **boxes, int len, Point **src, Point **dest) {
  for (int i = 0; i < len; i++) {
    BoundingBox *closest = closest_bounding_boxes(boxes[i], boxes, len);
    (*src)[i] = get_bounding_box_center(boxes[i]);
    (*dest)[i] = get_bounding_box_center(closest);
  }
}

Point get_closest_point(Point point, Point *points, int len) {
  Point closest = points[0];
  double min_dist = dist2(point, closest);
  for (int i = 1; i < len; i++) {
    double dist = dist2(point, points[i]);
    if (dist < min_dist) {
      min_dist = dist;
      closest = points[i];
    }
  }
  return closest;
}

BoundingBox **get_char_boxes(SDL_Surface *surface, int *res_len) {
  int len = surface->w * surface->h;
  BoundingBox **char_boxes = malloc(len * sizeof(BoundingBox *));
  for (int i = 0; i < len; i++) {
    char_boxes[i] = NULL;
  }
  get_bounding_boxes(surface, &char_boxes);
  BoundingBox **res = get_minimal(char_boxes, len, res_len);
  free(char_boxes);
  return res;
}

void get_corners(Point *points, int len, Point **corners) {
  Point top_left = points[0];
  Point bottom_right = points[0];

  for (int i = 1; i < len; i++) {
    // TOP LEFT
    if (points[i].x < top_left.x || points[i].y < top_left.y)
      top_left = points[i];

    // BOTTOM RIGHT
    if (points[i].x > bottom_right.x || points[i].y > bottom_right.y)
      bottom_right = points[i];
  }

  (*corners)[0] = top_left;
  (*corners)[1] = bottom_right;
}

Point compute_mean_of_points(Point *points, int len) {
  int m_x = 0;
  int m_y = 0;
  int count = 0;
  for (int i = 0; i < len; i++) {
    m_x += points[i].x;
    m_y += points[i].y;
    count++;
  }
  return (Point){(int)((double)m_x / (double)count),
                 (int)((double)m_y / (double)count)};
}

void k_means(SDL_Surface *surface, Point **res_centroids) {
  int len;
  BoundingBox **boxes = get_char_boxes(surface, &len);
  Point *points = bounding_box_to_points(boxes, len);
  Point *centroids = calloc(2, sizeof(Point));
  get_corners(points, len, &centroids);

  Point centroid_points[2][len];
  int centroid_p_index[2];
  int nb_points[2];

  int changes = 1;
  while (changes != 0) {
    // Reset k-means variables
    changes = 0;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < len; j++) {
        centroid_points[i][j] = (Point){-1, -1};
      }
      centroid_p_index[i] = 0;
      nb_points[i] = 0;
    }

    // Iterate through all points
    for (int i = 0; i < len; i++) {
      Point point = points[i];

      // Get closest centroid
      int closest_centroid = 0;
      double min_dist = dist(point, centroids[closest_centroid]);
      for (int j = 1; j < 2; j++) {
        double n_dist = dist(point, centroids[j]);
        if (n_dist < min_dist) {
          min_dist = n_dist;
          closest_centroid = j;
        }
      }

      // Update variables for mean calculation
      centroid_points[closest_centroid][centroid_p_index[closest_centroid]++] =
          point;
      nb_points[closest_centroid] += 1;
    }

    for (int i = 0; i < 2; i++) {
      Point old_centroid = centroids[i];
      Point new_centroid =
          compute_mean_of_points(centroid_points[i], centroid_p_index[i]);
      if (abs(old_centroid.x - new_centroid.x) > 2)
        changes = 1;
      centroids[i] = new_centroid;
    }
  }

  printf("Number of points per centroid\n");
  for (int i = 0; i < 2; i++) {
    printf("Centroid 1: %i\n", centroid_p_index[i]);
  }
  (*res_centroids)[0] = centroids[0];
  (*res_centroids)[1] = centroids[1];

  free(centroids);
  free_boxes(boxes, len);
  free(points);
}

BoundingBox *histo_grid_detect(SDL_Surface *surface) {
  int len;
  BoundingBox **boxes = get_char_boxes(surface, &len);
  int *col_histo = get_col_box_histogram(surface, boxes, len);
  int *row_histo = get_row_box_histogram(surface, boxes, len);

  int col = get_most_frequent(col_histo, surface->w);
  int row = get_most_frequent(row_histo, surface->h);

  sort_bounding_boxes(&boxes, len, ascending_bounding_box);

  BoundingBox *grid[col][row];
  for (int i = 0; i < col; i++) {
    for (int j = 0; j < row; j++) {
      grid[i][j] = NULL;
    }
  }
  int i_c = 0;
  int j_c = 0;
  int count = 0;
  // For each bound box, check if center point is in correct bound
  for (int i = 0; i < len; i++) {
    BoundingBox *box = boxes[i];
    Point center = get_bounding_box_center(box);

    // Check columns
    if (col_histo[center.x] >= col && row_histo[center.y] >= row) {
      if (i_c == col)
        return NULL;
      grid[i_c][j_c] = box;

      j_c++;
      if (j_c == row) {
        j_c = 0;
        i_c++;
      }
      count++;
    }
  }

  free(col_histo);
  free(row_histo);

  if (grid[0][0] == NULL || grid[col - 1][row - 1] == NULL)
    return NULL;
  Point top_left = grid[0][0]->p1;
  Point bottom_right = grid[col - 1][row - 1]->p2;
  BoundingBox *res = calloc(1, sizeof(BoundingBox));
  res->p1 = top_left;
  res->p2 = bottom_right;

  free_boxes(boxes, len);

  return res;
}

int get_max_x(BoundingBox **boxes, int len) {
  int max_x = boxes[0]->p2.x;
  for (int i = 1; i < len; i++) {
    int x = boxes[i]->p2.x;
    if (x > max_x)
      max_x = x;
  }
  return max_x;
}

int get_max_y(BoundingBox **boxes, int len) {
  int max_y = boxes[0]->p2.y;
  for (int i = 1; i < len; i++) {
    int y = boxes[i]->p2.y;
    if (y > max_y)
      max_y = y;
  }
  return max_y;
}

int get_min_x(BoundingBox **boxes, int len) {
  int min_x = boxes[0]->p1.x;
  for (int i = 1; i < len; i++) {
    int x = boxes[i]->p1.x;
    if (x < min_x)
      min_x = x;
  }
  return min_x;
}

int get_min_y(BoundingBox **boxes, int len) {
  int min_y = boxes[0]->p1.y;
  for (int i = 1; i < len; i++) {
    int y = boxes[i]->p1.y;
    if (y < min_y)
      min_y = y;
  }
  return min_y;
}

BoundingBox *histo_word_list_detect(SDL_Surface *surface) {
  int len;
  BoundingBox **boxes = get_char_boxes(surface, &len);

  BoundingBox *grid_box = histo_grid_detect(surface);
  if (grid_box == NULL)
    return NULL;

  int *col_histo = get_col_box_histogram(surface, boxes, len);
  int *row_histo = get_row_box_histogram(surface, boxes, len);
  int new_len = len - get_most_frequent(col_histo, surface->w) *
                          get_most_frequent(row_histo, surface->h);
  free(col_histo);
  free(row_histo);
  BoundingBox **word_list_boxes = malloc(new_len * sizeof(BoundingBox *));
  for (int i = 0; i < new_len; i++) {
    word_list_boxes[i] = NULL;
  }
  int c = 0;
  for (int i = 0; i < len; i++) {
    BoundingBox *box = boxes[i];
    if (!is_in_boxes(&grid_box, 1, get_bounding_box_center(box))) {
      word_list_boxes[c] = box;
      c++;
    }
  }
  free(grid_box);
  BoundingBox *res = malloc(1 * sizeof(BoundingBox));
  Point p1 = {get_min_x(word_list_boxes, new_len),
              get_min_y(word_list_boxes, new_len)};
  Point p2 = {get_max_x(word_list_boxes, new_len),
              get_max_y(word_list_boxes, new_len)};
  *res = (BoundingBox){p1, p2};
  free(word_list_boxes);
  free_boxes(boxes, len);
  return res;
}

int count_except_null(int *histogram, int len) {
  int count = 0;
  int i = 0;
  // Remove trailing 0
  while (i < len && histogram[i] == 0)
    i++;

  // Count when 0 appear
  while (i < len) {
    while (i < len && histogram[i] != 0)
      i++;

    count++;
    while (i < len && histogram[i] == 0)
      i++;
  }

  return count;
}

int *get_words_length(int *histogram, int len, int nb_words,
                      int **words_height) {
  int *res = calloc(nb_words, sizeof(int));
  int c = 0;
  int i = 0;
  // Remove trailing 0
  while (i < len && histogram[i] == 0)
    i++;

  // Count when 0 appear
  while (i < len) {
    res[c] = histogram[i];
    (*words_height)[c] = i;
    while (i < len && histogram[i] != 0)
      i++;
    c++;
    while (i < len && histogram[i] == 0)
      i++;
  }
  return res;
}

BoundingBox **histo_words_detect(SDL_Surface *surface, int *word_count) {
  int len;
  BoundingBox **boxes = get_char_boxes(surface, &len);

  BoundingBox *grid_box = histo_grid_detect(surface);
  if (grid_box == NULL)
    return NULL;

  int *col_histo = get_col_box_histogram(surface, boxes, len);
  int *row_histo = get_row_box_histogram(surface, boxes, len);
  int new_len = len - get_most_frequent(col_histo, surface->w) *
                          get_most_frequent(row_histo, surface->h);
  free(row_histo);
  free(col_histo);

  BoundingBox **word_list_boxes = malloc(new_len * sizeof(BoundingBox *));
  for (int i = 0; i < new_len; i++) {
    word_list_boxes[i] = NULL;
  }
  int c = 0;
  for (int i = 0; i < len; i++) {
    BoundingBox *box = boxes[i];
    if (!is_in_boxes(&grid_box, 1, get_bounding_box_center(box))) {
      word_list_boxes[c] = box;
      c++;
    }
  }

  free(grid_box);

  sort_bounding_boxes(&word_list_boxes, new_len, ascending_bounding_box);

  BoundingBox *word_list_box = malloc(1 * sizeof(BoundingBox));
  Point p1 = {get_min_x(word_list_boxes, new_len),
              get_min_y(word_list_boxes, new_len)};
  Point p2 = {get_max_x(word_list_boxes, new_len),
              get_max_y(word_list_boxes, new_len)};
  *word_list_box = (BoundingBox){p1, p2};

  int *col_words_histo =
      get_row_bounded_box_histogram(word_list_boxes, word_list_box, new_len);

  *word_count = count_except_null(col_words_histo, word_list_box->p2.y);

  int *words_height = calloc(*word_count, sizeof(int));
  int *word_lengths = get_words_length(col_words_histo, word_list_box->p2.y,
                                       *word_count, &words_height);
  free(col_words_histo);

  BoundingBox **res = calloc(*word_count, sizeof(BoundingBox *));

  for (int i = 0; i < *word_count; i++) {
    int c = 0;
    Point p = {0, words_height[i]};
    BoundingBox **word_box = calloc(word_lengths[i], sizeof(BoundingBox *));

    // Find the full word
    for (int j = 0; j < new_len; j++) {
      BoundingBox *box = word_list_boxes[j];
      p.x = get_bounding_box_center(box).x;
      if (is_in_boxes(&box, 1, p)) {
        word_box[c] = box;
        c++;
      }
    }

    if (c != word_lengths[i])
      return NULL;

    BoundingBox *word_res = calloc(1, sizeof(BoundingBox));
    word_res->p1 = word_box[0]->p1;
    word_res->p2 = word_box[word_lengths[i] - 1]->p2;

    free(word_box);

    res[i] = word_res;
  }

  free(words_height);
  free(word_lengths);
  free(word_list_box);
  free(word_list_boxes);
  free_boxes(boxes, len);

  return res;
}

BoundingBox **get_word_boxes(SDL_Surface *surface, int *word_count) {
  return histo_words_detect(surface, word_count);
}

BoundingBox *get_word_list_box(SDL_Surface *surface) {
  return histo_word_list_detect(surface);
}

BoundingBox *get_grid_box(SDL_Surface *surface) {
  return histo_grid_detect(surface);
}

#include "cluster-detection.h"

int is_background(SDL_Surface *surface, Point coord) {
  Uint32 pixel = ((Uint32 *)surface->pixels)[coord.y * surface->w + coord.x];
  Uint8 r;
  Uint8 g;
  Uint8 b;
  SDL_GetRGB(pixel, surface->format, &r, &g, &b);
  if (r == 0)
    return 1;
  else
    return 0;
}

// int nb = 0;

void flood_fill(SDL_Surface *surface, Point *p1, Point *p2, Point coord,
                int **visited) {
  if (coord.x <= 0 || coord.y <= 0 || coord.x >= surface->w - 1 ||
      coord.y >= surface->h - 1)
    return;
  if (is_background(surface, coord) == 1 ||
      (*visited)[coord.y * surface->w + coord.x] == 1)
    return;

  (*visited)[coord.y * surface->w + coord.x] = 1;

  if (coord.x == p1->x)
    p1->x--;
  if (coord.y == p1->y)
    p1->y--;
  if (coord.x == p2->x)
    p2->x++;
  if (coord.y == p2->y)
    p2->y++;

  int dir_x[] = {0, 0, -1, 1};
  int dir_y[] = {-1, 1, 0, 0};
  for (size_t i = 0; i < 4; i++) {
    Point n_coord = {coord.x + dir_x[i], coord.y + dir_y[i]};
    flood_fill(surface, p1, p2, n_coord, visited);
  }
}

int *eq_table_insert(int *table_len, int *table, int a, int b) {
  int new_len = max(max(a + 1, b + 1), *table_len);
  // If the new elements are bigger than the length of the table
  // then realloc a new array and initialize all new elements to 0
  if (*table_len < new_len) {
    table = realloc(table, new_len * sizeof(int));
    for (int i = *table_len; i < new_len; i++) {
      table[i] = 0;
    }
    *table_len = new_len;
  }
  // Check if equivalence already created
  int eq_a = a;
  int eq_b = b;
  while (table[eq_a] != 0)
    eq_a = table[eq_a];
  while (table[eq_b] != 0)
    eq_b = table[eq_b];
  if (eq_a == eq_b)
    return table;

  // Create equivalence
  int lower = min(a, b);
  int greater = max(a, b);
  int eq1 = table[lower];
  int eq2 = table[greater];

  // Both are pointing to 0 then
  if (eq1 == 0 && eq2 == 0)
    // greater -> lower
    table[greater] = lower;
  // Lower pointing to 0 and greater pointing to eq2 then
  else if (eq1 == 0 && eq2 != 0) {
    // If eq2 < lower < greater then
    if (eq2 < lower)
      // lower -> eq2
      table[lower] = eq2;
    // If lower < eq2 < greater then
    if (lower < eq2) {
      // eq2 -> lower
      // greater -> lower
      table[eq2] = lower;
      table[greater] = lower;
    }
  }
  // Greater pointing to 0 and lower pointing to eq1 then
  else if (eq1 != 0 && eq2 == 0)
    // greater -> eq1
    table[greater] = eq1;
  // Lower pointing to eq1 and greater to eq2
  else if (eq1 != 0 && eq2 != 0) {
    // if eq1 < eq2
    if (eq1 < eq2) {
      // greater -> eq1
      // eq2 -> eq1
      table[greater] = eq1;
      table[eq2] = eq1;
    }
    // if eq2 < eq1
    else {
      // lower -> eq2
      // eq1 -> eq2
      table[lower] = eq2;
      table[eq1] = eq2;
    }
  }
  return table;
}

void compute_equivalences(int len, int **table) {
  for (int i = len - 1; i > 0; i--) {
    int new = (*table)[i];
    while ((*table)[new] != 0)
      new = (*table)[new];
    (*table)[i] = new;
  }
}

/* Compute all bounding boxes of the surface and store them in box_list
 * We assume box_list has already been initialized with a sentinel
 */
void compute_bounding_boxes(SDL_Surface *surface, struct list *box_list) {
  int *clusters = calloc(surface->w * surface->h, sizeof(int));
  int *eq_table = calloc(1, sizeof(int));
  int eq_table_len = 1;
  int cluster_id = 1;
  for (int j = 0; j < surface->h; j++) {
    for (int i = 0; i < surface->w; i++) {
      if (!get_gpixel_from_coord(surface, i, j))
        continue;

      // If diagonal then same cluster
      if (i > 0 && j > 0 && clusters[(j - 1) * surface->w + i - 1])
        clusters[j * surface->w + i] = clusters[(j - 1) * surface->w + i - 1];
      // If strictly left or strictly above then same cluster
      // Case above
      else if (((i > 0 && !clusters[j * surface->w + i - 1]) || i <= 0) &&
               j > 0 && clusters[(j - 1) * surface->w + i])
        clusters[j * surface->w + i] = clusters[(j - 1) * surface->w + i];
      // Case left
      else if (((j > 0 && !clusters[(j - 1) * surface->w + i]) || j <= 0) &&
               i > 0 && clusters[j * surface->w + i - 1])
        clusters[j * surface->w + i] = clusters[j * surface->w + i - 1];
      // If both then same cluster + note to equivalence table
      else if (j > 0 && i > 0 && clusters[j * surface->w + i - 1] &&
               clusters[(j - 1) * surface->w + i]) {
        clusters[j * surface->w + i] = clusters[(j - 1) * surface->w + i];
        eq_table = eq_table_insert(&eq_table_len, eq_table,
                                   clusters[j * surface->w + i - 1],
                                   clusters[(j - 1) * surface->w + i]);
      }
      // Else new cluster
      else {
        eq_table = eq_table_insert(&eq_table_len, eq_table, cluster_id, 0);
        clusters[j * surface->w + i] = cluster_id++;
      }
    }
  }
  if (cluster_id == 1) {
    printf("No pixels found.\n");
    return;
  }

  // Compute final equivalences
  compute_equivalences(eq_table_len, &eq_table);

  // Resolve equivalences
  for (int i = 0; i < surface->w * surface->h; i++) {
    if (eq_table[clusters[i]] != 0)
      clusters[i] = eq_table[clusters[i]];
  }

  BoundingBox **boxes = calloc(cluster_id, sizeof(BoundingBox));
  for (int i = 0; i < cluster_id; i++) {
    boxes[i] = calloc(1, sizeof(BoundingBox));
  }

  int *count_pixels = calloc(cluster_id, sizeof(int));
  int *seen = calloc(cluster_id, sizeof(int));
  int id;
  for (int j = 0; j < surface->h; j++) {
    for (int i = 0; i < surface->w; i++) {
      id = clusters[j * surface->w + i];
      if (!id || eq_table[id] != 0)
        continue;
      count_pixels[id]++;
      if (!seen[id]) {
        seen[id] = 1;
        box_init_coord(boxes[id], i, j);
        boxes[id]->start = (Point){i, j};
      } else {
        box_expand_coord(boxes[id], i, j);
      }
    }
  }
  free(seen);

  // Construct struct list* box_list
  for (int i = 1; i < cluster_id; i++) {
    if (eq_table[i] == 0) {
      list_push_front(box_list, boxes[i]);
      box_list->next->box->pixel_nb = count_pixels[i];
    }
  }
  free(boxes);
}
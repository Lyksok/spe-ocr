#ifndef MY_PRINT_H
#define MY_PRINT_H

#include <stdio.h>
#include <stdarg.h>
#include <gtk/gtk.h>
#include "main.h" // for the builder

void my_print(const char *format, ...); // The '...' is a variadic argument
void init_log_view(const char *ui_file);

#endif
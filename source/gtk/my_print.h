#ifndef MY_PRINT_H
#define MY_PRINT_H

#include "main.h" // for the builder
#include <gtk/gtk.h>
#include <stdarg.h>
#include <stdio.h>

void my_print(const char *format, ...); // The '...' is a variadic argument
void init_log_view(const char *ui_file);

#endif
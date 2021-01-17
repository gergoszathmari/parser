#ifndef INTERFACE_H
#define INTERFACE_H

static const unsigned char SET_UNSIGNED = 0x1;
static const unsigned char SET_CONST = 0x2;
static const unsigned char SET_VOLATILE = 0x4;
static const unsigned char SET_POINTER = 0x8;
static const unsigned char SET_STATIC = 0x10;

#include <stdlib.h>

int sym_type(const char *);
void printTypenames();
void clearTypes();

#endif // INTERFACE_H
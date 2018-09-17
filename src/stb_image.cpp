/**
 * @file stb_iamge.cpp
 * The stb_image library requires that STB_IMAGE_IMPLEMENTATION be defined only
 * once in the target, before including the library. This source file is simply
 * a way to guarantee that the definition is included once and only once.
 * @author Matthew McLaurin
 */
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

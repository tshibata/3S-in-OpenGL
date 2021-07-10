#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <png.h>
#include <platform.h>
#include "files.h"

namespace sss
{

#define UC_MAGIC 299792
#define UC_VERSION 1.0

void readBin(const char * path, unsigned char * * data, size_t * size)
{
	struct stat s;
	if (stat(path, & s) != 0)
	{
		exit(1); // failed to get file status
	}
	FILE * fp = fopen(path, "rb");
	if (! fp)
	{
		exit(2); // failed to open
	}
	float sig;
	if (fread(& sig, sizeof(sig), 1, fp) != 1)
	{
		exit(3); // failed to read signature
	}
	if (sig != UC_MAGIC + UC_VERSION)
	{
		exit(4); // wrong signature
	}
	* size = s.st_size - sizeof(sig);
	* data = (unsigned char *) malloc(* size);
	for (int i = 0; i < * size ; i++)
	{
		(* data)[i] = fgetc(fp);
	}
	fclose(fp);
}

void readPng(const char * path, unsigned char * * data, int * width, int * height, unsigned int * type)
{
	png_byte sig[8];
	png_structp png;
	png_infop info;
	FILE * fp = fopen(path, "rb");
	if (! fp)
	{
		exit(1); // failed to open
	}
	if (fread(sig, sizeof(sig), 1, fp) != 1)
	{
		exit(2); // failed to read signature
	}
	if (png_sig_cmp(sig, 0, sizeof(sig)) != 0)
	{
		exit(3); // wrong signature
	}
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (png == nullptr) {
		exit(4); // failed because of memory or whatever
	}
	info = png_create_info_struct(png);
	if (info == nullptr) {
		exit(5); // failed because of memory or whatever
	}
	png_init_io(png, fp);
	png_set_sig_bytes(png, sizeof(sig));
	int xform = PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_EXPAND;
	png_read_png(png, info, xform, nullptr);
	* width = png_get_image_width(png, info);
	* height = png_get_image_height(png, info);
	png_bytepp rows = png_get_rows(png, info);
	int rowbytes = png_get_rowbytes(png, info);
	switch (png_get_bit_depth(png, info))
	{
	case 8:
		break;
	default:
		exit(6); // unsupported format
	}
	// convert the jagged array into a linear array
	* data = (unsigned char *) malloc(rowbytes * * height);
	for (int i = 0; i < * height; i++)
	{
		for (int j = 0; j < rowbytes; j++)
		{
			(* data)[i * rowbytes + j] = rows[i][j];
		}
	}
	switch (png_get_color_type(png, info))
	{
	case PNG_COLOR_TYPE_RGB:
		* type = GL_RGB;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		* type = GL_RGBA;
		break;
	default:
		exit(6); // unsupported format
	}
	png_destroy_read_struct(& png, & info, nullptr);
	fclose(fp);
}

}

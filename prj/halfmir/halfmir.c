#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

#define MAXFILELEN 1024

const char *cmd;
const char *outstyle = "-hm";
int opt_l = 0;

void
usage(FILE *output)
{
	fprintf(output, "This program makes half-transparent" \
	                " half-mirrored images.\n");
	fprintf(output, "Usage: %s [-h] [-s str] [-l] [--] file1" \
	                " [file2 ...]\n", cmd);
}

const char *getoutfilename(const char *filename) {
	static char ret[MAXFILELEN];
	int i;

	for (i = 0; filename[i] && i < MAXFILELEN - 5 \
	                          - strlen(outstyle); i++) {
		if (!strcmp(filename + i, ".png"))
			break;
		ret[i] = filename[i];
	}

	sprintf(ret + i, "%s.png", outstyle);

	return ret;
}

void
handlepixels(png_struct *src, png_struct *dst, png_info *info)
{
	int i, j, h, k;
	png_uint_32 N;
	png_bytep rs, rd;
	png_byte p;

	h = png_get_image_height(src, info);
	N = png_get_rowbytes(src, info);
	p = png_get_channels(src, info) \
	    * png_get_bit_depth(src, info) / 4;

	rs = malloc(N);
	rd = malloc(N);
	for (k = 0; k < h; k++) {
		png_read_row(src, rs, NULL);
		for (i = 0; i < N / p; i++) {
			for (j = 0; j < p; j++) {
				if (opt_l) {
					rd[i*p + j] = (k%2) * rs[i*p + j] \
					       + !(k%2) * rs[N - (i+1)*p + j];
				} else {
					rd[i*p + j] = ( rs[i*p + j] \
					          + rs[N - (i+1)*p + j] ) / 2;
				}
			}
		}
		png_write_row(dst, rd);
	}
	free(rs);
	free(rd);
}

int
halfmirfile(const char *filename)
{
	png_struct *png1, *png2;
	png_info *info, *endinfo;
	const char *outfilename = getoutfilename(filename);
	FILE *input, *output;

	if (!(input = fopen(filename, "rb"))) {
		fprintf(stderr, "Cannot open file %s.\n", filename);
		goto noinput;
	}
	if (!(output = fopen(outfilename, "wb"))) {
		fprintf(stderr, "Cannot open file %s.\n", outfilename);
		goto nooutput;
	}

	if (!(png1 = png_create_read_struct(PNG_LIBPNG_VER_STRING,
	                                    NULL, NULL, NULL))) {
		fputs("Cannot create png read struct.\n", stderr);
		goto nopng1;
	}
	if (!(info = png_create_info_struct(png1))) {
		fputs("Cannot create read info struct.\n", stderr);
		goto noinfo;
	}
	if (!(endinfo = png_create_info_struct(png1))) {
		fputs("Cannot create end info struct.\n", stderr);
		goto noendinfo;
	}
	if (!(png2 = png_create_write_struct(PNG_LIBPNG_VER_STRING,
	                                     NULL, NULL, NULL))) {
		fputs("Cannot create png write struct.\n", stderr);
		goto nopng2;
	}
	png_init_io(png1, input);
	png_init_io(png2, output);

	png_read_info(png1, info);
	png_write_info(png2, info);
	
	handlepixels(png1, png2, info);

	png_read_end(png1, endinfo);
	png_write_end(png2, endinfo);

	png_destroy_read_struct(&png1, &info, &endinfo);
	png_destroy_write_struct(&png2, NULL);

	return 0;

nopng2:
	png_destroy_info_struct(png1, &endinfo);
noendinfo:
	png_destroy_info_struct(png1, &info);
noinfo:
	png_destroy_read_struct(&png1, NULL, NULL);
nopng1:
	fclose(output);
nooutput:
	fclose(input);
noinput:
	return 1;
}

const char *
chararg(int *argc, char ***argv, char **opt)
{
	const char *ret;

	if (opt[0][1]) {
		ret = *opt + 1;
		fputs("one", stdout);
	} else if (argv[0][1]) {
		*argc -= 1; *argv += 1;
		ret = **argv;
	} else {
		fprintf(stderr, "-%c flag needs a string agrument.\n", **opt);
		return NULL;
	}
	*opt += strlen(*opt) - 1;

	return ret;
}

int
main(int argc, char **argv)
{
	cmd = *argv;
	for (argc--, argv++; *argv && **argv == '-'; argc--, argv++) {
		if (!strcmp(*argv, "--")) {
			argv++;
			break;
		}
		for (char *opt = ++*argv; *opt; opt++) {
			switch (*opt) {
			case 's':
				/* Specify another outstyle */
				if (!(outstyle = chararg(&argc, &argv, &opt)))
					return 1;
				break;
			case 'l':
				/* Mirror each second line */
				opt_l = 1;
				break;
			case 'h':
				usage(stdout);
				return 0;
			default:
				fprintf(stderr, "Unknown option %c\n", *opt);
				usage(stderr);
				return 1;
			}
		}
	}

	/* The rest of argv is filenames */
	if (!(*argv)) {
		fputs("No filenames provided.\n", stderr);
		return 1;
	}
	for (; *argv; argc--, argv++) {
		if (halfmirfile(*argv))
			return 1;
	}

	return 0;
}

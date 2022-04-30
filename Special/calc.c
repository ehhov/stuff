/*
 * Using this program, you can calculate DPI based on the resolution
 * and diagonal or side lengths based on the resolution and DPI.
 * I use it to configure 20-display.conf and .Xresousces.
 *
 * Don't forget the -lm when linking.
 */
#include <stdio.h>
#include <math.h>

static const double inmm = 25.4;

static void
DPI(int X, int Y, double diagonal)
{
	printf("DPI: %lf\n", X / diagonal * sqrt(1 + Y*Y * 1.0 / X/X));
}

static void
sides(int X, int Y, int DPI)
{
	printf("Sides from DPI %d: %.0lfmm x %.0lfmm\n",
	       DPI, inmm * X / DPI, inmm * Y / DPI);
}

static void
realsides(int X, int Y, double diagonal)
{
	double Xside = diagonal / sqrt(1 + 1.0 * Y*Y / X/X) * inmm;
	printf("Sides from diagonal %.1lf in: %.0lfmm x %.0lfmm\n",
	       diagonal, Xside, Xside * Y / X);
}

int
main()
{
	int Xres = 1920, Yres = 1080;
	double diagonal = 13.3; /* in inches */
	int dpi = 130;

	DPI(Xres, Yres, diagonal);
	sides(Xres, Yres, dpi);
	realsides(Xres, Yres, diagonal);

	return 0;
}

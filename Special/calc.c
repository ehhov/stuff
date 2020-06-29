/*
 * using this program, you can calculate DPI based on the resolution
 * and diagonal or side lengths based on the resolution and DPI.
 * I use it to configure 20-display.conf and .Xresousces.
 *
 * Don't forget the -lm when linking.
 */
#include <stdio.h>
#include <math.h>

double inmm = 25.4;

void
DPI(int X, int Y, double diagonal)
{
	printf("%lf\n", X / diagonal * sqrt(1 + Y*Y * 1.0 / X/X));
}

void
sides(int X, int Y, double DPI)
{
	printf("%.0lfmm x %.0lfmm\n", X / DPI * inmm, Y / DPI * inmm);
}

void
realsides(int X, int Y, double diagonal)
{
	double Xside = diagonal / sqrt(1 + 1.0 * Y*Y / X/X) * inmm;
	printf("%.0lfmm x %.0lfmm\n", Xside, Xside * Y / X);
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

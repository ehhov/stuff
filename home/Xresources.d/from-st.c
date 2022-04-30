#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const char *Xres_output = "colors";
static const char *live_schemes = "live_colors.sh";
static const char *alarcitty_colors = "colors.yml";

static const char *
vim_color_filename(const char *filename)
{
	static char buf[BUFSIZ];
	static char *home = NULL;
	static char *config = NULL;

	/* XDG_CONFIG_HOME... */
	if (!config && !home)
		config = getenv("XDG_CONFIG_HOME");
	if (!config && !home)
		home = getenv("HOME");
	if (!config && !home)
		exit(1);

	if (config)
		snprintf(buf, sizeof(buf), "%s/nvim/colors/gui_%s.vim", config, filename);
	else
		snprintf(buf, sizeof(buf), "%s/.config/nvim/colors/gui_%s.vim", home, filename);
	return buf;
}

static void
vim_append_gui_from_terminal(FILE* file)
{
	char buf[BUFSIZ];
	FILE *source;
	size_t n, i;

	if (!(source = fopen(vim_color_filename("from_terminal"), "r"))) {
		fputs("Failed to append gui_from_terminal.\n", stderr);
		return;
	}

	fputs("\n\" === Apply the actual colorscheme === {{""{\n", file);
	while ((n = fread(buf, sizeof(*buf), BUFSIZ, source)) > 0)
		for (i = 0; i < n; i++)
			fputc(buf[i], file);
	fputs("\" }}""}\n", file);

	fclose(source);
}


/*
 * Copy the palette from st/config.h here
 * Delete all the [n]=
 * Edit the theme names variable if needed
 * Compile and run while in ~/.Xresources.d/
 */
static const char *palettes[][259] = {
/* black     red        green      yellow     blue       magenta    cyan      white
 * [256] = bg fg cur */
{"#000000", "#FF2424", "#00c000", "#CC7700", "#0055FF", "#d769ce", "#1D97FC", "#B9BBB9",
 "#D7EFDD", "#FB3737", "#40BF4A", "#E69119", "#3846ff", "#c779de", "#09B3B3", "#ffffff",
 [256] = "#e4f7e4", "#000000", "#000000"}, /* My light theme */

{"#000000", "#d73a49", "#2cbe4e", "#e36209", "#005cc5", "#f34b7d", "#00add8", "#babfc4",
 "#eff2f6", "#cb2431", "#2c974b", "#f97732", "#036ee7", "#6f42c1", "#00b4ab", "#ffffff",
 [256] = "#f6f8fa", "#24292e", "#24292e"}, /* GitHub */

{"#000000", "#ff5555", "#50fa7b", "#FFB366", "#bd93f9", "#ff79c6", "#8be9fd", "#8B8BA7",
 "#34344B", "#FF4444", "#3DF56B", "#f1fa8c", "#B78CF2", "#F881C5", "#6EDEF7", "#ffffff",
 [256] = "#282a36", "#d8d8d2", "#d8d8d2"}, /* Dracula */

{"#3b4252", "#bf616a", "#a3be8c", "#ebcb8b", "#81a1c1", "#b48ead", "#88c0d0", "#757C8A",
 "#383E4D", "#bf616a", "#a3be8c", "#ebcb8b", "#81a1c1", "#b48ead", "#8fbcbb", "#eceff4",
 [256] = "#2e3440", "#d8dee9", "#d8dee9"}, /* Nord theme */

{"#002129", "#dc322f", "#859900", "#bf6f00", "#268bd2", "#d33682", "#2aa198", "#586e75",
 "#073642", "#cb4b16", "#586e00", "#a57b00", "#3374b6", "#8c71b4", "#43b1a1", "#fdf6e3",
 [256] = "#002b36", "#839496", "#93a1a1"}, /* Solarized dark */

{"#eee8d5", "#dc322f", "#859900", "#b58900", "#268bd2", "#d33682", "#2aa198", "#b5b5ab",
 "#ddd6c3", "#cb4b36", "#589e22", "#a39400", "#657bb3", "#6c71c4", "#589ea5", "#002b36",
 [256] = "#fdf6e3", "#556b73", "#586e75"}, /* Solarized light */

{"#eee8d5", "#dc322f", "#859900", "#b58900", "#268bd2", "#d33682", "#2aa198", "#b5b5ab",
 "#ddd6c3", "#cb4b36", "#589e22", "#a39400", "#657bb3", "#6c71c4", "#589ea5", "#002b36",
 [256] = "#1F1F28", "#DCD7BA", "#54546D"}, /* Kanagawa */

{"#1c1b19", "#ef2f27", "#519f50", "#fbb829", "#2c78bf", "#e02c6d", "#0aaeb3", "#6C6860",
 "#292724", "#f75341", "#98bc37", "#fed06e", "#68a8e4", "#ff5c8f", "#2be4d0", "#fce8c3",
 [256] = "#1c1b19", "#fce8c3", "#fbb829"}, /* Srcery */

{"#101010", "#D67B76", "#80c990", "#C79D73", "#a3b8ef", "#e6a3dc", "#50cacd", "#606060",
 "#252525", "#E0A084", "#5accaf", "#c8c874", "#ccaced", "#f2a1c2", "#74c3e4", "#c0c0c0",
 [256] = "#101010", "#a0a0a0", "#a0a0a0"}, /* soft dark */

{"#2E3436", "#CC0000", "#4B910B", "#C4A000", "#3465A4", "#755079", "#06989A", "#916E86",
 "#401C35", "#EF2929", "#8AE234", "#FCE94F", "#729FCF", "#AD7FA8", "#34E2E2", "#EEEEEC",
 [256] = "#300a24", "#ffffff", "#ffffff"}, /* Ubuntu */

{"#282828", "#cc241d", "#98971a", "#d79921", "#458588", "#b16286", "#689d6a", "#95836A",
 "#37352F", "#fb4934", "#b8bb26", "#fabd2f", "#83a598", "#d3869b", "#8ec07c", "#ebdbb2",
 [256] = "#282828", /* hard #1d2021 / soft #32302f */ "#ebdbb2", "#ebdbb2"}, /* Gruvbox dark */

{"#fbf1c7", "#cc241d", "#98971a", "#d79921", "#458588", "#b16286", "#689d6a", "#BEAD9D",
 "#F5EBBC", "#9d0006", "#79740e", "#b57614", "#076678", "#8f3f71", "#427b58", "#3c3836",
 [256] = "#fbf1c7", /* hard #f9f5d7 / soft #f2e5bc */ "#3c3836", "#3c3836"}, /* Gruvbox light */

{"#555555", "#E06C75", "#98C379", "#f5dd7B", "#60aaff", "#C678DD", "#46c6b2", "#c0c0c0",
 "#777777", "#dd0000", "#00dd00", "#eebb33", "#5c5cff", "#dd00dd", "#22aacc", "#ffffff",
 [256] = "#222222", "#cccccc", "#cccccc"}, /* My dark theme */

{"#073642", "#dc322f", "#859900", "#b58900", "#268bd2", "#d33682", "#2aa198", "#eee8d5",
 "#002b36", "#cb4b16", "#586e75", "#657b83", "#839496", "#6c71c4", "#93a1a1", "#fdf6e3",
 [256] = "#002b36", "#a3b4b6", "#93a1a1"}, /* Solarized dark original */

{"#eee8d5", "#dc322f", "#859900", "#b58900", "#268bd2", "#d33682", "#2aa198", "#073642",
 "#fdf6e3", "#cb4b16", "#93a1a1", "#839496", "#657b83", "#6c71c4", "#586e75", "#002b36",
 [256] = "#fdf6e3", "#657b83", "#586e75"}, /* Solarized light original */
};
static const char *names[] = {
	"light", "github", "Dracula", "Nord", "Solarized_dark", "Solarized_light",
	"Kanagawa", "Srcery", "soft_dark", "Ubuntu", "Gruvbox_dark", "Gruvbox_light",
	"dark", "Solarized_dark_orig", "Solarized_light_orig"
};

int main() {
	int N = sizeof(palettes) / sizeof(*palettes), i;
	FILE *file;

	if (file = fopen(Xres_output, "w")) {
		fputs("! vim:ft=xdefaults\n"
		      "! === This file is created using ~/.Xresources.d/from-st.c ===\n", file);
		for (i = 0; i < N; i++) {
			fprintf(file, "\n! %s color scheme\n", names[i]);
			fprintf(file, "#define %s_c0    %s\n", names[i], palettes[i][0]);
			fprintf(file, "#define %s_c1    %s\n", names[i], palettes[i][1]);
			fprintf(file, "#define %s_c2    %s\n", names[i], palettes[i][2]);
			fprintf(file, "#define %s_c3    %s\n", names[i], palettes[i][3]);
			fprintf(file, "#define %s_c4    %s\n", names[i], palettes[i][4]);
			fprintf(file, "#define %s_c5    %s\n", names[i], palettes[i][5]);
			fprintf(file, "#define %s_c6    %s\n", names[i], palettes[i][6]);
			fprintf(file, "#define %s_c7    %s\n", names[i], palettes[i][7]);
			fprintf(file, "#define %s_c8    %s\n", names[i], palettes[i][8]);
			fprintf(file, "#define %s_c9    %s\n", names[i], palettes[i][9]);
			fprintf(file, "#define %s_c10   %s\n", names[i], palettes[i][10]);
			fprintf(file, "#define %s_c11   %s\n", names[i], palettes[i][11]);
			fprintf(file, "#define %s_c12   %s\n", names[i], palettes[i][12]);
			fprintf(file, "#define %s_c13   %s\n", names[i], palettes[i][13]);
			fprintf(file, "#define %s_c14   %s\n", names[i], palettes[i][14]);
			fprintf(file, "#define %s_c15   %s\n", names[i], palettes[i][15]);
			fprintf(file, "#define %s_bg    %s\n", names[i], palettes[i][256]);
			fprintf(file, "#define %s_fg    %s\n", names[i], palettes[i][257]);
			fprintf(file, "#define %s_cur   %s\n", names[i], palettes[i][258]);
		}
		fclose(file);
	}

	if (file = fopen(live_schemes, "w")) {
		fputs("#!/bin/sh\n"
		      "# === This file is created using ~/.Xresources.d/from-st.c ===\n"
		      "# The script makes use of color changing escape sequences\n"
		      "# They are supported by xterm, tilix, alacritty, (urxvt?) and not by st\n"
		      "# For some details, see e.g. https://github.com/alacritty/alacritty/issues/656\n"
		      "\nschemes=\"\"\n", file);
		for (i = 0; i < N; i++) {
			fprintf(file, "\n%s() {\n", names[i]);
			fprintf(file, "\tprintf \"\\033]4;0;%s\\007\"\n",   palettes[i][0]);
			fprintf(file, "\tprintf \"\\033]4;1;%s\\007\"\n",   palettes[i][1]);
			fprintf(file, "\tprintf \"\\033]4;2;%s\\007\"\n",   palettes[i][2]);
			fprintf(file, "\tprintf \"\\033]4;3;%s\\007\"\n",   palettes[i][3]);
			fprintf(file, "\tprintf \"\\033]4;4;%s\\007\"\n",   palettes[i][4]);
			fprintf(file, "\tprintf \"\\033]4;5;%s\\007\"\n",   palettes[i][5]);
			fprintf(file, "\tprintf \"\\033]4;6;%s\\007\"\n",   palettes[i][6]);
			fprintf(file, "\tprintf \"\\033]4;7;%s\\007\"\n",   palettes[i][7]);
			fprintf(file, "\tprintf \"\\033]4;8;%s\\007\"\n",   palettes[i][8]);
			fprintf(file, "\tprintf \"\\033]4;9;%s\\007\"\n",   palettes[i][9]);
			fprintf(file, "\tprintf \"\\033]4;10;%s\\007\"\n",  palettes[i][10]);
			fprintf(file, "\tprintf \"\\033]4;11;%s\\007\"\n",  palettes[i][11]);
			fprintf(file, "\tprintf \"\\033]4;12;%s\\007\"\n",  palettes[i][12]);
			fprintf(file, "\tprintf \"\\033]4;13;%s\\007\"\n",  palettes[i][13]);
			fprintf(file, "\tprintf \"\\033]4;14;%s\\007\"\n",  palettes[i][14]);
			fprintf(file, "\tprintf \"\\033]4;15;%s\\007\"\n",  palettes[i][15]);
			fprintf(file, "\tprintf \"\\033]11;%s\\007\"\n",    palettes[i][256]);
			fprintf(file, "\tprintf \"\\033]708;%s\\007\"\n",   palettes[i][256]);
			fprintf(file, "\tprintf \"\\033]10;%s\\007\"\n",    palettes[i][257]);
			fprintf(file, "\tprintf \"\\033]12;%s\\007\"\n",    palettes[i][258]);
			fprintf(file, "}\n");
			fprintf(file, "schemes=\"$schemes %s\"\n", names[i]);
		}
		fputs("\nPS3=\"Choose a color scheme: \"\n" \
		      "select scheme in $schemes \"Do not change\"\n" \
		      "do\n" \
		      "\t${scheme#Do not change}\n" \
		      "\tbreak\n" \
		      "done\n", file);
		fclose(file);
	}

	if (file = fopen(alarcitty_colors, "w")) {
		fputs("schemes:\n", file);
		for (i = 0; i < N; i++) {
			fprintf(file, "\n  %s: &%s\n", names[i], names[i]);
			fprintf(file, "    primary:\n");
			fprintf(file, "      background: '%s'\n", palettes[i][256]);
			fprintf(file, "      foreground: '%s'\n", palettes[i][257]);
			fprintf(file, "    normal:\n");
			fprintf(file, "      black: '%s'\n",      palettes[i][0]);
			fprintf(file, "      red: '%s'\n",        palettes[i][1]);
			fprintf(file, "      green: '%s'\n",      palettes[i][2]);
			fprintf(file, "      yellow: '%s'\n",     palettes[i][3]);
			fprintf(file, "      blue: '%s'\n",       palettes[i][4]);
			fprintf(file, "      magenta: '%s'\n",    palettes[i][5]);
			fprintf(file, "      cyan: '%s'\n",       palettes[i][6]);
			fprintf(file, "      white: '%s'\n",      palettes[i][7]);
			fprintf(file, "    bright:\n");
			fprintf(file, "      black: '%s'\n",      palettes[i][8]);
			fprintf(file, "      red: '%s'\n",        palettes[i][9]);
			fprintf(file, "      green: '%s'\n",      palettes[i][10]);
			fprintf(file, "      yellow: '%s'\n",     palettes[i][11]);
			fprintf(file, "      blue: '%s'\n",       palettes[i][12]);
			fprintf(file, "      magenta: '%s'\n",    palettes[i][13]);
			fprintf(file, "      cyan: '%s'\n",       palettes[i][14]);
			fprintf(file, "      white: '%s'\n",      palettes[i][15]);
		}
		fclose(file);
	}

	for (int i = 0; i < N; i++) {
		if (!(file = fopen(vim_color_filename(names[i]), "w")))
			continue;

		fprintf(file, "let g:colors_name = \'%s\'\n\n", names[i]);
		fprintf(file, "let s:c0  = \'%s\'\n", palettes[i][0]);
		fprintf(file, "let s:c1  = \'%s\'\n", palettes[i][1]);
		fprintf(file, "let s:c2  = \'%s\'\n", palettes[i][2]);
		fprintf(file, "let s:c3  = \'%s\'\n", palettes[i][3]);
		fprintf(file, "let s:c4  = \'%s\'\n", palettes[i][4]);
		fprintf(file, "let s:c5  = \'%s\'\n", palettes[i][5]);
		fprintf(file, "let s:c6  = \'%s\'\n", palettes[i][6]);
		fprintf(file, "let s:c7  = \'%s\'\n", palettes[i][7]);
		fprintf(file, "let s:c8  = \'%s\'\n", palettes[i][8]);
		fprintf(file, "let s:c9  = \'%s\'\n", palettes[i][9]);
		fprintf(file, "let s:c10 = \'%s\'\n", palettes[i][10]);
		fprintf(file, "let s:c11 = \'%s\'\n", palettes[i][11]);
		fprintf(file, "let s:c12 = \'%s\'\n", palettes[i][12]);
		fprintf(file, "let s:c13 = \'%s\'\n", palettes[i][13]);
		fprintf(file, "let s:c14 = \'%s\'\n", palettes[i][14]);
		fprintf(file, "let s:c15 = \'%s\'\n", palettes[i][15]);
		fprintf(file, "let s:bg  = \'%s\'\n", palettes[i][256]);
		fprintf(file, "let s:fg  = \'%s\'\n", palettes[i][257]);
		fprintf(file, "let s:cur = \'%s\'\n", palettes[i][258]);
		vim_append_gui_from_terminal(file);

		fclose(file);
	}

	return 0;
}

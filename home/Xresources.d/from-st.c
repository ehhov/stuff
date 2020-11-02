#include <stdio.h>
#include <stdlib.h>

static const char *Xres_output = "colors";
static const char *liveschemes = "live_colors.sh";

static const char *
vimcolorfile(const char *filename)
{
	static char buf[1024];
	static char *home = NULL;

	/* XDG_CONFIG_HOME... */
	if (!home)
		home = getenv("HOME");
	if (!home)
		exit(1);

	snprintf(buf, sizeof(buf), "%s/.config/nvim/colors/gui_%s.vim", home, filename);
	return buf;
}

/*
 * Copy the palette from st/config.h here
 * Delete all the [n]=
 * Edit the theme names variable if needed
 * Compile and run while in ~/.Xresources.d/
 */
static const char *palettes[][259] = {
     /* black      red        green      yellow     blue       magenta    cyan       white  bg fg cur */
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

{"#101010", "#efa6a2", "#80c990", "#a69460", "#a3b8ef", "#e6a3dc", "#50cacd", "#808080",
 "#252525", "#e0af85", "#5accaf", "#c8c874", "#ccaced", "#f2a1c2", "#74c3e4", "#c0c0c0",
 [256] = "#101010", "#c0c0c0", "#c0c0c0"}, /* other dark */

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
	"light", "github", "Dracula", "Nord", "Solarized_dark", "Solarized_light", "other_dark", "Ubuntu", "Gruvbox_dark", "Gruvbox_light", "dark", "Solarized_dark_orig", "Solarized_light_orig"
};

int main() {
	int N = sizeof(palettes)/sizeof(palettes[0]);
	FILE *file;

	file=fopen(Xres_output, "w");
	fputs("! vim:ft=xdefaults\n" \
	      "!!! This file is created using ~/.Xresources.d/from-st.c !!!\n", file);
	for (int theme=0; theme<N; theme++){
		fprintf(file, "\n!!! %s color scheme\n", names[theme]);
		fprintf(file, "#define %s_c0    %s\n", names[theme], palettes[theme][0]);
		fprintf(file, "#define %s_c1    %s\n", names[theme], palettes[theme][1]);
		fprintf(file, "#define %s_c2    %s\n", names[theme], palettes[theme][2]);
		fprintf(file, "#define %s_c3    %s\n", names[theme], palettes[theme][3]);
		fprintf(file, "#define %s_c4    %s\n", names[theme], palettes[theme][4]);
		fprintf(file, "#define %s_c5    %s\n", names[theme], palettes[theme][5]);
		fprintf(file, "#define %s_c6    %s\n", names[theme], palettes[theme][6]);
		fprintf(file, "#define %s_c7    %s\n", names[theme], palettes[theme][7]);
		fprintf(file, "#define %s_c8    %s\n", names[theme], palettes[theme][8]);
		fprintf(file, "#define %s_c9    %s\n", names[theme], palettes[theme][9]);
		fprintf(file, "#define %s_c10   %s\n", names[theme], palettes[theme][10]);
		fprintf(file, "#define %s_c11   %s\n", names[theme], palettes[theme][11]);
		fprintf(file, "#define %s_c12   %s\n", names[theme], palettes[theme][12]);
		fprintf(file, "#define %s_c13   %s\n", names[theme], palettes[theme][13]);
		fprintf(file, "#define %s_c14   %s\n", names[theme], palettes[theme][14]);
		fprintf(file, "#define %s_c15   %s\n", names[theme], palettes[theme][15]);
		fprintf(file, "#define %s_bg    %s\n", names[theme], palettes[theme][256]);
		fprintf(file, "#define %s_fg    %s\n", names[theme], palettes[theme][257]);
		fprintf(file, "#define %s_cur   %s\n", names[theme], palettes[theme][258]);
	}
	fclose(file);

	file=fopen(liveschemes, "w");
	fputs("#!/bin/sh\n" \
	      "### This file is created using ~/.Xresources.d/from-st.c ###\n" \
	      "# The script makes use of color changing escape sequences\n" \
	      "# They are supported by xterm, tilix, alacritty, (urxvt?) and not by st\n" \
	      "# For some details, see e.g. https://github.com/alacritty/alacritty/issues/656\n" \
	      "\nschemes=\"\"\n", file);
	for (int theme=0; theme<N; theme++){
		fprintf(file, "\n%s() {\n", names[theme]);
		fprintf(file, "\tprintf \"\\033]4;0;%s\\007\"\n",   palettes[theme][0]);
		fprintf(file, "\tprintf \"\\033]4;1;%s\\007\"\n",   palettes[theme][1]);
		fprintf(file, "\tprintf \"\\033]4;2;%s\\007\"\n",   palettes[theme][2]);
		fprintf(file, "\tprintf \"\\033]4;3;%s\\007\"\n",   palettes[theme][3]);
		fprintf(file, "\tprintf \"\\033]4;4;%s\\007\"\n",   palettes[theme][4]);
		fprintf(file, "\tprintf \"\\033]4;5;%s\\007\"\n",   palettes[theme][5]);
		fprintf(file, "\tprintf \"\\033]4;6;%s\\007\"\n",   palettes[theme][6]);
		fprintf(file, "\tprintf \"\\033]4;7;%s\\007\"\n",   palettes[theme][7]);
		fprintf(file, "\tprintf \"\\033]4;8;%s\\007\"\n",   palettes[theme][8]);
		fprintf(file, "\tprintf \"\\033]4;9;%s\\007\"\n",   palettes[theme][9]);
		fprintf(file, "\tprintf \"\\033]4;10;%s\\007\"\n",  palettes[theme][10]);
		fprintf(file, "\tprintf \"\\033]4;11;%s\\007\"\n",  palettes[theme][11]);
		fprintf(file, "\tprintf \"\\033]4;12;%s\\007\"\n",  palettes[theme][12]);
		fprintf(file, "\tprintf \"\\033]4;13;%s\\007\"\n",  palettes[theme][13]);
		fprintf(file, "\tprintf \"\\033]4;14;%s\\007\"\n",  palettes[theme][14]);
		fprintf(file, "\tprintf \"\\033]4;15;%s\\007\"\n",  palettes[theme][15]);
		fprintf(file, "\tprintf \"\\033]11;%s\\007\"\n",    palettes[theme][256]);
		fprintf(file, "\tprintf \"\\033]708;%s\\007\"\n",   palettes[theme][256]);
		fprintf(file, "\tprintf \"\\033]10;%s\\007\"\n",    palettes[theme][257]);
		fprintf(file, "\tprintf \"\\033]12;%s\\007\"\n",    palettes[theme][258]);
		fprintf(file, "}\n");
		fprintf(file, "schemes=\"$schemes %s\"\n", names[theme]);
	}
	fputs("\nPS3=\"Choose a color scheme: \"\n" \
	      "select scheme in $schemes \"Do not change\"\n" \
	      "do\n" \
	      "\t${scheme/#Do not change/}\n" \
	      "\tbreak\n" \
	      "done\n", file);
	fclose(file);


	for (int theme=0; theme<N; theme++){
		file=fopen(vimcolorfile(names[theme]), "w");

		fputs("hi clear Normal\n"
		      "set bg&\n"
		      "hi clear\n"
		      "if exists(\"syntax_on\")\n"
		      "\tsyntax reset\n"
		      "endif\n\n", file);
		fprintf(file, "let g:colors_name = \"%s\"\n\n", names[theme]);
		fprintf(file, "let s:none = \"none\"\n");
		fprintf(file, "let s:c0    = \"%s\"\n", palettes[theme][0]);
		fprintf(file, "let s:c1    = \"%s\"\n", palettes[theme][1]);
		fprintf(file, "let s:c2    = \"%s\"\n", palettes[theme][2]);
		fprintf(file, "let s:c3    = \"%s\"\n", palettes[theme][3]);
		fprintf(file, "let s:c4    = \"%s\"\n", palettes[theme][4]);
		fprintf(file, "let s:c5    = \"%s\"\n", palettes[theme][5]);
		fprintf(file, "let s:c6    = \"%s\"\n", palettes[theme][6]);
		fprintf(file, "let s:c7    = \"%s\"\n", palettes[theme][7]);
		fprintf(file, "let s:c8    = \"%s\"\n", palettes[theme][8]);
		fprintf(file, "let s:c9    = \"%s\"\n", palettes[theme][9]);
		fprintf(file, "let s:c10   = \"%s\"\n", palettes[theme][10]);
		fprintf(file, "let s:c11   = \"%s\"\n", palettes[theme][11]);
		fprintf(file, "let s:c12   = \"%s\"\n", palettes[theme][12]);
		fprintf(file, "let s:c13   = \"%s\"\n", palettes[theme][13]);
		fprintf(file, "let s:c14   = \"%s\"\n", palettes[theme][14]);
		fprintf(file, "let s:c15   = \"%s\"\n", palettes[theme][15]);
		fprintf(file, "let s:bg    = \"%s\"\n", palettes[theme][256]);
		fprintf(file, "let s:fg    = \"%s\"\n", palettes[theme][257]);
		fprintf(file, "let s:cur   = \"%s\"\n", palettes[theme][258]);
		fputs("\n\
\" Default 16-colors color scheme in vim {{{\n\
exe  \"hi  SpecialKey        gui=none            guifg=\"  .  s:c4    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  NonText           gui=none            guifg=\"  .  s:c12   .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Directory         gui=none            guifg=\"  .  s:c4    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  ErrorMsg          gui=none            guifg=\"  .  s:c15   .  \" guibg=\"  .  s:c1       \n\
exe  \"hi  IncSearch         gui=reverse         guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Search            gui=none            guifg=\"  .  s:c0    .  \" guibg=\"  .  s:c11      \n\
exe  \"hi  MoreMsg           gui=none            guifg=\"  .  s:c2    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  ModeMsg           gui=bold            guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  LineNr            gui=none            guifg=\"  .  s:c3    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  LineNrAbove       gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  LineNrBelow       gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  CursorLineNr      gui=underline       guifg=\"  .  s:c3    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Question          gui=none            guifg=\"  .  s:c2    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  StatusLine        gui=bold,reverse    guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  StatusLineNC      gui=reverse         guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  VertSplit         gui=reverse         guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Title             gui=none            guifg=\"  .  s:c5    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Visual            gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c7       \n\
exe  \"hi  VisualNOS         gui=bold,underline  guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  WarningMsg        gui=none            guifg=\"  .  s:c1    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  WildMenu          gui=none            guifg=\"  .  s:c0    .  \" guibg=\"  .  s:c11      \n\
exe  \"hi  Folded            gui=none            guifg=\"  .  s:c4    .  \" guibg=\"  .  s:c7       \n\
exe  \"hi  FoldColumn        gui=none            guifg=\"  .  s:c4    .  \" guibg=\"  .  s:c7       \n\
exe  \"hi  DiffAdd           gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c12      \n\
exe  \"hi  DiffChange        gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c13      \n\
exe  \"hi  DiffDelete        gui=none            guifg=\"  .  s:c12   .  \" guibg=\"  .  s:c14      \n\
exe  \"hi  DiffText          gui=bold            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c9       \n\
exe  \"hi  SignColumn        gui=none            guifg=\"  .  s:c4    .  \" guibg=\"  .  s:c7       \n\
exe  \"hi  Conceal           gui=none            guifg=\"  .  s:c7    .  \" guibg=\"  .  s:c8       \n\
exe  \"hi  SpellBad          gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c9       \n\
exe  \"hi  SpellCap          gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c12      \n\
exe  \"hi  SpellRare         gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c13      \n\
exe  \"hi  SpellLocal        gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c14      \n\
exe  \"hi  Pmenu             gui=none            guifg=\"  .  s:c0    .  \" guibg=\"  .  s:c13      \n\
exe  \"hi  PmenuSel          gui=none            guifg=\"  .  s:c0    .  \" guibg=\"  .  s:c7       \n\
exe  \"hi  PmenuSbar         gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c7       \n\
exe  \"hi  PmenuThumb        gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c0       \n\
exe  \"hi  TabLine           gui=underline       guifg=\"  .  s:c0    .  \" guibg=\"  .  s:c7       \n\
exe  \"hi  TabLineSel        gui=bold            guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  TabLineFill       gui=reverse         guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  CursorColumn      gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c7       \n\
exe  \"hi  CursorLine        gui=underline       guifg=\"  .  s:none  .  \" guibg=\"  .  s:none     \n\
exe  \"hi  ColorColumn       gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c9       \n\
exe  \"hi  Normal            gui=none            guifg=\"  .  s:fg    .  \" guibg=\"  .  s:bg       \n\
exe  \"hi  StatusLineTerm    gui=bold            guifg=\"  .  s:c15   .  \" guibg=\"  .  s:c2       \n\
exe  \"hi  StatusLineTermNC  gui=none            guifg=\"  .  s:c15   .  \" guibg=\"  .  s:c2       \n\
exe  \"hi  MatchParen        gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c14      \n\
exe  \"hi  ToolbarLine       gui=none            guifg=\"  .  s:none  .  \" guibg=\"  .  s:c7       \n\
exe  \"hi  ToolbarButton     gui=bold            guifg=\"  .  s:c15   .  \" guibg=\"  .  s:c8       \n\
exe  \"hi  Comment           gui=none            guifg=\"  .  s:c4    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Constant          gui=none            guifg=\"  .  s:c1    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Special           gui=none            guifg=\"  .  s:c5    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Identifier        gui=none            guifg=\"  .  s:c6    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Statement         gui=none            guifg=\"  .  s:c3    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  PreProc           gui=none            guifg=\"  .  s:c5    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Type              gui=none            guifg=\"  .  s:c2    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Underlined        gui=underline       guifg=\"  .  s:c5    .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Ignore            gui=none            guifg=\"  .  s:c15   .  \" guibg=\"  .  s:none     \n\
exe  \"hi  Error             gui=none            guifg=\"  .  s:c15   .  \" guibg=\"  .  s:c9       \n\
exe  \"hi  Todo              gui=none            guifg=\"  .  s:c0    .  \" guibg=\"  .  s:c11      \n\
\" }}} \n\n\
exe  \"hi  Folded        gui=none       guifg=\"  .  s:c4    .  \" guibg=\"  .  s:c8\n\
exe  \"hi  Comment       gui=none       guifg=\"  .  s:c4    .  \" guibg=\"  .  s:none\n\
exe  \"hi  Search        gui=none       guifg=\"  .  s:c0    .  \" guibg=\"  .  s:c11\n\
exe  \"hi  CursorColumn  gui=none       guifg=\"  .  s:none  .  \" guibg=\"  .  s:c8\n\
exe  \"hi  CursorLine    gui=none       guifg=\"  .  s:none  .  \" guibg=\"  .  s:c8\n\
exe  \"hi  CursorLineNr  gui=none       guifg=\"  .  s:c7    .  \" guibg=\"  .  s:c8\n\
exe  \"hi  LineNr        gui=none       guifg=\"  .  s:c7    .  \" guibg=\"  .  s:none\n\
exe  \"hi  MatchParen    gui=none       guifg=\"  .  s:none  .  \" guibg=\"  .  s:c7\n\
exe  \"hi  Visual        gui=none       guifg=\"  .  s:none  .  \" guibg=\"  .  s:c7\n\
exe  \"hi  VertSplit     gui=reverse    guifg=\"  .  s:c7    .  \" guibg=\"  .  s:none\n\
exe  \"hi  Pmenu         gui=none       guifg=\"  .  s:none  .  \" guibg=\"  .  s:c8\n\
exe  \"hi  PmenuSel      gui=reverse    guifg=\"  .  s:c7    .  \" guibg=\"  .  s:none\n\
exe  \"hi  PmenuSbar     gui=none       guifg=\"  .  s:c7    .  \" guibg=\"  .  s:c8\n\
exe  \"hi  PmenuThumb    gui=none       guifg=\"  .  s:c7    .  \" guibg=\"  .  s:c7\n\
exe  \"hi  SpellBad      gui=underline  guifg=\"  .  s:c9    .  \" guibg=\"  .  s:none\n\
exe  \"hi  SpellCap      gui=underline  guifg=\"  .  s:c12   .  \" guibg=\"  .  s:none\n\
exe  \"hi  SpellLocal    gui=underline  guifg=\"  .  s:c14   .  \" guibg=\"  .  s:none\n\
exe  \"hi  DiffAdd       gui=none       guifg=\"  .  s:none  .  \" guibg=\"  .  s:c10\n\
exe  \"hi  DiffChange    gui=none       guifg=\"  .  s:none  .  \" guibg=\"  .  s:c11\n\
exe  \"hi  DiffDelete    gui=none       guifg=\"  .  s:none  .  \" guibg=\"  .  s:c9\n\
exe  \"hi  DiffText      gui=none       guifg=\"  .  s:c15   .  \" guibg=\"  .  s:c9\n\
exe  \"hi  TabLine       gui=none       guifg=\"  .  s:c0    .  \" guibg=\"  .  s:c7\n\
exe  \"hi  TabLineFill   gui=none       guifg=\"  .  s:none  .  \" guibg=\"  .  s:c7\n\
exe  \"hi  TabLineSel    gui=bold       guifg=\"  .  s:none  .  \" guibg=\"  .  s:none\n\
", file);

		fclose(file);
	}

	return 0;
}

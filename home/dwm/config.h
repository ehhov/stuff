/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderdef = 0;         /* initial value of border */
static       unsigned int borderpx  = borderdef; /* border pixel of windows */
static const unsigned int borderpxf = 5;         /* floating windows border increment */
static const unsigned int snap      = 10;		     /* snap pixel */
static const unsigned int gapdef    = 5;         /* initial value of gaps */
static       unsigned int gappx     = gapdef;    /* gap pixel between windows */
static const int showsystray        = 1;			   /* 0 means no systray */
static const int showbar            = 1;         /* 0 means no bar */
static const int topbar             = 1;         /* 0 means bottom bar */
static const int title_alignment    = 1;         /* 0 is right, 1 is titleblock centered, 2 is left */
static const int focusonwheel       = 0;         /* 0 means only on click */
static const unsigned int systraypinning = 0;    /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 0;    /* systray spacing */
static const int systraypinningfailfirst = 1;    /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const char dmenufont[]       = "Iosevka:size=10:antialias=true:autohint=true";
static const char *fonts[]          = { dmenufont, "Monospace:size=10", "NotoColorEmoji:size=10" };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char nf[] = "#999999", nb[] = "#101010", nbor[] = "#101010"; /* from dwm site */
static const char sf[] = "#ffffff", sb[] = "#224488", sbor[] = "#224488";
//static const char nf[] = "#333333", nb[] = "#eeeeee", nbor[] = "#444444"; /* my light blue */
//static const char sf[] = "#000066", sb[] = "#ddddff", sbor[] = "#ddddff";
//static const char nf[] = "#839496", nb[] = "#002b36", nbor[] = "#eee8d5"; /* Solzrized dark */
//static const char sf[] = "#839496", sb[] = "#073642", sbor[] = "#eee8d5";
//static const char nf[] = "#657b83", nb[] = "#fdf6e3", nbor[] = "#073642"; /* Solzrized light */
//static const char sf[] = "#657b83", sb[] = "#eee8d5", sbor[] = "#073642";
//static const char nf[] = "#bbbbbb", nb[] = "#222222", nbor[] = "#444444"; /* dwm default */
//static const char sf[] = "#eeeeee", sb[] = "#005577", sbor[] = "#005577";
static const char *colors[][3]      = {
    /*            fg          bg          border */
	[SchemeNorm] = { nf, nb, nbor },
	[SchemeSel]  = { sf, sb, sbor },
};

/* Custom functions */
static void tile_gaps(Monitor *);
static void tile_vert_gaps(Monitor *);
static void tile_vert(Monitor *);
static void tile_deck_gaps(Monitor *);
static void tile_deck(Monitor *);
static void monocle_gaps(Monitor *);
static void reset_layout(const Arg *arg);
static void setgap(const Arg *arg);
static void setborder(const Arg *arg);
static void setclientborder(Client* c);
static void togglefullscreen(const Arg *arg);
static void focusurgent(const Arg *arg);
static void movestack(const Arg *arg);

/* autostart commands */
static const char *const autostart[] = {
	"sh", "-c", "xrdb -load ~/.Xresources", NULL,
	"sh", "-c", "feh --bg-fill ~/.wallpapers/foggy-forest-light.jpg", NULL,
	"sh", "-c", "~/.bin/kb", NULL,
	"dwms", NULL,
	"clipmenud", NULL,
	"dunst", NULL,
	"sh", "-c", "~/.bin/compton", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title              tags mask     isfloating   monitor */
	{ "Firefox",     NULL,       NULL,               1 << 0,      0,           -1 },
	{ "firefox",     NULL,       NULL,               1 << 0,      0,           -1 },
	{ "Thunderbird", NULL,       NULL,               1 << 8,      0,           -1 },
	{ "Klavaro",     NULL,       NULL,                    0,      1,           -1 },
	//{ "Example",   NULL,       NULL,               0b987654321, 1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile_gaps },
	{ "[T]",      tile },
	{ "[V]",      tile_vert_gaps },
	{ "[V]",      tile_vert },
	{ "[D]",      tile_deck_gaps },
	{ "[D]",      tile_deck },
	{ "[F]",      NULL },
	{ "[M]",      monocle_gaps },

	[100] = { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define Win Mod4Mask
#define Alt Mod1Mask
#define Ctrl ControlMask
#define Shift ShiftMask
#define ButtonUp Button4
#define ButtonDown Button5
#define TAGKEYS(KEY,TAG) \
	{ 0, MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ 0, MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ 0, MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ 0, MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/bash", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run",   "-m", dmenumon, "-fn", dmenufont, "-nb", nb, "-nf", nf, "-sb", sb, "-sf", sf, NULL };
static const char *clipmenucmd[] = { "clipmenu", "-m", dmenumon, "-fn", dmenufont, "-nb", nb, "-nf", nf, "-sb", sb, "-sf", sf, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *suspendcmd[] = { "systemctl", "suspend", NULL };

static Key keys[] = {
	/*type modifier                    key        function        argument */
	/* Application launchers */
	{ 0, MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ 0, Alt,                          XK_space,  spawn,          {.v = dmenucmd } },
	{ 0, Win,                          XK_Return, spawn,          {.v = termcmd } },
	{ 0, Win,                          XK_c,      spawn,          {.v = clipmenucmd } },
	{ 0, Win,                          XK_u,      spawn,          SHCMD("urxvt") },
	{ 0, Win,                          XK_x,      spawn,          SHCMD("xterm") },
	{ 0, Win,                          XK_s,      spawn,          SHCMD("st") },
	{ 0, Win,                          XK_g,      spawn,          SHCMD("gvim") },
	{ 0, Win,                          XK_n,      spawn,          SHCMD("nvim-qt") },
	{ 0, Win,                          XK_e,      spawn, 	 	 	 	  SHCMD("thunar") },
	{ 0, Win|Ctrl,                     XK_f,      spawn,          SHCMD("firefox") },
	{ 0, Win|Ctrl,                     XK_c,      spawn,          SHCMD("google-chrome-stable") },
	{ 0, Win|Ctrl,                     XK_s,      spawn,          SHCMD("~/.surf_/surf https://google.com") },
	{ 0, Win|Ctrl,                     XK_k,      spawn,          SHCMD("konsole") },
	{ 0, Win|Ctrl,                     XK_d,      spawn,          SHCMD("dolphin") },
	{ 0, Win|Ctrl,                     XK_o,      spawn,          SHCMD("okular") },
	{ 0, Win|Ctrl,                     XK_p,      spawn,          SHCMD("qpdfview") },
	{ 0, Win|Ctrl,                     XK_g,      spawn,          SHCMD("gimp") },
	{ 0, Win|Ctrl,                     XK_i,      spawn,          SHCMD("inkscape") },
	{ 0, Win|Ctrl,                     XK_x,      spawn,          SHCMD("texstudio") },
	{ 0, Win|Ctrl,                     XK_t,      spawn,          SHCMD("telegram-desktop") },
	{ 0, Win|Ctrl,                     XK_m,      spawn,          SHCMD("thunderbird") },
	{ 0, Win|Ctrl,                     XK_l,      spawn,          SHCMD("libreoffice") },
	{ 0, Win|Ctrl,                     XK_v,      spawn,          SHCMD("pavucontrol") },
	
	/* Window management */
	{ 0, MODKEY,                       XK_b,      togglebar,      {0} },
	{ 0, MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ 0, MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ 0, MODKEY|Shift,                 XK_j,      movestack,      {.i = +1 } },
	{ 0, MODKEY|Shift,                 XK_k,      movestack,      {.i = -1 } },
	{ 0, Alt,                          XK_Tab,    focusstack,     {.i = +1 } },
	{ 0, Alt|Shift,                    XK_Tab,    focusstack,     {.i = -1 } },
	{ 0, MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ 0, MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ 0, MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ 0, MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ 0, Alt,                          XK_Return, zoom,           {0} },
	{ 0, Win|Shift,                    XK_u,      focusurgent,    {0} },
	{ 0, MODKEY,                       XK_Tab,    view,           {0} },
	{ 0, MODKEY|Shift,                 XK_c,      killclient,     {0} },
	{ 0, MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ 0, MODKEY|Shift,                 XK_t,      setlayout,      {.v = &layouts[1]} },
	{ 0, MODKEY,                       XK_v,      setlayout,      {.v = &layouts[2]} },
	{ 0, MODKEY|Shift,                 XK_v,      setlayout,      {.v = &layouts[3]} },
	{ 0, MODKEY,                       XK_a,      setlayout,      {.v = &layouts[4]} },
	{ 0, MODKEY|Shift,                 XK_a,      setlayout,      {.v = &layouts[5]} },
	{ 0, MODKEY,                       XK_f,      setlayout,      {.v = &layouts[6]} },
	{ 0, MODKEY,                       XK_m,      setlayout,      {.v = &layouts[7]} },
	{ 0, MODKEY|Shift,                 XK_m,      setlayout,      {.v = &layouts[100]} },
	{ 0, MODKEY|Shift,                 XK_f,      togglefullscreen,{0} },
	{ 0, Win,                          XK_equal,  setgap,         {.i = 1} },
	{ 0, Win,                          XK_minus,  setgap,         {.i = -1} },
	{ 0, Win|Shift,                    XK_equal,  setborder,      {.i = 1} },
	{ 0, Win|Shift,                    XK_minus,  setborder,      {.i = -1} },
	{ 0, Alt|Ctrl,                     XK_space,  reset_layout,   {.i = 1} },
	{ 0, Alt|Shift,                    XK_space,  togglefloating, {0} },
	{ 0, MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ 0, MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ 0, MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ 0, MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ 0, MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ 0, MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                           XK_1,                      0)
	TAGKEYS(                           XK_2,                      1)
	TAGKEYS(                           XK_3,                      2)
	TAGKEYS(                           XK_4,                      3)
	TAGKEYS(                           XK_5,                      4)
	TAGKEYS(                           XK_6,                      5)
	TAGKEYS(                           XK_7,                      6)
	TAGKEYS(                           XK_8,                      7)
	TAGKEYS(                           XK_9,                      8)

	/* External monitor configuration */
	{ 0, Win|Alt,                      XK_d,      spawn,          SHCMD("~/.bin/monitor dis") },
	{ 0, Win|Alt,                      XK_e,      spawn,          SHCMD("~/.bin/monitor ext") },
	{ 0, Win|Alt,                      XK_s,      spawn,          SHCMD("~/.bin/monitor split") },
	{ 0, Win|Alt,                      XK_b,      spawn,          SHCMD("~/.bin/monitor dup") },

  { 0, Win|Alt,                      XK_space,  spawn,          SHCMD("~/.bin/kb") },
	
	/* Exit commands */
	{ 0, MODKEY|ShiftMask,             XK_Escape, quit,           {0} },
	{ 0, Win|Shift|Ctrl,               XK_s,      spawn,          {.v = suspendcmd} },

	/* Screenshots */
	{ 0, 0,                            XK_Print,  spawn,          SHCMD("~/.bin/print screen") },
	{ 0, Shift,                        XK_Print,  spawn,          SHCMD("~/.bin/print delayed") },
	{ 1, Win|Shift,                    XK_s,      spawn,          SHCMD("~/.bin/print region") },
	{ 0, Win|Shift,                    XK_w,      spawn,          SHCMD("~/.bin/print import") },
	{ 0, Win|Shift,                    XK_q,      spawn,          SHCMD("~/.bin/print quit") },
	{ 0, Win,                          XK_Print,  spawn,          SHCMD("spectacle") },

	/* XF86XK_Keys */
	{ 0, 0,   XF86XK_AudioRaiseVolume,  spawn, SHCMD("~/.bin/volume up") },
	{ 0, 0,   XF86XK_AudioLowerVolume,  spawn, SHCMD("~/.bin/volume down") },
	{ 0, 0,   XF86XK_AudioMute,         spawn, SHCMD("~/.bin/volume toggle") },
	{ 0, Win, XF86XK_AudioMute,         spawn, SHCMD("~/.bin/volume mute") },
	{ 0, Win, XF86XK_AudioRaiseVolume,  spawn, SHCMD("~/.bin/volume bigup") },
	{ 0, Win, XF86XK_AudioLowerVolume,  spawn, SHCMD("~/.bin/volume bigdown") },
	{ 0, 0,   XF86XK_MonBrightnessUp,   spawn, SHCMD("~/.bin/brightness up") },
	{ 0, 0,   XF86XK_MonBrightnessDown, spawn, SHCMD("~/.bin/brightness down") },
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[100]} },
	{ ClkWinTitle,          0,              Button1,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },

	{ ClkClientWin,         MODKEY|Shift,   Button2,        killclient,     {0} },
	{ ClkClientWin,         MODKEY|Shift,   Button1,        zoom,           {0} },
	{ ClkWinTitle,          0,              Button2,        killclient,     {0} },
	/* brightness and volume control using bar */
	{ ClkWinTitle,          0,              ButtonUp,       spawn,          SHCMD("~/.bin/brightness down") },
	{ ClkWinTitle,          0,              ButtonDown,     spawn,          SHCMD("~/.bin/brightness up") },
	{ ClkStatusText,        0,              Button1,        spawn,          SHCMD("~/.bin/volume toggle") },
	{ ClkStatusText,        0,              ButtonUp,       spawn,          SHCMD("~/.bin/volume up") },
	{ ClkStatusText,        0,              ButtonDown,     spawn,          SHCMD("~/.bin/volume down") },
};


/* Custom functions */
void
tile_gaps (Monitor *m)
{
	unsigned int i, n, h, mw, my, ty, ns;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster) {
		mw = m->nmaster ? m->ww * m->mfact : 0;
		ns = m->nmaster > 0 ? 2 : 1;
	} else {
		mw = m->ww;
		ns = 1;
	}
	for (i = 0, my = ty = gappx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i) - gappx;
			resize(c, m->wx + gappx, m->wy + my, mw - (2*c->bw) - gappx*(5-ns)/2, h - (2*c->bw), 0);
			if (my + HEIGHT(c) + gappx < m->wh)
				my += HEIGHT(c) + gappx;
		} else {
			h = (m->wh - ty) / (n - i) - gappx;
			resize(c, m->wx + mw + gappx/ns, m->wy + ty, m->ww - mw - (2*c->bw) - gappx*(5-ns)/2, h - (2*c->bw), 0);
			if (ty + HEIGHT(c) + gappx < m->wh)
				ty += HEIGHT(c) + gappx;
		}
}

void
tile_vert_gaps (Monitor *m)
{
	unsigned int i, n, w, mh, mx, tx, ns;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster) {
		mh = m->nmaster ? m->wh * m->mfact : 0;
		ns = m->nmaster > 0 ? 2 : 1;
	} else {
		mh = m->wh;
		ns = 1;
	}
	for (i = 0, mx = tx = gappx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i) - gappx;
			resize(c, m->wx + mx, m->wy + gappx, w - (2*c->bw), mh - (2*c->bw) - gappx*(5-ns)/2, 0);
			if (mx + WIDTH(c) + gappx < m->ww)
				mx += WIDTH(c) + gappx;
		} else {
			w = (m->ww - tx) / (n - i) - gappx;
			resize(c, m->wx + tx, m->wy + mh + gappx/ns, w - (2*c->bw), m->wh - mh - (2*c->bw) - gappx*(5-ns)/2, 0);
			if (tx + WIDTH(c) + gappx < m->ww)
				tx += WIDTH(c) + gappx;
		}
}

void
tile_vert (Monitor *m)
{
	unsigned int i, n, w, mh, mx, tx;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster)
		mh = m->nmaster ? m->wh * m->mfact : 0;
	else
		mh = m->wh;
	for (i = 0, mx = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx + mx, m->wy, w - (2*c->bw), mh - (2*c->bw), 0);
			if (mx + WIDTH(c) < m->ww)
				mx += WIDTH(c);
		} else {
			w = (m->ww - tx) / (n - i);
			resize(c, m->wx + tx, m->wy + mh, w - (2*c->bw), m->wh - mh - (2*c->bw), 0);
			if (tx + WIDTH(c) < m->ww)
				tx += WIDTH(c);
		}
}

void
tile_deck_gaps (Monitor *m)
{
	unsigned int i, n, h, mw, my, ns;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if(n == 0)
		return;

	if(n > m->nmaster) {
		mw = m->nmaster ? m->ww * m->mfact : 0;
		ns = m->nmaster > 0 ? 2 : 1;
		if (n > m->nmaster + 1) snprintf(m->ltsymbol, sizeof(m->ltsymbol), "[%d]", n - m->nmaster);
	}
	else {
		mw = m->ww;
		ns = 1;
	}
	for(i = 0, my = gappx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if(i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i) - gappx;
			resize(c, m->wx + gappx, m->wy + my, mw - (2*c->bw) - gappx*(5-ns)/2, h - (2*c->bw), 0);
			if (my + HEIGHT(c) + gappx < m->wh)
				my += HEIGHT(c) + gappx;
		}
		else
			resize(c, m->wx + mw + gappx/ns, m->wy + gappx, m->ww - mw - (2*c->bw) - gappx*(5-ns)/2, m->wh - (2*c->bw), 0);
}

void
tile_deck (Monitor *m)
{
	unsigned int i, n, h, mw, my;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if(n == 0)
		return;

	if(n > m->nmaster) {
		mw = m->nmaster ? m->ww * m->mfact : 0;
		if (n > m->nmaster + 1) snprintf(m->ltsymbol, sizeof(m->ltsymbol), "[%d]", n - m->nmaster);
	}
	else
		mw = m->ww;
	for(i = my = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if(i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
			if (my + HEIGHT(c) < m->wh)
				my += HEIGHT(c);
		}
		else
			resize(c, m->wx + mw, m->wy, m->ww - mw - (2*c->bw), m->wh - (2*c->bw), 0);
}

void
monocle_gaps (Monitor *m)
{
	unsigned int n = 0;
	unsigned int gap = gappx;
	Client *c;

	for (c = m->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;
	if (n > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof(m->ltsymbol), "[%d]", n);
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx + gap, m->wy + gap, m->ww - 2 * c->bw - 2*gap, m->wh - 2 * c->bw - 2*gap, 0);
}

static void
focusurgent(const Arg *arg)
{
	Client *c;
	int i;
	for(c=selmon->clients; c && !c->isurgent; c=c->next);
	if(c) {
		for(i=0; i < LENGTH(tags) && !((1 << i) & c->tags); i++);
		if(i < LENGTH(tags)) {
			const Arg a = {.ui = 1 << i};
			view(&a);
			focus(c);
		}
	}
}

void
movestack(const Arg *arg) {
	Client *c = NULL, *p = NULL, *pc = NULL, *i;

	if(arg->i > 0) {
		/* find the client after selmon->sel */
		for(c = selmon->sel->next; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);
		if(!c)
			for(c = selmon->clients; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);

	}
	else {
		/* find the client before selmon->sel */
		for(i = selmon->clients; i != selmon->sel; i = i->next)
			if(ISVISIBLE(i) && !i->isfloating)
				c = i;
		if(!c)
			for(; i; i = i->next)
				if(ISVISIBLE(i) && !i->isfloating)
					c = i;
	}
	/* find the client before selmon->sel and c */
	for(i = selmon->clients; i && (!p || !pc); i = i->next) {
		if(i->next == selmon->sel)
			p = i;
		if(i->next == c)
			pc = i;
	}

	/* swap c and selmon->sel selmon->clients in the selmon->clients list */
	if(c && c != selmon->sel) {
		Client *temp = selmon->sel->next==c?selmon->sel:selmon->sel->next;
		selmon->sel->next = c->next==selmon->sel?c:c->next;
		c->next = temp;

		if(p && p != c)
			p->next = c;
		if(pc && pc != selmon->sel)
			pc->next = selmon->sel;

		if(selmon->sel == selmon->clients)
			selmon->clients = c;
		else if(c == selmon->clients)
			selmon->clients = selmon->sel;

		arrange(selmon);
	}
}

void
togglefullscreen(const Arg *arg)
{
	if(selmon->sel)
		setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}

void
setgap(const Arg *arg)
{
	gappx = (int)gappx + arg->i<0? 0 : gappx + arg->i;
	arrange(selmon);
}

void
setborder(const Arg *arg)
{
	Monitor *m;
	Client *c;

	if (arg!=NULL)
		borderpx = (int)borderpx + arg->i<0? 0 : borderpx + arg->i;
	for(m = mons; m; m = m->next)
		for(c = m->clients; c; c = c->next)
			setclientborder(c);
	arrange(selmon);
}

void
setclientborder(Client* c)
{
	c->bw = borderpx + borderpxf * c->isfloating * (1 - c->isfullscreen);
	XWindowChanges wc;
	wc.border_width = c->bw;
	XConfigureWindow(dpy, c->win, CWBorderWidth, &wc);
}

void
reset_layout(const Arg *arg)
{
	Arg a;
	a.v = &layouts[0];
	setlayout(&a);
	a.f = 1.0 + mfact;
	setmfact(&a);
	a.i = 100 + nmaster;
	incnmaster(&a);
	if(arg) {
		a.i = (int)gapdef - (int)gappx;
		setgap(&a);
		a.i = (int)borderdef - (int)borderpx;
		setborder(&a);
	}
}

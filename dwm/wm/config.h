/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderdef = 0;         /* initial value of border */
static       unsigned int borderpx  = borderdef; /* border pixel of windows */
static const unsigned int borderpxf = 5;         /* floating windows border increment */
static const unsigned int snap      = 10;		     /* snap pixel */
static const unsigned int gapdef    = 10;        /* initial value of gaps */
static       unsigned int gappx     = gapdef;    /* gap pixel between windows */
static const int showsystray        = 1;			   /* 0 means no systray */
static const int showbar            = 1;         /* 0 means no bar */
static const int topbar             = 0;         /* 0 means bottom bar */
static const int title_alignment    = 1;         /* 0 is right, 1 is titleblock centered, 2 is left */
static const int focusonwheel       = 0;         /* 0 means only on click */
static const unsigned int systraypinning = 0;    /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 0;    /* systray spacing */
static const int systraypinningfailfirst = 1;    /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const char dmenufont[]       = "Iosevka:size=10";
static const char *fonts[]          = { dmenufont, "DejaVuSans:size=10" };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char nb[]              = "#000000";
static const char nf[]              = "#cccccc";
static const char sb[]              = "#224488"; //"#005577";
static const char sf[]              = "#ffffff";
static const char nbor[]            = "#444444";
static const char sbor[]            = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { nf,        nb,        nbor      },
	[SchemeSel]  = { sf,        sb,        sbor      },
};

/* Custom functions */
static void tile_gaps(Monitor *);
static void tile_vert_gaps(Monitor *);
static void tile_vert(Monitor *);
static void monocle_gaps(Monitor *);
static void reset_layout(const Arg *arg);
static void setgap(const Arg *arg);
static void setborder(const Arg *arg);
static void setclientborder(Client* c);

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title              tags mask     isfloating   monitor */
	{ "plasmashell", NULL,       NULL,               0b111111111, 1,           -1 },
	{ NULL,          NULL,       "Desktop — Plasma", 1 << 8,      1,           -1 },
	//{ "Example",   NULL,       NULL,               0b123456789, 1,           -1 },
	//{ "Firefox",   NULL,       NULL,               1 << 8,      0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile_gaps },  /* 0 first entry is default */
	{ "[T]",      tile },       /* 1 */
	{ "[V]",      tile_vert_gaps }, /* 2 */
	{ "[V]",      tile_vert },  /* 3 */
	{ "[F]",      NULL },		    /* 4 no layout function means floating behavior */
	{ "[M]",      monocle_gaps }, /* 5 */
	{ "[M]",      monocle },    /* 6 */
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
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", nb, "-nf", nf, "-sb", sb, "-sf", sf, NULL };
static const char *termcmd[]  = { "xterm", NULL };
static const char *suspendcmd[] = { "systemctl", "suspend", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	/* Application launchers */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ Alt,                          XK_space,  spawn,          {.v = dmenucmd } },
	{ Win,                          XK_Return, spawn,          {.v = termcmd } },
	{ Win,                          XK_u,      spawn,          SHCMD("urxvt") },
	{ Win,                          XK_x,      spawn,          SHCMD("xterm") },
	{ Win,                          XK_s,      spawn,          SHCMD("~/.st/st") },
	{ Win,                          XK_e,      spawn,					 SHCMD("emacs") },
	{ Win|Ctrl,                     XK_f,      spawn,          SHCMD("firefox") },
	{ Win|Ctrl,                     XK_c,      spawn,          SHCMD("google-chrome") },
	{ Win|Ctrl,                     XK_s,      spawn,          SHCMD("surf https://google.com") },
	{ Win|Ctrl,                     XK_o,      spawn,          SHCMD("okular") },
	{ Win|Ctrl,                     XK_d,      spawn,          SHCMD("QT_QPA_PLATFORMTHEME=kde dolphin") },
	{ Win|Ctrl,                     XK_g,      spawn,          SHCMD("gimp") },
	{ Win|Ctrl,                     XK_i,      spawn,          SHCMD("inkscape") },
	{ Win|Ctrl,                     XK_x,      spawn,          SHCMD("texstudio") },
	{ Win|Ctrl,                     XK_t,      spawn,          SHCMD("/opt/tg/app") },
	{ Win|Ctrl,                     XK_m,      spawn,          SHCMD("thunderbird") },
	
	/* External monitor configuration */
	{ Win|Alt,                      XK_d,      spawn,          SHCMD("~/.bin/monitor dis") },
	{ Win|Alt,                      XK_e,      spawn,          SHCMD("~/.bin/monitor ext") },
	{ Win|Alt,                      XK_s,      spawn,          SHCMD("~/.bin/monitor split") },
	{ Win|Alt,                      XK_b,      spawn,          SHCMD("~/.bin/monitor dup") },

  { Win|Alt,                      XK_space,  spawn,          SHCMD("~/.bin/kb") },
	
	/* Window management */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ Alt,                          XK_Tab,    focusstack,     {.i = +1 } },
	{ Alt|Shift,                    XK_Tab,    focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ Win|Shift,                    XK_Return, zoom,           {0} },
	{ Alt,                          XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|Shift,                 XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_v,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|Shift,                 XK_v,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|Shift,                 XK_m,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY|Shift,                 XK_i,      setgap,         {.i = 1} },
	{ MODKEY|Shift,                 XK_d,      setgap,         {.i = -1} },
	{ Alt|Shift,                    XK_i,      setborder,      {.i = 1} },
	{ Alt|Shift,                    XK_d,      setborder,      {.i = -1} },
	{ Alt|Ctrl,                     XK_space,  reset_layout,   {0} },
	{ Alt|Shift,                    XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	/* Exit commands */
	{ MODKEY|ShiftMask,             XK_Escape, quit,           {0} },
	//{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ Win|Shift|Ctrl,               XK_s,      spawn,          {.v = suspendcmd} },

	/* Screenshots */
	{ 0,         XK_Print, spawn, SHCMD("~/.bin/print screen") },
	{ Win|Shift, XK_s,     spawn, SHCMD("~/.bin/print rect") },
	{ Win|Shift, XK_w,     spawn, SHCMD("~/.bin/print win") },
	{ Win|Shift, XK_q,     spawn, SHCMD("~/.bin/print quit") },
	{ Win,       XK_Print, spawn, SHCMD("spectacle") },
	{ Shift,     XK_Print, spawn, SHCMD("spectacle -r") },

	/* XF86XK_Keys */
	{ 0,   XF86XK_AudioRaiseVolume,  spawn, SHCMD("~/.bin/volume up") },
	{ 0,   XF86XK_AudioLowerVolume,  spawn, SHCMD("~/.bin/volume down") },
	{ 0,   XF86XK_AudioMute,         spawn, SHCMD("~/.bin/volume toggle") },
	{ Win, XF86XK_AudioMute,         spawn, SHCMD("~/.bin/volume mute") },
	{ Win, XF86XK_AudioRaiseVolume,  spawn, SHCMD("~/.bin/volume bigup") },
	{ Win, XF86XK_AudioLowerVolume,  spawn, SHCMD("~/.bin/volume bigdown") },
	{ 0,   XF86XK_MonBrightnessUp,   spawn, SHCMD("~/.bin/brightness up") },
	{ 0,   XF86XK_MonBrightnessDown, spawn, SHCMD("~/.bin/brightness down") }, 
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[6]} },
	{ ClkWinTitle,          0,              Button1,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },

	//{ ClkRootWin,           0,              Button1,        spawn,          {.v = termcmd } },
	//{ ClkRootWin,           0,              Button3,        spawn,          {.v = dmenucmd } },
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
	unsigned int gap = gappx;
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
	for (i = 0, my = ty = gap, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i) - gap;
			resize(c, m->wx + gap, m->wy + my, mw - (2*c->bw) - gap*(5-ns)/2, h - (2*c->bw), 0);
			my += HEIGHT(c) + gap;
		} else {
			h = (m->wh - ty) / (n - i) - gap;
			resize(c, m->wx + mw + gap/ns, m->wy + ty, m->ww - mw - (2*c->bw) - gap*(5-ns)/2, h - (2*c->bw), 0);
			ty += HEIGHT(c) + gap;
		}
}

void
tile_vert_gaps (Monitor *m)
{
	unsigned int i, n, w, mh, mx, tx, ns;
	unsigned int gap = gappx;
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
	for (i = 0, mx = tx = gap, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i) - gap;
			resize(c, m->wx + mx, m->wy + gap, w - (2*c->bw), mh - (2*c->bw) - gap*(5-ns)/2, 0);
			mx += WIDTH(c) + gap;
		} else {
			w = (m->ww - tx) / (n - i) - gap;
			resize(c, m->wx + tx, m->wy + mh + gap/ns, w - (2*c->bw), m->wh - mh - (2*c->bw) - gap*(5-ns)/2, 0);
			tx += WIDTH(c) + gap;
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
			mx += WIDTH(c);
		} else {
			w = (m->ww - tx) / (n - i);
			resize(c, m->wx + tx, m->wy + mh, w - (2*c->bw), m->wh - mh - (2*c->bw), 0);
			tx += WIDTH(c);
		}
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
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx + gap, m->wy + gap, m->ww - 2 * c->bw - 2*gap, m->wh - 2 * c->bw - 2*gap, 0);
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
	c->bw = borderpx + c->isfloating*borderpxf;
}

void 
reset_layout(const Arg *arg)
{
	Arg a;
	a.f = 1.0 + mfact;
	setmfact(&a);
	a.i = 100 + nmaster;
	incnmaster(&a);
	a.i = (int)gapdef - (int)gappx;
	setgap(&a);
	a.i = (int)borderdef - (int)borderpx;
	setborder(&a);
	a.v = &layouts[0];
	setlayout(&a);
}

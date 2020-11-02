hi clear Normal
set bg&
hi clear
if exists("syntax_on")
	syntax reset
endif

let g:colors_name = "Dracula"

let s:none = "none"
let s:c0    = "#000000"
let s:c1    = "#ff5555"
let s:c2    = "#50fa7b"
let s:c3    = "#FFB366"
let s:c4    = "#bd93f9"
let s:c5    = "#ff79c6"
let s:c6    = "#8be9fd"
let s:c7    = "#8B8BA7"
let s:c8    = "#34344B"
let s:c9    = "#FF4444"
let s:c10   = "#3DF56B"
let s:c11   = "#f1fa8c"
let s:c12   = "#B78CF2"
let s:c13   = "#F881C5"
let s:c14   = "#6EDEF7"
let s:c15   = "#ffffff"
let s:bg    = "#282a36"
let s:fg    = "#d8d8d2"
let s:cur   = "#d8d8d2"

" Default 16-colors color scheme in vim {{{
exe  "hi  SpecialKey        gui=none            guifg="  .  s:c4    .  " guibg="  .  s:none     
exe  "hi  NonText           gui=none            guifg="  .  s:c12   .  " guibg="  .  s:none     
exe  "hi  Directory         gui=none            guifg="  .  s:c4    .  " guibg="  .  s:none     
exe  "hi  ErrorMsg          gui=none            guifg="  .  s:c15   .  " guibg="  .  s:c1       
exe  "hi  IncSearch         gui=reverse         guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  Search            gui=none            guifg="  .  s:c0    .  " guibg="  .  s:c11      
exe  "hi  MoreMsg           gui=none            guifg="  .  s:c2    .  " guibg="  .  s:none     
exe  "hi  ModeMsg           gui=bold            guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  LineNr            gui=none            guifg="  .  s:c3    .  " guibg="  .  s:none     
exe  "hi  LineNrAbove       gui=none            guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  LineNrBelow       gui=none            guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  CursorLineNr      gui=underline       guifg="  .  s:c3    .  " guibg="  .  s:none     
exe  "hi  Question          gui=none            guifg="  .  s:c2    .  " guibg="  .  s:none     
exe  "hi  StatusLine        gui=bold,reverse    guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  StatusLineNC      gui=reverse         guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  VertSplit         gui=reverse         guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  Title             gui=none            guifg="  .  s:c5    .  " guibg="  .  s:none     
exe  "hi  Visual            gui=none            guifg="  .  s:none  .  " guibg="  .  s:c7       
exe  "hi  VisualNOS         gui=bold,underline  guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  WarningMsg        gui=none            guifg="  .  s:c1    .  " guibg="  .  s:none     
exe  "hi  WildMenu          gui=none            guifg="  .  s:c0    .  " guibg="  .  s:c11      
exe  "hi  Folded            gui=none            guifg="  .  s:c4    .  " guibg="  .  s:c7       
exe  "hi  FoldColumn        gui=none            guifg="  .  s:c4    .  " guibg="  .  s:c7       
exe  "hi  DiffAdd           gui=none            guifg="  .  s:none  .  " guibg="  .  s:c12      
exe  "hi  DiffChange        gui=none            guifg="  .  s:none  .  " guibg="  .  s:c13      
exe  "hi  DiffDelete        gui=none            guifg="  .  s:c12   .  " guibg="  .  s:c14      
exe  "hi  DiffText          gui=bold            guifg="  .  s:none  .  " guibg="  .  s:c9       
exe  "hi  SignColumn        gui=none            guifg="  .  s:c4    .  " guibg="  .  s:c7       
exe  "hi  Conceal           gui=none            guifg="  .  s:c7    .  " guibg="  .  s:c8       
exe  "hi  SpellBad          gui=none            guifg="  .  s:none  .  " guibg="  .  s:c9       
exe  "hi  SpellCap          gui=none            guifg="  .  s:none  .  " guibg="  .  s:c12      
exe  "hi  SpellRare         gui=none            guifg="  .  s:none  .  " guibg="  .  s:c13      
exe  "hi  SpellLocal        gui=none            guifg="  .  s:none  .  " guibg="  .  s:c14      
exe  "hi  Pmenu             gui=none            guifg="  .  s:c0    .  " guibg="  .  s:c13      
exe  "hi  PmenuSel          gui=none            guifg="  .  s:c0    .  " guibg="  .  s:c7       
exe  "hi  PmenuSbar         gui=none            guifg="  .  s:none  .  " guibg="  .  s:c7       
exe  "hi  PmenuThumb        gui=none            guifg="  .  s:none  .  " guibg="  .  s:c0       
exe  "hi  TabLine           gui=underline       guifg="  .  s:c0    .  " guibg="  .  s:c7       
exe  "hi  TabLineSel        gui=bold            guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  TabLineFill       gui=reverse         guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  CursorColumn      gui=none            guifg="  .  s:none  .  " guibg="  .  s:c7       
exe  "hi  CursorLine        gui=underline       guifg="  .  s:none  .  " guibg="  .  s:none     
exe  "hi  ColorColumn       gui=none            guifg="  .  s:none  .  " guibg="  .  s:c9       
exe  "hi  Normal            gui=none            guifg="  .  s:fg    .  " guibg="  .  s:bg       
exe  "hi  StatusLineTerm    gui=bold            guifg="  .  s:c15   .  " guibg="  .  s:c2       
exe  "hi  StatusLineTermNC  gui=none            guifg="  .  s:c15   .  " guibg="  .  s:c2       
exe  "hi  MatchParen        gui=none            guifg="  .  s:none  .  " guibg="  .  s:c14      
exe  "hi  ToolbarLine       gui=none            guifg="  .  s:none  .  " guibg="  .  s:c7       
exe  "hi  ToolbarButton     gui=bold            guifg="  .  s:c15   .  " guibg="  .  s:c8       
exe  "hi  Comment           gui=none            guifg="  .  s:c4    .  " guibg="  .  s:none     
exe  "hi  Constant          gui=none            guifg="  .  s:c1    .  " guibg="  .  s:none     
exe  "hi  Special           gui=none            guifg="  .  s:c5    .  " guibg="  .  s:none     
exe  "hi  Identifier        gui=none            guifg="  .  s:c6    .  " guibg="  .  s:none     
exe  "hi  Statement         gui=none            guifg="  .  s:c3    .  " guibg="  .  s:none     
exe  "hi  PreProc           gui=none            guifg="  .  s:c5    .  " guibg="  .  s:none     
exe  "hi  Type              gui=none            guifg="  .  s:c2    .  " guibg="  .  s:none     
exe  "hi  Underlined        gui=underline       guifg="  .  s:c5    .  " guibg="  .  s:none     
exe  "hi  Ignore            gui=none            guifg="  .  s:c15   .  " guibg="  .  s:none     
exe  "hi  Error             gui=none            guifg="  .  s:c15   .  " guibg="  .  s:c9       
exe  "hi  Todo              gui=none            guifg="  .  s:c0    .  " guibg="  .  s:c11      
" }}} 

exe  "hi  Folded        gui=none       guifg="  .  s:c4    .  " guibg="  .  s:c8
exe  "hi  Comment       gui=none       guifg="  .  s:c4    .  " guibg="  .  s:none
exe  "hi  Search        gui=none       guifg="  .  s:c0    .  " guibg="  .  s:c11
exe  "hi  CursorColumn  gui=none       guifg="  .  s:none  .  " guibg="  .  s:c8
exe  "hi  CursorLine    gui=none       guifg="  .  s:none  .  " guibg="  .  s:c8
exe  "hi  CursorLineNr  gui=none       guifg="  .  s:c7    .  " guibg="  .  s:c8
exe  "hi  LineNr        gui=none       guifg="  .  s:c7    .  " guibg="  .  s:none
exe  "hi  MatchParen    gui=none       guifg="  .  s:none  .  " guibg="  .  s:c7
exe  "hi  Visual        gui=none       guifg="  .  s:none  .  " guibg="  .  s:c7
exe  "hi  VertSplit     gui=reverse    guifg="  .  s:c7    .  " guibg="  .  s:none
exe  "hi  Pmenu         gui=none       guifg="  .  s:none  .  " guibg="  .  s:c8
exe  "hi  PmenuSel      gui=reverse    guifg="  .  s:c7    .  " guibg="  .  s:none
exe  "hi  PmenuSbar     gui=none       guifg="  .  s:c7    .  " guibg="  .  s:c8
exe  "hi  PmenuThumb    gui=none       guifg="  .  s:c7    .  " guibg="  .  s:c7
exe  "hi  SpellBad      gui=underline  guifg="  .  s:c9    .  " guibg="  .  s:none
exe  "hi  SpellCap      gui=underline  guifg="  .  s:c12   .  " guibg="  .  s:none
exe  "hi  SpellLocal    gui=underline  guifg="  .  s:c14   .  " guibg="  .  s:none
exe  "hi  DiffAdd       gui=none       guifg="  .  s:none  .  " guibg="  .  s:c10
exe  "hi  DiffChange    gui=none       guifg="  .  s:none  .  " guibg="  .  s:c11
exe  "hi  DiffDelete    gui=none       guifg="  .  s:none  .  " guibg="  .  s:c9
exe  "hi  DiffText      gui=none       guifg="  .  s:c15   .  " guibg="  .  s:c9
exe  "hi  TabLine       gui=none       guifg="  .  s:c0    .  " guibg="  .  s:c7
exe  "hi  TabLineFill   gui=none       guifg="  .  s:none  .  " guibg="  .  s:c7
exe  "hi  TabLineSel    gui=bold       guifg="  .  s:none  .  " guibg="  .  s:none

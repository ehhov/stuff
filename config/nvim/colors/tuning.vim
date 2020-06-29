" from colors/default.vim
hi clear Normal
set bg&
hi clear
if exists("syntax_on")
  syntax reset
endif

" Default 16-colors color scheme in vim {{{ 
hi  SpecialKey        cterm=none            ctermfg=4     ctermbg=none  
hi  NonText           cterm=none            ctermfg=12    ctermbg=none  
hi  Directory         cterm=none            ctermfg=4     ctermbg=none  
hi  ErrorMsg          cterm=none            ctermfg=15    ctermbg=1     
hi  IncSearch         cterm=reverse         ctermfg=none  ctermbg=none  
hi  Search            cterm=none            ctermfg=0     ctermbg=11
hi  MoreMsg           cterm=none            ctermfg=2     ctermbg=none  
hi  ModeMsg           cterm=bold            ctermfg=none  ctermbg=none  
hi  LineNr            cterm=none            ctermfg=3     ctermbg=none  
hi  LineNrAbove       cterm=none            ctermfg=none  ctermbg=none  
hi  LineNrBelow       cterm=none            ctermfg=none  ctermbg=none  
hi  CursorLineNr      cterm=underline       ctermfg=3     ctermbg=none  
hi  Question          cterm=none            ctermfg=2     ctermbg=none  
hi  StatusLine        cterm=bold,reverse    ctermfg=none  ctermbg=none  
hi  StatusLineNC      cterm=reverse         ctermfg=none  ctermbg=none  
hi  VertSplit         cterm=reverse         ctermfg=none  ctermbg=none  
hi  Title             cterm=none            ctermfg=5     ctermbg=none  
hi  Visual            cterm=none            ctermfg=none  ctermbg=7     
hi  VisualNOS         cterm=bold,underline  ctermfg=none  ctermbg=none  
hi  WarningMsg        cterm=none            ctermfg=1     ctermbg=none  
hi  WildMenu          cterm=none            ctermfg=0     ctermbg=11    
hi  Folded            cterm=none            ctermfg=4     ctermbg=7     
hi  FoldColumn        cterm=none            ctermfg=4     ctermbg=7     
hi  DiffAdd           cterm=none            ctermfg=none  ctermbg=12    
hi  DiffChange        cterm=none            ctermfg=none  ctermbg=13    
hi  DiffDelete        cterm=none            ctermfg=12    ctermbg=14    
hi  DiffText          cterm=bold            ctermfg=none  ctermbg=9     
hi  SignColumn        cterm=none            ctermfg=4     ctermbg=7     
hi  Conceal           cterm=none            ctermfg=7     ctermbg=8     
hi  SpellBad          cterm=none            ctermfg=none  ctermbg=9     
hi  SpellCap          cterm=none            ctermfg=none  ctermbg=12    
hi  SpellRare         cterm=none            ctermfg=none  ctermbg=13    
hi  SpellLocal        cterm=none            ctermfg=none  ctermbg=14    
hi  Pmenu             cterm=none            ctermfg=0     ctermbg=13    
hi  PmenuSel          cterm=none            ctermfg=0     ctermbg=7     
hi  PmenuSbar         cterm=none            ctermfg=none  ctermbg=7     
hi  PmenuThumb        cterm=none            ctermfg=none  ctermbg=0     
hi  TabLine           cterm=underline       ctermfg=0     ctermbg=7     
hi  TabLineSel        cterm=bold            ctermfg=none  ctermbg=none  
hi  TabLineFill       cterm=reverse         ctermfg=none  ctermbg=none  
hi  CursorColumn      cterm=none            ctermfg=none  ctermbg=7
hi  CursorLine        cterm=underline       ctermfg=none  ctermbg=none
hi  ColorColumn       cterm=none            ctermfg=none  ctermbg=9     
hi  Normal            cterm=none            ctermfg=none  ctermbg=none  
hi  StatusLineTerm    cterm=bold            ctermfg=15    ctermbg=2     
hi  StatusLineTermNC  cterm=none            ctermfg=15    ctermbg=2     
hi  MatchParen        cterm=none            ctermfg=none  ctermbg=14    
hi  ToolbarLine       cterm=none            ctermfg=none  ctermbg=7     
hi  ToolbarButton     cterm=bold            ctermfg=15    ctermbg=8     
hi  Comment           cterm=none            ctermfg=4     ctermbg=none  
hi  Constant          cterm=none            ctermfg=1     ctermbg=none  
hi  Special           cterm=none            ctermfg=5     ctermbg=none  
hi  Identifier        cterm=none            ctermfg=6     ctermbg=none  
hi  Statement         cterm=none            ctermfg=3     ctermbg=none  
hi  PreProc           cterm=none            ctermfg=5     ctermbg=none  
hi  Type              cterm=none            ctermfg=2     ctermbg=none  
hi  Underlined        cterm=underline       ctermfg=5     ctermbg=none  
hi  Ignore            cterm=none            ctermfg=15    ctermbg=none  
hi  Error             cterm=none            ctermfg=15    ctermbg=9     
hi  Todo              cterm=none            ctermfg=0     ctermbg=11    
" }}}
"set t_Co=16
let g:colors_name="tuning"
" S this is and

hi  Folded        cterm=none      ctermfg=4     ctermbg=8
hi  Comment       cterm=none      ctermfg=4     ctermbg=none
hi  Search        cterm=none      ctermfg=0     ctermbg=11
"                                               
hi  CursorColumn  cterm=none      ctermfg=none  ctermbg=8
hi  CursorLine    cterm=none      ctermfg=none  ctermbg=8
hi  CursorLineNr  cterm=none      ctermfg=7     ctermbg=8
hi  LineNr        cterm=none      ctermfg=7     ctermbg=none
hi  MatchParen    cterm=none      ctermfg=none  ctermbg=7
hi  Visual        cterm=none      ctermfg=none  ctermbg=7
hi  VertSplit     cterm=reverse   ctermfg=7     ctermbg=none
"                                               
hi  Pmenu         cterm=none      ctermfg=none  ctermbg=8
hi  PmenuSel      cterm=reverse   ctermfg=7     ctermbg=none
hi  PmenuSbar     cterm=none      ctermfg=7     ctermbg=8   
hi  PmenuThumb    cterm=none      ctermfg=7     ctermbg=7
"                                               
hi  SpellBad      cterm=underline ctermfg=9     ctermbg=none
hi  SpellCap      cterm=underline ctermfg=12    ctermbg=none
hi  SpellLocal    cterm=underline ctermfg=14    ctermbg=none
"                                               
hi  TabLine       cterm=none      ctermfg=0     ctermbg=7
hi  TabLineFill   cterm=none      ctermfg=none  ctermbg=7
hi  TabLineSel    cterm=bold      ctermfg=none  ctermbg=none

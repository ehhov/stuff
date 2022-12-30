if exists("b:current_syntax")
  	finish
endif

finish

" Should be in Caption, but not in tabular{}
" Appears in \( math \), souldn't
" Commands are marked (\eq would be)
" .5\baselineskip is marked
"
" Exception for 's (Johnson's $S_U$)

syntax match tex_tildereq  '\([^~]\|^\)\<[A-Za-zА-Яа-я0-9]\{1,2\}\>\([^~{}]\|$\)'  containedin=@tex_tildereq_cluster
syntax match tex_tildereq  '\<[Tt][Hh][Ee]\>\([^~{}]\|$\)'  containedin=@tex_tildereq_cluster
syntax match tex_tildereq  '\<[Aa]\>\([^~{}]\|$\)'          containedin=@tex_tildereq_cluster
syntax match tex_tildereq  '\<[Aa][Nn]\>\([^~{}]\|$\)'      containedin=@tex_tildereq_cluster
" texDocZone,texParen,texMatcher,texSectionZone,texTitle,texAbstract,texBoldStyle,texItalStyle,texEmphStyle,texBoldItalStyle,texItalBoldStyle,texMatcherNM,texPartZone,texChapterZone,texParaZone,texSubSectionZone,texSubSubsectionZone,texSubParaZone
" Not including: texMathText
highlight link tex_tildereq  Error

syntax cluster tex_tildereq_cluster  contains=ALLBUT,@texMathZones,@texCmdGroup

set keymap=russian
set imsearch=-1
set iminsert=0
inoremap <c-space> <c-o>:let &iminsert=!&iminsert <cr>
nnoremap <c-space> :let &iminsert=!&iminsert <cr>
" make iminsert global
let g:layout = &iminsert
augroup global_keymap
	au! 
	au BufLeave * let g:layout = &iminsert
	au BufEnter * let &iminsert = g:layout
augroup END

" because langmap is noremap, but I need consistency between keyboard layouts
map Ё ~
map ё `
map А F
map Б <
map В D
map Г U
map Д L
map Е T
map Ж :
map З P
map И B
map Й Q
map К R
map Л K
map М V
map Н Y
map О J
map П G
map Р H
map С C
map Т N
map У E
map Ф A
map Х {
map Ц W
map Ч X
map Ш I
map Щ O
map Ъ }
map Ы S
map Ь M
map Э "
map Ю >
map Я Z
map а f
map б ,
map в d
map г u
map д l
map е t
map з p
map и b
map й q
map к r
map л k
map м v
map н y
map о j
map п g
map р h
map с c
map т n
map у e
map ф a
map х [
map ц w
map ч x
map ш i
map щ o
map ъ ]
map ы s
map ь m
map э '
map ю .
map я z
map № #
" langmap does something else that makes things like gg work right
" But it is still noremap, so they work like in a non-configured vim
set langmap=Ё~,ё`,АF,Б<,ВD,ГU,ДL,ЕT,Ж:,ЗP,ИB,ЙQ,КR,ЛK,МV,НY,ОJ,ПG,РH,СC,ТN,УE,ФA,Х{,ЦW,ЧX,ШI,ЩO,Ъ},ЫS,ЬM,Э\",Ю>,ЯZ,аf,вd,гu,дl,еt,зp,иb,йq,кr,лk,мv,нy,оj,пg,рh,сc,тn,уe,фa,х[,цw,чx,шi,щo,ъ],ыs,ьm,э',ю.,яz,№#

"command <C-v>u0446 w " \u0446 = ц

" All unmapped, because otherwise there is no chance to search for Russian letters or substitute them 
"cmap Ё ~
"cmap ё `
"cmap А F
"cmap Б <
"cmap В D
"cmap Г U
"cmap Д L
"cmap Е T
"cmap Ж :
"cmap З P
"cmap И B
"cmap Й Q
"cmap К R
"cmap Л K
"cmap М V
"cmap Н Y
"cmap О J
"cmap П G
"cmap Р H
"cmap С C
"cmap Т N
"cmap У E
"cmap Ф A
"cmap Х {
"cmap Ц W
"cmap Ч X
"cmap Ш I
"cmap Щ O
"cmap Ъ }
"cmap Ы S
"cmap Ь M
"cmap Э "
"cmap Ю >
"cmap Я Z
"cmap а f
"cmap в d
"cmap г u
"cmap д l
"cmap е t
"cmap з p
"cmap и b
"cmap й q
"cmap к r
"cmap л k
"cmap м v
"cmap н y
"cmap о j
"cmap п g
"cmap р h
"cmap с c
"cmap т n
"cmap у e
"cmap ф a
"cmap х [
"cmap ц w
"cmap ч x
"cmap ш i
"cmap щ o
"cmap ъ ]
"cmap ы s
"cmap ь m
"cmap э '
"cmap ю .
"cmap я z
"cmap № #
"cmap б ,
"cmap ю .
"cmap ж ;

" The following lines are trying to make <C-char> work in gvim under 
" a non-english layout. It didn't work because of the Crtl ASCII behavior.
" The lines are left here if this problem ever appears to be fixable. 
"map  <C-ё> <C-`>
"map! <C-ё> <C-`>
"map  <C-а> <C-f>
"map! <C-а> <C-f>
"map  <C-в> <C-d>
"map! <C-в> <C-d>
"map  <C-г> <C-u>
"map! <C-г> <C-u>
"map  <C-д> <C-l>
"map! <C-д> <C-l>
"map  <C-е> <C-t>
"map! <C-е> <C-t>
"map  <C-з> <C-p>
"map! <C-з> <C-p>
"map  <C-и> <C-b>
"map! <C-и> <C-b>
"map  <C-й> <C-q>
"map! <C-й> <C-q>
"map  <C-к> <C-r>
"map! <C-к> <C-r>
"map  <C-л> <C-k>
"map! <C-л> <C-k>
"map  <C-м> <C-v>
"map! <C-м> <C-v>
"map  <C-н> <C-y>
"map! <C-н> <C-y>
"map  <C-о> <C-j>
"map! <C-о> <C-j>
"map  <C-п> <C-g>
"map! <C-п> <C-g>
"map  <C-р> <C-h>
"map! <C-р> <C-h>
"map  <C-с> <C-c>
"map! <C-с> <C-c>
"map  <C-т> <C-n>
"map! <C-т> <C-n>
"map  <C-у> <C-e>
"map! <C-у> <C-e>
"map  <C-ф> <C-a>
"map! <C-ф> <C-a>
"map  <C-х> <C-[>
"map! <C-х> <C-[>
"map  <C-ц> <C-w>
"map! <C-ц> <C-w>
"map  <C-ч> <C-x>
"map! <C-ч> <C-x>
"map  <C-ш> <C-i>
"map! <C-ш> <C-i>
"map  <C-щ> <C-o>
"map! <C-щ> <C-o>
"map  <C-ъ> <C-]>
"map! <C-ъ> <C-]>
"map  <C-ы> <C-s>
"map! <C-ы> <C-s>
"map  <C-ь> <C-m>
"map! <C-ь> <C-m>
"map  <C-э> <C-'>
"map! <C-э> <C-'>
"map  <C-ю> <C-.>
"map! <C-ю> <C-.>
"map  <C-я> <C-z>
"map! <C-я> <C-z>
"map  <C-б> <C-,>
"map! <C-б> <C-,>
"map  <C-ю> <C-.>
"map! <C-ю> <C-.>
"map  <C-ж> <C-;>
"map! <C-ж> <C-;>

syntax on
set nu
" NERDTree:
autocmd vimenter * NERDTree
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTreeType") && b:NERDTreeType == "primary") | q | endif
let g:NERDTreeDirArrows=0
" ctrl-p fuzzy serarch:
set runtimepath^=~/.vim/bundle/ctrlp.vim

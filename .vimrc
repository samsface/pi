syntax on
set nu
command CDC cd %:p:h
autocmd vimenter * NERDTree
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTreeType") && b:NERDTreeType == "primary") | q | endif
let g:NERDTreeDirArrows=0
set runtimepath^=~/.vim/bundle/ctrlp.vim

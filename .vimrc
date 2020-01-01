" Vimrc

filetype indent plugin on

set background=dark
syntax on

"-----------------------------------------------------------
" Settings

set nocompatible
set encoding=utf-8
set hidden
set wildmenu
set showcmd
set hlsearch
set ignorecase
set smartcase
set backspace=indent,eol,start
set autoindent
set nostartofline
set ruler
set laststatus=2
set confirm
set visualbell
set t_vb=
set mouse=a
set cmdheight=2
set nu rnu
set notimeout ttimeout ttimeoutlen=200
set pastetoggle=<F11>
set shiftwidth=2
set softtabstop=2
set expandtab

"------------------------------------------------------------
" Mappings

" Map Y to act like D and C, i.e. to yank until EOL, rather than act as yy,
" which is the default
set clipboard=unnamed
vnoremap y "*y
vnoremap Y "*Y
nnoremap p "*p
nnoremap P "*P
"map Y $y

" Map <C-L> (redraw screen) to also turn off search highlighting until the
" next search
nnoremap <C-L> :redraw!<CR>
nnoremap <C-I> :nohl<CR><C-L>

" Map split window navigation
noremap <C-J> <C-W><C-J>
noremap <C-K> <C-W><C-K>
noremap <C-L> <C-W><C-L>
noremap <C-H> <C-W><C-H>
nnoremap <C-W>O :call MaximizeToggle()<CR>
nnoremap <C-W>o :call MaximizeToggle()<CR>
nnoremap <C-W><C-O> :call MaximizeToggle()<CR>
nnoremap <leader>mm :call MaximizeToggle()<CR>

function! MaximizeToggle()
  if exists("s:maximize_session")
    exec "source " . s:maximize_session
    call delete(s:maximize_session)
    unlet s:maximize_session
    let &hidden=s:maximize_hidden_save
    unlet s:maximize_hidden_save
  else
    let s:maximize_hidden_save = &hidden
    let s:maximize_session = tempname()
    set hidden
    exec "mksession! " . s:maximize_session
    only
  endif
endfunction

" Split window configuration
set splitbelow
set splitright

"------------------------------------------------------------
" Call plugins

call plug#begin('~/.vim/plugged')

Plug 'vim-airline/vim-airline'
Plug 'xolox/vim-colorscheme-switcher'
Plug 'xolox/vim-misc'
Plug 'sheerun/vim-polyglot'
" bugger
Plug 'jeetsukumaran/vim-buffergator'
Plug 'kien/ctrlp.vim'
" tagging
"Plug 'multilobyte/gtags-cscope'
Plug 'ludovicchabant/vim-gutentags'
"Plug 'skywind3000/gutentags_plus'

if has ('nvim')
  Plug 'shougo/deoplete.nvim', { 'do': ':UpdateRemotePlugins' }
else
  Plug 'shougo/deoplete.nvim'
  Plug 'roxma/nvim-yarp'
  Plug 'roxma/vim-hug-neovim-rpc'
endif

let g:deoplete#enable_at_startup = 1
Plug 'Shougo/neosnippet.vim'
Plug 'Shougo/neosnippet-snippets'

call plug#end()

"------------------------------------------------------------
" Autocomplete and snippets
"
imap <S-TAB>  <Plug>(neosnippet_expand_or_jump)
smap <S-TAB>  <Plug>(neosnippet_expand_or_jump)
xmap <S-TAB>  <Plug>(neosnippet_expand_target)

inoremap<expr> <TAB> pumvisible() ? "\<C-n>" : "\<TAB>"

if has('conceal')
  set conceallevel=2 concealcursor=niv
endif

"------------------------------------------------------------
" Buffer stuf

" setup tagging
" enable gtags module
let g:gutentags_modules = ['ctags']
let g:gutentags_project_root = ['.root']
let g:gutentags_cache_dir = expand('~/.cache/tags')

" Setup some default ignores
let g:ctrlp_custom_ignore = {
  \ 'dir':  '\v[\/](\.(git|hg|svn)|\_site)$',
  \ 'file': '\v\.(exe|so|dll|class|png|jpg|jpeg)$',
  \}

let g:ctrlp_working_path_mode = 'r'

" enable buffer tab bar
let g:airline#extensions#tabline#enabled = 1

" Use a leader instead of the actual named binding
nmap <leader>p :CtrlP<cr>

nmap <leader>bb :CtrlPBuffer<cr>
nmap <leader>bm :CtrlPMixed<cr>
nmap <leader>bs :CtrlPMRU<cr>

set wildignore+=*/tmp/*,*.so,*.swp,*.zip     " MacOSX/Linux
set wildignore+=*\\tmp\\*,*.swp,*.zip,*.exe  " Windows

let g:ctrlp_custom_ignore = '\v[\/]\.(git|hg|svn)$'
let g:ctrlp_custom_ignore = {
  \ 'dir':  '\v[\/]\.(git|hg|svn)$',
  \ 'file': '\v\.(exe|so|dll)$',
  \ 'link': 'some_bad_symbolic_links',
  \ }

" Use the right side of the screen
let g:buffergator_viewport_split_policy = 'R'

" I want my own keymappings...
let g:buffergator_suppress_keymaps = 1

" Looper buffers
"let g:buffergator_mru_cycle_loop = 1

" Go to the previous buffer open
nmap <leader>jj :BuffergatorMruCyclePrev<cr>

" Go to the next buffer open
nmap <leader>kk :BuffergatorMruCycleNext<cr>

" View the entire list of buffers open
nmap <leader>bl :BuffergatorOpen<cr>

" Shared bindings from Solution #1 from earlier
nmap <leader>T :enew<cr>
nmap <leader>bq :bp <BAR> bd #<cr>

" use ctags for tagging
" au BufWritePost *.c,*.cpp,*.h silent! !ctags -R &
nnoremap <leader>rb :! buildcurr.sh <CR>
nnoremap <leader>rm :!( cd ./build ; make)<CR>
nnoremap <leader>rr :!bin/run
nnoremap <leader>rt :!bin/run_tests<CR>
nnoremap <leader>ra :!./run.sh
nnoremap <leader>sv :source ~/.vimrc<CR>
nnoremap <leader>rd :!doxygen doc/doxygen.conf<CR>
nnoremap <leader>cc :CtrlPClearCache<CR>

" ctags mapping
nnoremap <leader>ct :!(ctags -R --exclude=build)<CR>

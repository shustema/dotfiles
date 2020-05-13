" Vimrc

set autoread
au CursorHold * checktime

" Get Plugged
if empty(glob('~/.vim/autoload/plug.vim'))
  silent !curl -fLo ~/.vim/autoload/plug.vim --create-dirs
    \ https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
  autocmd VimEnter * PlugInstall --sync | source $MYVIMRC
endif

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
map Y $y

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

" Backup
set backup
set backupdir=~/.vim/backup//
set writebackup
set backupcopy=yes
au BufWritePre * let &bex = '@' . strftime("%F.%H:%M")

" Don't need this with NerdTree
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

" status bar
Plug 'vim-airline/vim-airline'
Plug 'tpope/vim-fugitive'
" look and feel
Plug 'xolox/vim-colorscheme-switcher'
Plug 'xolox/vim-misc'
Plug 'powerline/powerline-fonts'
Plug 'ryanoasis/vim-devicons'
" formatting tools
Plug 'sheerun/vim-polyglot'
Plug 'lervag/vimtex'
Plug 'vhdirk/vim-cmake'
" buffer
Plug 'jeetsukumaran/vim-buffergator'
Plug 'kien/ctrlp.vim'
" tagging
Plug 'ludovicchabant/vim-gutentags'
Plug 'google/vim-maktaba'
Plug 'google/vim-codefmt'
Plug 'google/vim-glaive'
" autocomplete 
if has ('nvim')
  Plug 'shougo/deoplete.nvim', { 'do': ':UpdateRemotePlugins' }
  Plug 'zchee/deoplete-clang'
else
  Plug 'shougo/deoplete.nvim'
  Plug 'roxma/nvim-yarp'
  Plug 'roxma/vim-hug-neovim-rpc'
  Plug 'mhinz/vim-grepper'
  Plug 'justmao945/vim-clang'
endif

" snippets
Plug 'Shougo/neosnippet.vim'
Plug 'Shougo/neosnippet-snippets'
" nerdtree
Plug 'preservim/nerdtree'
Plug 'google/yapf'
" colors
Plug 'ap/vim-css-color'
" vim wiki
Plug 'vimwiki/vimwiki'
Plug 'dhruvasagar/vim-table-mode'

call plug#end()

set guifont=DroidSansMono\ Nerd\ Font\ 11

let g:webdevicons_enable_nerdtree = 1
let g:webdevicons_enable_ctrlp = 1
let g:webdevicons_enable_airline_tabline = 1
let g:webdevicons_enable_airline_statusline = 1

"------------------------------------------------------------
" Auto Formater

augroup autoformat_settings
  autocmd FileType bzl AutoFormatBuffer buildifier
  autocmd FileType c,cpp,proto AutoFormatBuffer clang-format
  autocmd FileType javascript AutoFormatBuffer prettier
  autocmd FileType dart AutoFormatBuffer dartfmt
  autocmd FileType go AutoFormatBuffer gofmt
  autocmd FileType gn AutoFormatBuffer gn
  autocmd FileType html,css,sass,scss,less,json AutoFormatBuffer js-beautify
  autocmd FileType java AutoFormatBuffer google-java-format
  "autocmd FileType python AutoFormatBuffer yapf
  autocmd FileType python AutoFormatBuffer autopep8
  autocmd FileType rust AutoFormatBuffer rustfmt
  autocmd FileType vue AutoFormatBuffer prettier
augroup END
let g:formatter_yapf_style = 'pep8'

" Deoplete Clang
let g:deoplete#enable_at_startup = 1
let g:clang_vim_exec = 'vim'
let g:clang_diagsopt = ''
let g:clang_auto=0
let g:clang_include_sysheaders = 1
let g:clang_include_sysheaders_from_gcc = 1
let g:python_host_prog = '/usr/bin/python'
let g:python3_host_prog = '/usr/bin/python3.8'

syntax enable
filetype plugin indent on

" Latex config
set nospell
autocmd BufRead,BufNewFile *.tex setlocal spell spelllang=en_us
inoremap <leader>w <c-g>u<Esc>[s1z=`]a<c-g>u
let g:livepreview_previewer = 'qpdfview'

let g:tex_flavor='latex'
let g:vimtex_view_method='qpdfview'
let g:vimtex_quickfix_mode=0
set conceallevel=1
let g:tex_conceal='abdmg'

"------------------------------------------------------------
" air-line
let g:airline#extensions#tabline#formatter = 'unique_tail'
let g:airline_powerline_fonts = 1

if !exists('g:airline_symbols')
    let g:airline_symbols = {}
endif

"------------------------------------------------------------
" vimwiki
let wiki_1 = {}
let wiki_1.path = '~/.vim/vimwiki/'
let wiki_1.nested_syntaxes = { 'python' : 'python', 
        \'c++': 'cpp',
        \'css': 'css'}
let wiki_1.syntax = 'markdown'

let wiki_2 = {}
let wiki_2.autowriteall = 1
let g:vimwiki_list = [wiki_1,wiki_2]

"------------------------------------------------------------
" NerdTree
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 0 && !exists("s:std_in") | NERDTree | endif
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 1 && isdirectory(argv()[0]) && !exists("s:std_in") | exe 'NERDTree' argv()[0] | wincmd p | ene | exe 'cd '.argv()[0] | endif
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | q | endif
map <C-n> :NERDTreeToggle<CR>
let g:NERDTreeDirArrowExpandable = '▸'
let g:NERDTreeDirArrowCollapsible = '▾'

let g:WebDevIconsUnicodeDecorateFolderNodes = 1
let g:DevIconsEnableFoldersOpenClose = 1

let g:DevIconsDefaultFolderOpenSymbol='' "symbol for open folder (f07c)
let g:WebDevIconsUnicodeDecorateFolderNodesDefaultSymbol='' " symbol for closed folder (f07b)

function! NERDTreeHighlightFile(extension, fg, bg, guifg, guibg)
 exec 'autocmd filetype nerdtree highlight ' . a:extension .' ctermbg='. a:bg .' ctermfg='. a:fg .' guibg='. a:guibg .' guifg='. a:guifg
 exec 'autocmd filetype nerdtree syn match ' . a:extension .' #^\s\+.*'. a:extension .'$#'
endfunction

call NERDTreeHighlightFile('jade', 'green', 'none', 'green', '#151515')
call NERDTreeHighlightFile('ini', 'yellow', 'none', 'yellow', '#151515')
call NERDTreeHighlightFile('md', 'blue', 'none', '#3366FF', '#151515')
call NERDTreeHighlightFile('yml', 'yellow', 'none', 'yellow', '#151515')
call NERDTreeHighlightFile('config', 'yellow', 'none', 'yellow', '#151515')
call NERDTreeHighlightFile('conf', 'yellow', 'none', 'yellow', '#151515')
call NERDTreeHighlightFile('json', 'yellow', 'none', 'yellow', '#151515')
call NERDTreeHighlightFile('html', 'yellow', 'none', 'yellow', '#151515')
call NERDTreeHighlightFile('styl', 'cyan', 'none', 'cyan', '#151515')
call NERDTreeHighlightFile('css', 'cyan', 'none', 'cyan', '#151515')
call NERDTreeHighlightFile('coffee', 'Red', 'none', 'red', '#151515')
call NERDTreeHighlightFile('js', 'Red', 'none', '#ffa500', '#151515')
call NERDTreeHighlightFile('php', 'Magenta', 'none', '#ff00ff', '#151515')

"------------------------------------------------------------
" Auto complete and snippets

imap <C-L>  <Plug>(neosnippet_expand_or_jump)
smap <C-L>  <Plug>(neosnippet_expand_or_jump)
xmap <C-L>  <Plug>(neosnippet_expand_target)

inoremap<expr> <TAB> pumvisible() ? "\<C-n>" : "\<TAB>"

if has('conceal')
  set conceallevel=2 concealcursor=niv
endif

"auto close {
function! s:CloseBracket()
    let line = getline('.')
    if line =~# '^\s*\(struct\|class\|enum\) '
        return "{\<Enter>};\<Esc>O"
    elseif searchpair('(', '', ')', 'bmn', '', line('.'))
        " Probably inside a function call. Close it off.
        return "{\<Enter>});\<Esc>O"
    else
        return "{\<Enter>}\<Esc>O"
    endif
endfunction
inoremap <expr> {<Enter> <SID>CloseBracket()

"------------------------------------------------------------
" Buffer stuff

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
nmap <leader>cp :CtrlP<cr>

nmap <leader>vv :CtrlPBuffer<cr>
nmap <leader>vm :CtrlPMixed<cr>
nmap <leader>vs :CtrlPMRU<cr>
nmap <leader>jj :BuffergatorMruCyclePrev<cr>
nmap <leader>kk :BuffergatorMruCycleNext<cr>
nmap <leader>vl :BuffergatorOpen<cr>

set wildignore+=*/tmp/*,*.so,*.swp,*.zip     " MacOSX/Linux
set wildignore+=*\\tmp\\*,*.swp,*.zip,*.exe  " Windows

let g:ctrlp_custom_ignore = '\v[\/]\.(git|hg|svn)$'
let g:ctrlp_custom_ignore = {
  \ 'dir':  '\v[\/]\.(git|hg|svn)$',
  \ 'file': '\v\.(exe|so|dll)$',
  \ 'link': 'some_bad_symbolic_links',
  \ }

let g:buffergator_viewport_split_policy = 'R'
let g:buffergator_suppress_keymaps = 1

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

" Prettier
nmap <leader>pp :FormatCode<CR>
nmap <leader>bs :!bash .vimsource<CR>
nmap <leader>bb :!bash build.sh<CR>
nmap <leader>bm :!bash make.sh<CR>
nmap <leader>bd :!bash docs.sh<CR>

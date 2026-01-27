cd src
gcc \
    ./module_comments/comments.c \
    ./module_directives/directives.c \
    ./module_args/arguments_menu.c \
    ./utils/file_utils.c \
    ./utils/string_utils.c \
    ./utils/errors_utils.c \
    main.c \
    -o preprocessor
chmod +x preprocessor
mkdir -p ~/.local/bin
mv preprocessor ~/.local/bin/
mkdir -p ~/.local/share/preprocessor/
cp man_page.txt ~/.local/share/preprocessor/man_page.txt
source ~/.bashrc
cd ..
preprocessor ./input-code-example/input-example.c -c
#!/bin/bash

# Build script for C Preprocessor
# Compiles all source files and creates the preprocessor executable

echo "Building C Preprocessor..."

cd src

gcc -I. \
    comments/comments.c \
    define/define.c \
    directives/directives.c \
    ifdef/ifdef.c \
    include/include.c \
    keyword_dispatcher/keyword_dispatcher.c \
    macro_expander/macro_expander.c \
    macro_parser/macro_parser.c \
    parse_arguments/parse_arguments.c \
    preprocessor/preprocessor.c \
    symbol_table/symbol_table.c \
    utils/error_utils.c \
    utils/string_utils.c \
    utils/file_utils.c \
    main.c \
    -o ../preprocessor

# To store the program in the Linux OS user executables
chmod +x ../preprocessor
mkdir -p ~/.local/bin
mv ../preprocessor ~/.local/bin/preprocessor
mkdir -p ~/.local/share/preprocessor/
cp man_page.txt ~/.local/share/preprocessor/man_page.txt
source ~/.bashrc

cd ..

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Preprocessor executable created: preprocessor"
    echo ""
    echo "Usage: preprocessor <input_file>"
    echo "Example: preprocessor test_files/test_comments.c"
else
    echo "Build failed!"
    exit 1
fi

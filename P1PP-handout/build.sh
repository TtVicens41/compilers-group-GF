gcc arguments_menu.c file_utils.c string_utils.c comments.c main.c -o preprocessor
chmod +x preprocessor
mkdir -p ~/.local/bin
mv preprocessor ~/.local/bin/
source ~/.bashrc
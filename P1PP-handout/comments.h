#ifndef COMMENTS_H
#define COMMENTS_H

void delete_comments(
    const char *input_path, 
    const char *output_path
);

void delete_empty_rows(
    const char *input_path, 
    const char *output_path
);

void process_comments(
    const char *input_path, 
    const char *output_path
);

#endif
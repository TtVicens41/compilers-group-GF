/**
 * @title: string_list.h
 * @authors:
 * @creation:
 */

/**
 * A string list implementation using a pointer to character
 * pointers and a size value to contrlor the number of strings.
 */
typedef struct {
    char **buffer; /// A sring array.
    int size; /// Number of strings in string array.
} StringList;

/**
 * Initializes the attributes of the string list.
 * @param[in] buffer A string array.
 * @param size The number of strings in string array.
 * @return A dynamic allocated string list.
 */
StringList *init_string_list(char **buffer, int size);

StringList *string_split(char *string, char splitter); 

StringList *apply_string_list(StringList *string_list, char *(*applier)(const char *), int delete);

/**
 * @brief Explica la responsabilidad de `clear_string_list` en el flujo del compilador.
 * @param Recibe: `StringList *string_list`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void clear_string_list(StringList *string_list);

/**
 * @brief Explica la responsabilidad de `delete_string_list` en el flujo del compilador.
 * @param Recibe: `StringList **string_list`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void delete_string_list(StringList **string_list);

/**
 * @brief Explica la responsabilidad de `print_string_list` en el flujo del compilador.
 * @param Recibe: `StringList *string_list, int raw`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void print_string_list(StringList *string_list, int raw);

int *to_integer_array(StringList *string_list, int delete);

# Explicación Detallada: Directiva -d del Preprocesador

## Autor: Proyecto de Compiladores
## Fecha: 11 de enero de 2026

---

## Índice

1. [Visión General](#visión-general)
2. [Estructuras de Datos](#estructuras-de-datos)
3. [Funciones Auxiliares](#funciones-auxiliares)
4. [Función Principal: process_directives](#función-principal-process_directives)
5. [Ejemplo Completo de Ejecución](#ejemplo-completo-de-ejecución)
6. [Casos Especiales](#casos-especiales)

---

## Visión General

La directiva `-d` del preprocesador tiene como objetivo **eliminar todas las directivas del preprocesador** (líneas que empiezan con `#`) y **reemplazar las constantes y macros** definidas con `#define` por sus valores correspondientes.

### ¿Qué hace exactamente?

1. **Elimina** todas las líneas que empiezan con `#` (directivas del preprocesador)
2. **Almacena** en una tabla las definiciones de `#define` 
3. **Reemplaza** en el código todas las apariciones de constantes/macros por sus valores

### Ejemplo:

**Entrada:**
```c
#define ON 1
#define OFF 0
int flag = ON;
```

**Salida:**
```c
int flag = 1;
```

---

## Estructuras de Datos

### 1. Estructura `Define`

```c
typedef struct {
    char *name;     // Nombre de la constante (ej: "ON")
    char *value;    // Valor de reemplazo (ej: "1")
} Define;
```

**Propósito:** Almacenar cada definición `#define` que encontremos en el archivo.

**Ejemplo:** 
- Para `#define MAX 100`
  - `name = "MAX"`
  - `value = "100"`

### 2. Tabla Global de Definiciones

```c
static Define defines[MAX_DEFINES];  // Array de hasta 100 definiciones
static int define_count = 0;         // Contador de cuántas llevamos
```

**Propósito:** Mantener un registro de todas las `#define` para poder buscarlas después.

---

## Funciones Auxiliares

### 1. `add_define(name, value)`

**Qué hace:** Añade una nueva definición a la tabla global.

**Paso a paso:**

```c
void add_define(const char *name, const char *value) {
    // 1. Comprobar si la tabla está llena
    if (define_count >= MAX_DEFINES) {
        return;  // No hay espacio, salir
    }
    
    // 2. Reservar memoria dinámica para el nombre
    //    strlen(name) + 1 porque necesitamos espacio para '\0'
    defines[define_count].name = malloc(strlen(name) + 1);
    
    // 3. Reservar memoria dinámica para el valor
    defines[define_count].value = malloc(strlen(value) + 1);
    
    // 4. Copiar las cadenas en la memoria reservada
    strcpy(defines[define_count].name, name);
    strcpy(defines[define_count].value, value);
    
    // 5. Incrementar el contador
    define_count++;
}
```

**Ejemplo:**
- `add_define("PI", "3.14159")`
  - Se guarda en `defines[0].name = "PI"`
  - Se guarda en `defines[0].value = "3.14159"`
  - `define_count` pasa de 0 a 1

---

### 2. `find_define(name)`

**Qué hace:** Busca una definición por su nombre y devuelve su valor.

**Paso a paso:**

```c
const char* find_define(const char *name) {
    // Recorrer toda la tabla de definiciones
    for (int i = 0; i < define_count; i++) {
        // Comparar el nombre buscado con cada entrada
        if (strcmp(defines[i].name, name) == 0) {
            // ¡Encontrado! Devolver el valor
            return defines[i].value;
        }
    }
    // No encontrado
    return NULL;
}
```

**Ejemplo:**
- Si tenemos `defines[0] = {name: "MAX", value: "100"}`
- `find_define("MAX")` → devuelve `"100"`
- `find_define("MIN")` → devuelve `NULL` (no existe)

---

### 3. `trim(str)`

**Qué hace:** Elimina espacios, tabulaciones y saltos de línea al inicio y final de una cadena.

**Paso a paso:**

```c
char* trim(char *str) {
    char *end;
    
    // PASO 1: Eliminar espacios/tabs del inicio
    // Avanzar el puntero mientras haya espacios o tabs
    while (*str == ' ' || *str == '\t') 
        str++;  // Mover el puntero hacia adelante
    
    // PASO 2: Si la cadena quedó vacía, devolverla
    if (*str == 0) return str;
    
    // PASO 3: Eliminar espacios/tabs/newlines del final
    // Posicionarse en el último carácter
    end = str + strlen(str) - 1;
    
    // Retroceder mientras haya espacios, tabs o saltos de línea
    while (end > str && (*end == ' ' || *end == '\t' || 
                         *end == '\n' || *end == '\r')) 
        end--;  // Mover hacia atrás
    
    // PASO 4: Poner '\0' después del último carácter válido
    *(end + 1) = 0;
    
    return str;
}
```

**Ejemplo:**
- `trim("  hello  \n")` → `"hello"`
- `trim("\t\tON\n")` → `"ON"`

---

### 4. `replace_defines_in_line(line, output_line)`

**Qué hace:** Recorre una línea carácter por carácter, identifica identificadores de C y los reemplaza si están en la tabla de defines.

**Paso a paso detallado:**

```c
void replace_defines_in_line(char *line, char *output_line) {
    char *pos = line;        // Puntero a la posición actual en la entrada
    char *out = output_line; // Puntero a la posición actual en la salida
    
    // Procesar cada carácter de la línea
    while (*pos) {
        // ¿Es una letra o guión bajo? (inicio de identificador)
        if ((*pos >= 'A' && *pos <= 'Z') || 
            (*pos >= 'a' && *pos <= 'z') || 
            *pos == '_') {
            
            // PASO 1: Extraer el identificador completo
            char identifier[256];
            int i = 0;
            
            // Continuar mientras sea letra, dígito o guión bajo
            while ((*pos >= 'A' && *pos <= 'Z') || 
                   (*pos >= 'a' && *pos <= 'z') || 
                   (*pos >= '0' && *pos <= '9') || 
                   *pos == '_') {
                identifier[i++] = *pos++;  // Copiar carácter y avanzar
            }
            identifier[i] = '\0';  // Terminar la cadena
            
            // PASO 2: Buscar si está definido
            const char *value = find_define(identifier);
            
            if (value) {
                // ENCONTRADO: Copiar el valor de reemplazo
                strcpy(out, value);
                out += strlen(value);  // Avanzar el puntero de salida
            } else {
                // NO ENCONTRADO: Mantener el identificador original
                strcpy(out, identifier);
                out += strlen(identifier);
            }
        } else {
            // No es un identificador: copiar el carácter tal cual
            *out++ = *pos++;
        }
    }
    *out = '\0';  // Terminar la cadena de salida
}
```

**Ejemplo paso a paso:**

Entrada: `"int x = MAX + MIN;"`  
Tabla: `MAX → 100`, `MIN → 0`

1. **'i'**: Es letra → extrae `"int"` → no está en tabla → copia `"int"`
2. **' '**: No es letra → copia `' '`
3. **'x'**: Es letra → extrae `"x"` → no está en tabla → copia `"x"`
4. **' = '**: No son letras → copia `" = "`
5. **'M'**: Es letra → extrae `"MAX"` → ¡ESTÁ en tabla! → copia `"100"`
6. **' + '**: No son letras → copia `" + "`
7. **'M'**: Es letra → extrae `"MIN"` → ¡ESTÁ en tabla! → copia `"0"`
8. **';'**: No es letra → copia `";"`

**Salida:** `"int x = 100 + 0;"`

---

## Función Principal: process_directives

Esta es la función principal que orquesta todo el proceso.

### Algoritmo General:

```
1. Abrir archivo de entrada
2. Abrir archivo de salida
3. Para cada línea del archivo:
   a. Si empieza con '#':
      - Si es #define: extraer nombre y valor, guardar en tabla
      - Si es otra directiva: ignorar
      - NO escribir en el archivo de salida
   b. Si es línea normal:
      - Reemplazar todas las constantes definidas
      - Escribir en el archivo de salida
4. Cerrar archivos
5. Liberar memoria
```

### Código Comentado Línea por Línea:

```c
void process_directives(const char *input_path, const char *output_path) {
    // === APERTURA DE ARCHIVOS ===
    
    // Abrir el archivo de entrada en modo lectura
    FILE *input_file = fopen(input_path, "r");
    if (!input_file) {
        printf(OPEN_FAILURE, input_path);  // Error: no se pudo abrir
        return;
    }

    // Abrir el archivo de salida en modo escritura
    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        printf(OPEN_FAILURE, output_path);
        fclose(input_file);  // Cerrar el archivo de entrada antes de salir
        return;
    }

    // === DECLARACIÓN DE VARIABLES ===
    
    char line[MAX_LINE_LENGTH];              // Buffer para leer cada línea (1024 bytes)
    char output_line[MAX_LINE_LENGTH * 2];   // Buffer de salida (2048 bytes, por si crece)
    int skip_continuation = 0;                // Flag para manejar #define multilínea
    
    // === BUCLE PRINCIPAL: PROCESAR LÍNEA POR LÍNEA ===
    
    while (fgets(line, sizeof(line), input_file)) {
        // fgets lee una línea completa incluyendo el '\n' final
        
        // --- CASO 1: Líneas de continuación de #define ---
        // Un #define puede ocupar varias líneas si termina con '\'
        // Ejemplo:
        //   #define ERROR(x) printf("Error: "); \
        //                    printf(x)
        
        if (skip_continuation) {
            // Esta línea es continuación de un #define anterior
            size_t len = strlen(line);
            skip_continuation = 0;  // Resetear el flag
            
            // ¿Esta línea también termina con '\'?
            if (len > 1 && line[len - 2] == '\\') {
                skip_continuation = 1;  // Sí, la siguiente también es continuación
            }
            continue;  // No escribir esta línea en la salida
        }
        
        // --- PRESERVAR LA LÍNEA ORIGINAL ---
        // Necesitamos hacer una copia porque trim() modifica la cadena
        // Queremos mantener los saltos de línea en la salida
        
        char line_copy[MAX_LINE_LENGTH];
        strcpy(line_copy, line);      // Copiar la línea
        char *trimmed = trim(line_copy);  // Obtener versión sin espacios
        
        // --- CASO 2: ¿Es una directiva del preprocesador? ---
        
        if (trimmed[0] == '#') {
            // Sí, empieza con '#'
            
            // Saltar el carácter '#'
            char *directive = trimmed + 1;  // Apuntar al siguiente carácter
            directive = trim(directive);     // Quitar espacios después de '#'
            
            // ¿Es un #define?
            if (strncmp(directive, "define", 6) == 0) {
                // Sí, es #define
                
                // Saltar la palabra "define"
                char *def = directive + 6;
                def = trim(def);  // Quitar espacios después de "define"
                
                // --- Extraer el NOMBRE de la constante/macro ---
                char name[256];
                int i = 0;
                
                // Leer caracteres hasta encontrar espacio, tab o fin de línea
                while (*def && *def != ' ' && *def != '\t' && *def != '\n') {
                    name[i++] = *def++;
                }
                name[i] = '\0';  // Terminar la cadena
                
                // Ejemplo: "#define MAX 100"
                //          Después de esto: name = "MAX", def apunta a " 100"
                
                // --- Extraer el VALOR ---
                def = trim(def);  // Quitar espacios antes del valor
                char value[1024] = "";
                
                if (*def) {  // Si hay algo después del nombre
                    strcpy(value, def);  // Copiar todo el valor
                    
                    // Quitar el '\n' del final
                    char *newline = strchr(value, '\n');
                    if (newline) *newline = '\0';
                    
                    // ¿El valor termina con '\'? (define multilínea)
                    size_t len = strlen(value);
                    if (len > 0 && value[len - 1] == '\\') {
                        skip_continuation = 1;  // La siguiente línea también es parte del #define
                        value[len - 1] = '\0';   // Quitar el '\'
                    }
                }
                
                // --- Guardar en la tabla ---
                add_define(name, value);
                
                // NO escribir esta línea en la salida (queremos eliminar los #define)
                continue;  // Pasar a la siguiente línea
            }
            
            // Otras directivas (#include, #ifndef, #endif, #ifdef, etc.)
            // También las eliminamos (no las escribimos en la salida)
            continue;
        }
        
        // --- CASO 3: Línea normal (no es directiva) ---
        
        // Reemplazar todas las constantes/macros por sus valores
        replace_defines_in_line(line, output_line);
        
        // Escribir la línea procesada en el archivo de salida
        fputs(output_line, output_file);
    }

    // === LIMPIEZA FINAL ===
    
    fclose(input_file);   // Cerrar archivo de entrada
    fclose(output_file);  // Cerrar archivo de salida
    free_defines();       // Liberar toda la memoria de las definiciones
}
```

---

## Ejemplo Completo de Ejecución

### Archivo de entrada: `example.h`

```c
#define ON 1
#define OFF 0
#define MAX 100

int flag = ON;
int counter = MAX;
if (status == OFF) {
    printf("Apagado\n");
}
```

### Paso a paso de la ejecución:

#### **Línea 1:** `#define ON 1`

1. `fgets` lee: `"#define ON 1\n"`
2. `trim` → `"#define ON 1"`
3. Detecta `#` → es una directiva
4. Extrae `"define"`
5. Extrae nombre: `"ON"`
6. Extrae valor: `"1"`
7. `add_define("ON", "1")`
   - `defines[0] = {name: "ON", value: "1"}`
   - `define_count = 1`
8. `continue` → **NO se escribe en la salida**

#### **Línea 2:** `#define OFF 0`

1. Similar al anterior
2. `add_define("OFF", "0")`
   - `defines[1] = {name: "OFF", value: "0"}`
   - `define_count = 2`
3. **NO se escribe en la salida**

#### **Línea 3:** `#define MAX 100`

1. `add_define("MAX", "100")`
   - `defines[2] = {name: "MAX", value: "100"}`
   - `define_count = 3`
2. **NO se escribe en la salida**

#### **Línea 4:** (línea vacía)

1. `fgets` lee: `"\n"`
2. No empieza con `#`
3. `replace_defines_in_line("\n", output_line)`
   - No hay identificadores
   - `output_line = "\n"`
4. `fputs("\n", output_file)` → **SE ESCRIBE**

#### **Línea 5:** `int flag = ON;`

1. `fgets` lee: `"int flag = ON;\n"`
2. No empieza con `#`
3. `replace_defines_in_line`:
   - `"int"` → no está en tabla → mantener `"int"`
   - `" flag = "` → no son identificadores → mantener
   - `"ON"` → **¡ESTÁ en tabla!** → reemplazar por `"1"`
   - `";\n"` → mantener
   - Resultado: `"int flag = 1;\n"`
4. `fputs("int flag = 1;\n", output_file)` → **SE ESCRIBE**

#### **Línea 6:** `int counter = MAX;`

1. Similar al anterior
2. `"MAX"` → reemplazar por `"100"`
3. Resultado: `"int counter = 100;\n"` → **SE ESCRIBE**

#### **Línea 7:** `if (status == OFF) {`

1. `"OFF"` → reemplazar por `"0"`
2. Resultado: `"if (status == 0) {\n"` → **SE ESCRIBE**

#### **Línea 8:** `printf("Apagado\n");`

1. No hay identificadores en la tabla
2. Se mantiene igual → **SE ESCRIBE**

#### **Línea 9:** `}`

1. Se mantiene igual → **SE ESCRIBE**

### Archivo de salida: `example_pp.h`

```c

int flag = 1;
int counter = 100;
if (status == 0) {
    printf("Apagado\n");
}
```

**Nota:** Las tres primeras líneas (#define) han sido **eliminadas**, y las constantes `ON`, `OFF`, `MAX` han sido **reemplazadas** por sus valores.

---

## Casos Especiales

### 1. **#define multilínea**

**Entrada:**
```c
#define ERROR(x) printf("Error: "); \
                 printf(x)
```

**Procesamiento:**
1. Primera línea termina con `\` → `skip_continuation = 1`
2. Segunda línea: `skip_continuation` está activo → se salta
3. Ambas líneas se eliminan de la salida

### 2. **#define sin valor**

**Entrada:**
```c
#define DEBUG
```

**Procesamiento:**
- `name = "DEBUG"`
- `value = ""` (cadena vacía)
- Se guarda en la tabla
- Cuando aparezca `DEBUG` en el código, se reemplazará por nada (se eliminará)

### 3. **Otras directivas**

**Entrada:**
```c
#include <stdio.h>
#ifndef MAX
#endif
```

**Procesamiento:**
- Ninguna es `#define`
- Todas se eliminan de la salida (con `continue`)

### 4. **Preservación de saltos de línea**

**Clave:** Hacemos una **copia** de la línea antes de aplicar `trim`:

```c
char line_copy[MAX_LINE_LENGTH];
strcpy(line_copy, line);      // Copiar
char *trimmed = trim(line_copy);  // Modificar la copia
```

Así, `line` mantiene el `\n` original y cuando llamamos a `replace_defines_in_line(line, ...)`, el salto de línea se preserva en la salida.

---

## Resumen del Flujo Completo

```
┌─────────────────────┐
│  Archivo de entrada │
│   (example.h)       │
└──────────┬──────────┘
           │
           ▼
    ┌─────────────┐
    │   fgets()   │ Lee línea por línea
    └──────┬──────┘
           │
           ▼
    ┌──────────────────┐
    │ ¿Empieza con #?  │
    └──────┬───────────┘
           │
      ┌────┴────┐
      │   SÍ    │   NO
      ▼         ▼
┌──────────┐  ┌─────────────────────┐
│ #define? │  │ replace_defines_    │
└────┬─────┘  │ in_line()           │
     │        └──────────┬──────────┘
     │                   │
     ▼                   ▼
┌──────────┐      ┌──────────────┐
│add_define│      │    fputs()   │
│(tabla)   │      │ → archivo    │
│NO escribir│     │   de salida  │
└──────────┘      └──────────────┘
```

---

## Puntos Clave para Explicar a tus Compañeros

1. **Tabla de símbolos:** Usamos un array estático para almacenar los `#define` que vamos encontrando.

2. **Dos pasadas conceptuales:** 
   - Primera pasada implícita: guardar todos los `#define`
   - Segunda pasada implícita: reemplazar en el código
   - En realidad se hace en **una sola pasada** porque procesamos línea por línea secuencialmente.

3. **Limitación:** Solo funciona si los `#define` aparecen **antes** de su uso en el archivo (que es lo normal en C).

4. **Manejo de memoria:** Usamos `malloc` para almacenar nombres y valores, y `free_defines()` al final para liberar la memoria.

5. **Preservación de formato:** La copia de la línea antes de `trim` es crucial para mantener los saltos de línea.

6. **Macros complejas:** No expandimos macros con parámetros completamente, solo almacenamos el texto tal cual (simplificación para este proyecto).

---

## Compilación y Prueba

```bash
# Compilar
gcc arguments_menu.c file_utils.c string_utils.c comments.c directives.c errors_handling.c main.c -o preprocessor

# Probar
./preprocessor -d input-code-example/input-example.h

# Ver resultado
cat input-code-example/input-example_pp.h
```

---

**Fin del documento**

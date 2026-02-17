# Arquitectura del Código (P3 Bottom-Up Parser)

## 1. Objetivo arquitectónico
El proyecto está diseñado como un parser **bottom-up shift/reduce genérico** en C, separado en módulos para:

- cargar la especificación de lenguaje (símbolos, producciones, ACTION/GOTO),
- cargar tokens de entrada,
- ejecutar el autómata shift/reduce con pila,
- y generar trazas de depuración.

La idea central es: **el motor no cambia**, solo cambia el fichero de lenguaje.

## 2. Capas y módulos

### 2.1 Capa de orquestación
- `src/main.c`
- `src/context.h`

Responsabilidad:
- parsear argumentos CLI,
- construir y liberar componentes,
- invocar el parser,
- devolver el estado final (`PARSE_ACCEPTED`, `PARSE_REJECTED`, `PARSE_INTERNAL_ERROR`).

### 2.2 Capa de especificación de lenguaje
- `src/language/language.h`
- `src/language/language.c`

Responsabilidad:
- modelar símbolos, producciones y tablas del parser,
- cargar `language1.txt`/`language2.txt`,
- ofrecer consultas `get_action(...)`, `get_goto_state(...)`, `find_symbol_id(...)`.

### 2.3 Capa de tokens de entrada
- `src/token/token.h`
- `src/token/token.c`
- `src/token/token_loader.h`
- `src/token/token_loader.c`

Responsabilidad:
- mantener un `TokenStream` dinámico,
- leer `.cscn` en formato TSV (`lexeme<TAB>category`),
- mapear tokens del scanner a terminales de gramática (`NUM`, `+`, `*`, `(`, `)`, `int`),
- añadir token EOF (`$`) al final.

### 2.4 Capa de estructura de pila
- `src/stack/parse_stack.h`
- `src/stack/parse_stack.c`

Responsabilidad:
- implementar `ParseStack` (push/pop/peek),
- exponer representación textual de la pila (`format_stack`) para trazas.

### 2.5 Capa de ejecución del parser
- `src/parser/parser.h`
- `src/parser/parser.c`

Responsabilidad:
- implementar el algoritmo shift/reduce:
  - estado inicial en pila,
  - consulta ACTION por `(estado, lookahead)`,
  - `SHIFT`: empuja terminal + nuevo estado y avanza input,
  - `REDUCE`: desapila `|rhs|`, consulta GOTO y empuja LHS,
  - `ACCEPT`/`ERROR`: finaliza.

### 2.6 Capa de trazabilidad
- `src/trace/trace.h`
- `src/trace/trace.c`

Responsabilidad:
- crear `<input>_p3dbg.txt`,
- registrar cada transición del parser:
  - `SHIFT`, `REDUCE`, `ACCEPT`, `ERROR`,
  - posición de input, estados, regla aplicada y pila.

### 2.7 Utilidades comunes
- `src/utils/string_utils.h`
- `src/utils/string_utils.c`
- `src/utils/file_utils.h`
- `src/utils/file_utils.c`
- `src/language_defs.h`

Responsabilidad:
- helpers de strings y path/output,
- constantes globales del proyecto.

## 3. Flujo de datos de extremo a extremo
1. `main` recibe: `./p3parser <input.cscn> <language.txt>`.
2. `language` carga símbolos, reglas y tablas.
3. `token_loader` carga `.cscn`, mapea terminales y añade `$`.
4. `trace` abre `<input>_p3dbg.txt`.
5. `parser` ejecuta shift/reduce usando:
   - `LanguageSpec` (ACTION/GOTO),
   - `TokenStream`,
   - `ParseStack`,
   - `TraceWriter`.
6. `main` imprime resultado y libera memoria.

## 4. Decisiones de diseño relevantes
- Arquitectura modular por responsabilidad única.
- `LanguageSpec` desacopla gramática del motor.
- Estructuras dinámicas (`TokenStream`, `ParseStack`) para evitar límites rígidos.
- Trazabilidad obligatoria en cada modificación de estado/pila para depuración.
- Código en C puro, consistente con `p1pp`/`p2lx`.

## 5. Extensibilidad
Para soportar otro lenguaje:
- crear nuevo fichero `resources/languageX.txt` con `SYMBOLS/PRODUCTIONS/ACTION/GOTO`,
- ajustar mapeos de `token_loader` solo si cambian terminales del scanner,
- reutilizar el mismo binario sin cambios en `parser.c`.

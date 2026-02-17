# Resumen General del Código (P3 Bottom-Up Parser)

## ¿Qué contiene el proyecto?
El proyecto `p3bottomup` contiene:

- código fuente C modular en `src/`,
- ficheros de lenguaje en `resources/`,
- casos de prueba `.cscn` en `test_files/`,
- script de compilación `build_local.sh`,
- documentación base (`README.md` + esta documentación).

## ¿Qué hace?
Implementa un parser sintáctico **bottom-up shift/reduce** que:

1. lee una secuencia de tokens desde un fichero `.cscn`,
2. carga una especificación de lenguaje (gramática + tablas),
3. ejecuta el autómata shift/reduce con pila,
4. decide si la entrada pertenece o no al lenguaje,
5. y genera un fichero de depuración `<input>_p3dbg.txt`.

Salida principal:
- `PARSE_ACCEPTED` si la expresión es válida,
- `PARSE_REJECTED` si no pertenece al lenguaje,
- `PARSE_INTERNAL_ERROR` si hay fallo interno.

## ¿Cómo funciona?

### 1. Entrada por línea de comandos
Comando:

```bash
./p3parser <input.cscn> <language.txt>
```

Ejemplos de lenguaje:
- `resources/language1.txt` (gramática aritmética con `NUM`, `+`, `*`, paréntesis).
- `resources/language2.txt` (gramática `e -> e + ( e ) | int`).

### 2. Carga de lenguaje
El módulo `language` lee:
- símbolos terminales/no terminales,
- producciones,
- tabla ACTION,
- tabla GOTO.

Con ello construye `LanguageSpec`, que es la base de decisiones del parser.

### 3. Carga de tokens
El módulo `token_loader` lee `.cscn` en TSV:

```text
lexeme<TAB>category
```

Mapea categorías del scanner a terminales de la gramática (por ejemplo `CAT_NUMBER -> NUM`) y añade EOF (`$`).

### 4. Ejecución shift/reduce
El módulo `parser`:
- inicia la pila con estado 0,
- consulta ACTION con el estado superior y lookahead,
- aplica `SHIFT` o `REDUCE` según corresponda,
- usa GOTO después de cada reducción,
- termina en `ACCEPT` o `ERROR`.

### 5. Depuración
Cada paso se registra en `_p3dbg.txt` con formato:

```text
STEP | OP | INPUT_POS | LOOKAHEAD | PREV_STATE | NEW_STATE | RULE | STACK
```

Esto permite seguir la ejecución completa paso a paso.

## Idea clave del diseño
El motor de parsing es **genérico**: para cambiar de lenguaje no se toca el código del parser, solo el fichero de especificación (`languageX.txt`) y, si hiciera falta, el mapeo de tokens.

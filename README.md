# cbf

A Brainfuck compiler and interpreter written in C. The project includes a static analyzer, interpreter, and x86-64 compiler (NASM).

<br/>

## Usage

```
./cbf hello_world.bf -x86 // specify an execution target
```

<br/>

## Runtime Flags

| Flag         | Description                          |
|--------------|--------------------------------------|
| `-interpret` | Run using the built-in interpreter   |
| `-x86`       | Compile to x86-64 and execute        |
| `-d`         | Enable debug mode (lexer/AST dump)   |

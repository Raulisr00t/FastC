# FastC

**FastC** is a lightweight REPL (Read-Eval-Print Loop) for the C programming language — built on top of [TinyCC (TCC)](https://bellard.org/tcc/). It lets you enter and evaluate C code **interactively**, much like Python’s `exec()` or Haskell's GHCi.

---

## Features

- 🔁 Interactive REPL for writing C live in terminal
- 🧠 Remembers variables and declarations across inputs
- 🛠️ Uses TCC for Just-in-Time compilation
- 📚 Supports standard C functions like `printf`, `malloc`, `free`, etc.
- 🔐 Safe isolation of user input and error reporting
- 🧪 Great for learning, quick testing, or experimenting with C

---

## Installation

### Dependencies

Make sure you have **TinyCC** installed:

```bash
sudo apt update
sudo apt install tcc libtcc-dev
```

### Clone and Build
```bash
git clone https://github.com/yourusername/FastC.git
cd FastC
gcc FastCCompiler.c -ltcc -ldl -o fastc
```

## Usage
```powershell
./fastc
```
You'll enter an interactive prompt:

```powershell
Welcome to FastC REPL (Ctrl+C to exit, :q to quit)
FastC>> int x = 10;
FastC>> printf("%d\n", x);
10
FastC>> void *mem = malloc(40);
FastC>> free(mem);
FastC>> :q
```

### Supported Commands

:q or :quit — exit the REPL

Standard C code, including:

Declarations: int x = 10;

Expressions: x + 10

Functions: printf(...), malloc(...), etc.

Structs, typedefs, includes (if supported)

## How It Works

FastC maintains a growing "program buffer" that collects all valid declarations. Each new line is wrapped in a void run() { ... } function and compiled by TCC into memory. This allows real-time execution with persistent state across multiple lines.

## Why?

C is traditionally a compiled language. There's no built-in REPL like Python or Ruby. FastC brings that experience to C — great for learning, debugging, or playing around with code without writing full files.

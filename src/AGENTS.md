# Repository Guidelines

## Project Structure & Module Organization
- `src/` holds all interpreter sources. Entry point is `src/main.cpp`.
- Core components live at the top level of `src/` (e.g., `lox.cpp`, `expr.cpp`, `printAst.cpp`).
- Tokenization lives in `src/scanner/` (e.g., `scanner.cpp`, `token.cpp`).
- Utilities and helpers live in `src/utils/` (e.g., `astPrinter.cpp`).
- Build artifacts are generated in `build/` at the repo root.

## Build, Test, and Development Commands
- `./your_program.sh` builds and runs the interpreter locally (uses CMake + vcpkg).
- `cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake` configures the build.
- `cmake --build ./build` compiles the `interpreter` binary into `build/`.
- `./build/interpreter <args>` runs the binary directly after a build.

## Coding Style & Naming Conventions
- Language: C++23 (see `CMakeLists.txt`).
- Indentation: follow existing files; currently 2 spaces per indent in `src/`.
- Filenames are lowercase with `.cpp`/`.hpp` pairs (e.g., `scanner.hpp`).
- Prefer descriptive class/type names that mirror Crafting Interpreters terms (e.g., `Token`, `Scanner`).

## Testing Guidelines
- No formal test framework is set up yet.
- Use `./your_program.sh` with sample inputs (e.g., `test.lox`) to validate behavior.
- Keep quick, manual checks focused on the stage being implemented.

## Commit & Pull Request Guidelines
- Commit history uses lightweight conventional prefixes like `feat:`, `refactor:`, `test`, and `init`.
- Keep commits small and stage-focused (e.g., `feat: scan parentheses`).
- For PRs, include a short summary, the stage/section of the book targeted, and any manual checks run.

## Scope & Learning Focus
- This repo follows the Crafting Interpreters book; keep changes aligned with current stage goals.
- Avoid adding new features or optimizations beyond what the stage requires.
- When in doubt, prefer small, educational changes that mirror the Java reference implementation.



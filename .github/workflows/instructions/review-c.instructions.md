Here are the instructions for reviewing C code in this project.

- Verify that the code follows `SPECIFICATION.md` and satisfies all functional requirements.

- Respect the existing code style and formatting. If you notice inconsistencies, point them out in your review.
- Point out style or formatting issues that conflict with generally accepted C conventions, unless the current style clearly improves readability or maintainability.
- Check for clear and descriptive naming of variables, functions, and types. Suggest improvements if names are ambiguous or misleading.
- Point out any code that is difficult to understand or maintain, and suggest ways to improve clarity and readability.
- Check whether the code follows standard C (for example, C89/C99) and common best practices.
- Check whether the code can be compiled by a standard C compiler with warnings enabled, without warnings or errors.

- Look for potential memory leaks, especially when using dynamic memory allocation (`malloc`, `calloc`, `realloc`).
- Ensure every owned resource is released on all paths (memory, file descriptors, and open `FILE*` handles).
- Check error handling for library and system calls, including return value checks and clear failure behavior.
- Look for potential buffer overflows, especially with functions such as `strcpy`, `strcat`, or `sprintf`. Suggest safer alternatives such as `snprintf`, bounded copies, and explicit length checks.
- Check pointer safety: initialization before use, null checks where needed, and no invalid dereferences.
- Check array/string boundary handling for off-by-one errors and missing null terminators.
- Check for integer overflow/underflow and risky signed/unsigned conversions in size, index, and length calculations.
- Flag undefined behavior risks (for example: use-after-free, double free, out-of-bounds access, and uninitialized reads).

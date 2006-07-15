#ifndef DIRT_READER_IMPLEMENTOR
#define DIRT_READER_IMPLEMENTOR

#define NEWLINE '\n': case '\r'
#define SPACE '\t': case '\x0b': case '\x0c': case ' '
#define WHITESPACE NEWLINE: case SPACE
#define DIGITS '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'
#define NUMBERLEADCHARS DIGITS: case '+': case '-'
#define NUMBERCHARS NUMBERLEADCHARS: case '.'
#define KNOWNBEGINSEPARATORS '<': case '{': case '['
#define BEGINSEPARATORS KNOWNBEGINSEPARATORS: case '('
#define ENDSEPARATORS ')': case ']': case '}': case '>'
#define SEPARATORS ',': case ':': case '=': case BEGINSEPARATORS: case ENDSEPARATORS

#endif

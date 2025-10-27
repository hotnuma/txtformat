#ifndef TXTPARSER_H
#define TXTPARSER_H

#include <stdbool.h>

typedef struct _TxtParser TxtParser;

TxtParser* parser_new();
void parser_free(TxtParser *parser);

bool parser_read(TxtParser *parser, const char *inpath);
bool parser_unbreak(TxtParser *parser);

#endif // TXTPARSER_H



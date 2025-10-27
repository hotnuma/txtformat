#include "txtparser.h"

#include <cfile.h>
#include <libpath.h>
#include <print.h>

struct _TxtParser
{
    CString *outdir;
    CFile *file;
    CString *buffer;
    CString *result;
};

TxtParser* parser_new()
{
    TxtParser *parser = (TxtParser*) malloc(sizeof(TxtParser));

    parser->outdir = cstr_new_size(64);
    parser->buffer = NULL;
    parser->file = cfile_new();

    return parser;
}

void parser_free(TxtParser *parser)
{
    cstr_free(parser->outdir);
    cfile_free(parser->file);
    free(parser);
}

bool parser_read(TxtParser *parser, const char *inpath)
{
    path_dirname(parser->outdir, inpath);

    if (!cfile_read(parser->file, inpath))
        return false;

    CString *result = cfile_buffer(parser->file);
    if (parser->buffer == NULL)
        parser->buffer = cstr_new_size(cstr_capacity(result) + 1);

    return true;
}

bool parser_unbreak(TxtParser *parser)
{
    CString *buffer = parser->buffer;
    CString *result = cfile_buffer(parser->file);
    cstr_swap(buffer, result);
    cstr_clear(result);

    char *p = cstr_data(buffer);

    while (*p != '\0')
    {
        if (*p != '\n' && *(p+1) == '\n')
        {
            if (*p == ' ')
            {
                cstr_append_c(result, '\n');
                p += 2;
                continue;
            }
        }
        else if (*p == '\n' && *(p+1) == '\n')
        {
            ++p;
            continue;
        }


        cstr_append_c(result, *p);
        ++p;
    }

    print(c_str(result));

    return true;
}



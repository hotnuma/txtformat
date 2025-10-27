#include <cfile.h>
#include <libpath.h>
#include <ctype.h>
#include <print.h>

typedef struct _TxtParse TxtParse;

struct _TxtParse
{
    CString *outdir;
    CString *outpath;
    int fcount;
    CFile *file;
    CString *buffer;
    CString *result;
};

bool tparse_unbreak(TxtParse *parser);

TxtParse* tparse_new()
{
    TxtParse *parser = (TxtParse*) malloc(sizeof(TxtParse));

    parser->outdir = cstr_new_size(64);
    parser->outpath = cstr_new_size(128);
    parser->fcount = 0;
    parser->buffer = NULL;
    parser->file = cfile_new();

    return parser;
}

void tparse_free(TxtParse *parser)
{
    cstr_free(parser->outdir);
    cstr_free(parser->outpath);
    cfile_free(parser->file);
    free(parser);
}

bool tparse_read(TxtParse *parser, const char *inpath)
{
    path_dirname(parser->outdir, inpath);

    if (!cfile_read(parser->file, inpath))
        return false;

    CString *result = cfile_buffer(parser->file);
    if (parser->buffer == NULL)
        parser->buffer = cstr_new_size(cstr_capacity(result) + 1);

    return true;
}

static CString* _tparse_getoutpath(TxtParse *parser)
{
    CStringAuto *fname = cstr_new_size(32);
    cstr_fmt(fname, "out%d.txt", ++(parser->fcount));
    path_join(parser->outpath, c_str(parser->outdir), c_str(fname));
    return parser->outpath;
}

bool tparse_writefile(TxtParse *parser, CString *result)
{
    CString *outpath = _tparse_getoutpath(parser);

    print("write to : %s", c_str(outpath));

    CFileAuto *file = cfile_new();
    if (!cfile_open(file, c_str(outpath), "wb"))
        return false;

    cfile_write(file, c_str(result));
    cfile_close(file);

    return true;

}

char tparse_getprev(TxtParse *parser, char *current)
{
    char ret = 0;

    if (current > cstr_data(parser->buffer))
        return *(current - 1);

    return ret;
}

static int _app_exit(bool usage, int ret)
{
    if (usage)
    {
        print("*** usage");
        print("txtparse -i \"filepath\"");
    }

    return ret;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return _app_exit(true, EXIT_FAILURE);

    int n = 1;

    const char *inpath = NULL;

    while (n < argc)
    {
        const char *part = argv[n];

        if (strcmp(part, "-i") == 0)
        {
            if (++n >= argc)
                return _app_exit(true, EXIT_FAILURE);

            inpath = argv[n];
        }
        else
        {
            return _app_exit(true, EXIT_FAILURE);
        }

        ++n;
    }

    if (!inpath)
        return EXIT_FAILURE;

    TxtParse *parser = tparse_new();

    if (!tparse_read(parser, inpath))
    {
        tparse_free(parser);
        return false;
    }

    tparse_unbreak(parser);

    tparse_free(parser);

    return EXIT_SUCCESS;
}

bool tparse_unbreak(TxtParse *parser)
{
    CString *buffer = parser->buffer;
    CString *result = cfile_buffer(parser->file);
    cstr_swap(buffer, result);
    cstr_clear(result);

    char *p = cstr_data(buffer);

    while (*p != '\0')
    {
        if (*p == '-' && *(p+1) == '\n' && *(p+2) != '\n')
        {
            p+=2;
            continue;
        }
        else if (*p == '\n')
        {
            if (*(p+1) != '\n')
            {
                cstr_append_c(result, ' ');

                p+=1;
                continue;
            }
            else if (*(p+1) == '\n')
            {
                cstr_append(result, "\n");

                while (*p == '\n' || *p == ' ') ++p;
                continue;
            }
        }

        cstr_append_c(result, *p);
        ++p;
    }

    tparse_writefile(parser, result);

    return true;
}



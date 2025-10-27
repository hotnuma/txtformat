#if 0

char parser_getprev(TxtParser *parser, char *current)
{
    char ret = 0;

    if (current > cstr_data(parser->buffer))
        return *(current - 1);

    return ret;
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

    print(c_str(result));

    return true;
}

bool parse2(CString *buffer, CString *result)
{
    cstr_swap(buffer, result);
    cstr_clear(result);
    char *p = cstr_data(buffer);

    char *c = NULL;

    while (*p != '\0')
    {
        if (*p == '-' && *(p+1) == '\n' && isdigit(*(p+2)))
        {
            c = p + 2;

            while (isdigit(*c)) ++c;
            if (*c == ' ' && *(c+1) == 12)
            {
                p = c+2;
            }
            else
            {
                cstr_append_c(result, *p);
                ++p;
            }

            continue;
        }
        else if (*p == '\n' && isdigit(*(p+1)))
        {
            c = p + 1;

            while (isdigit(*c)) ++c;
            if (*c == ' ' && *(c+1) == 12)
            {
                p = c+2;
                cstr_append_c(result, ' ');
            }
            else
            {
                cstr_append_c(result, *p);
                ++p;
            }

            continue;
        }

        cstr_append_c(result, *p);
        ++p;
    }

    _write_file(result);

    return true;
}

static CString* _parser_getoutpath(TxtParser *parser)
{
    CStringAuto *fname = cstr_new_size(32);
    cstr_fmt(fname, "out%d.txt", ++(parser->fcount));
    path_join(parser->outpath, c_str(parser->outdir), c_str(fname));
    return parser->outpath;
}

bool parser_writefile(TxtParser *parser, CString *result)
{
    CString *outpath = _parser_getoutpath(parser);

    print("write to : %s", c_str(outpath));

    CFileAuto *file = cfile_new();
    if (!cfile_open(file, c_str(outpath), "wb"))
        return false;

    cfile_write(file, c_str(result));
    cfile_close(file);

    return true;

}

#endif



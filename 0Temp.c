#if 0

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


#endif



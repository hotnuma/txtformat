#include <cfile.h>
#include <libpath.h>
#include <ctype.h>
#include <print.h>

static int _app_exit(bool usage, int ret)
{
    if (usage)
    {
        print("*** usage");
        print("app -i \"filepath\"");
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

    CStringAuto *outpath = cstr_new(inpath);
    path_strip_ext(outpath, true);
    cstr_append(outpath, "-new.txt");

    CFileAuto *file = cfile_new();
    if (!cfile_read(file, inpath))
        return EXIT_FAILURE;

    if (!cfile_open(file, c_str(outpath), "wb"))
        return EXIT_FAILURE;

    CString *buffer = cfile_buffer(file);
    char *p = cstr_data(buffer);
    CStringAuto *result = cstr_new_size(strlen(p) + 1);

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

    cstr_swap(buffer, result);
    cstr_clear(result);
    p = cstr_data(buffer);
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

    cfile_write(file, c_str(result));

    return EXIT_SUCCESS;
}



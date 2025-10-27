#include <stdlib.h>
#include <string.h>
#include <print.h>

#include "txtparser.h"

static void usage_exit()
{
    print("*** usage");
    print("txtformat -i \"filepath\"");
    print("abort...");

    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc < 2)
        usage_exit();

    int n = 1;

    const char *inpath = NULL;

    while (n < argc)
    {
        const char *part = argv[n];

        if (strcmp(part, "-i") == 0)
        {
            if (++n >= argc)
                usage_exit();

            inpath = argv[n];
        }
        else
        {
            usage_exit();
        }

        ++n;
    }

    if (!inpath)
        usage_exit();

    TxtParser *parser = parser_new();

    if (!parser_read(parser, inpath))
    {
        parser_free(parser);
        return false;
    }

    parser_unbreak(parser);

    parser_free(parser);

    return EXIT_SUCCESS;
}


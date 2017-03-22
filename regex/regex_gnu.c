#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#define NUM 3

int main (int argc, const char * argv[])
{
    if (argc < 3) {
        fprintf (stderr, "%s pattern text\n", argv[0]);
        return -1;
    }

    const char * pattern = argv[1];
    const char * text    = argv[2];
    char err_msg[2014]   = {0};
    char match[1024]     = {0};
    size_t  err_len      = 0;
    regex_t preg;
    regmatch_t pmatch[NUM];
    int reg_code;
    int i;

    reg_code = regcomp (&preg, pattern, REG_EXTENDED);
    if (reg_code != 0) goto end;

    reg_code = regexec (&preg, text, NUM, pmatch, 0);
    
    if (reg_code == REG_NOMATCH) {
        printf ("[%s] not matches [%s]\n", text, pattern);
    }
    else if (reg_code != 0) goto end;

    for (i = 0; i < NUM; ++i) {
        memset (match, '\0', sizeof(match));
        memcpy (match, text + pmatch[i].rm_so, pmatch[i].rm_eo - pmatch[i].rm_so);
        printf ("%d: [%s]\n", i, match);
    }

end:
    if (reg_code != 0) {
        err_len = regerror (reg_code, &preg, err_msg, sizeof (err_msg));         
        fprintf (stderr, "reg err: %s\n", err_msg);
    }

    regfree (&preg);

    return reg_code;
}

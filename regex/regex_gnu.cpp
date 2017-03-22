#include <regex.h>
#include <sys/types.h>
#include <sstream>
#include <stdio.h>
#include <memory>

#define NUM 5

using namespace std;

void print_match (const char * text, const regmatch_t * pmatch)
{
    shared_ptr <unsigned char> sp_buf;
    for (int i = 0; i < NUM; ++i) {
        size_t len = pmatch[i].rm_eo - pmatch[i].rm_so;
        if (len) {
            sp_buf.reset ((unsigned char *) malloc (len), free);
            memcpy (sp_buf.get (), text + pmatch[i].rm_so, len);
            sp_buf.get ()[len] = '\0';
            printf ("[%s]\n", sp_buf.get ());
        }
    }
}

bool can_match (regex_t * reg, const char * pattern, const char * text, regmatch_t * pmatch)
{
    int reg_code = regcomp (reg, pattern, REG_EXTENDED);
    if (reg_code != 0) 
        return false;
    
    reg_code = regexec (reg, text, NUM, pmatch, 0);
     
    if (reg_code != 0 && reg_code != REG_NOMATCH) {
        char err_msg[256] = {0};
        size_t err_len = regerror (reg_code, reg, err_msg, sizeof (err_msg));
        err_len = err_len < sizeof (err_msg) ? err_len : sizeof (err_msg) - 1;
        err_msg[err_len] = '\0';
        fprintf (stderr, "reg err: [%s]\n", err_msg);
    }

    regfree (reg);

    return reg_code == 0 ? true : false;
}

int main (int argc, char * argv[])
{
    ostringstream oss;
    oss << "ffmpeg -i \"" << argv[1] << "\" 2>&1";

    shared_ptr <FILE> sp (popen (oss.str().c_str(), "r"), pclose);

    const char * p_duration ("Duration: ([0-9]{2}):([0-9]{2}):([0-9]{2})"); // Duration: 01:32:47.03
    const char * p_video ("Stream #0:([0-9]{1,})(.*)Video"); // Stream #0:0(und): Video
    const char * p_wxh ("([0-9]{2,})x([0-9]{2,}) \\[SAR"); // 320x240 [SAR
    const char * p_fps ("([0-9]{1,})(.[0-9]{1,})* fps"); // "25 fps" or "29.97 fps"
    // Stream #0:1(chi): Audio: dts (DTS), 48000 Hz, 5.1(side), fltp, 1536 kb/s (default)
    // Stream #0:2(chi): Audio: ac3, 48000 Hz, 5.1(side), fltp, 640 kb/s (default)
    const char * p_audio ("Stream #0:([0-9]{1,})(.*): Audio: (.*), ([0-9]{2,}) Hz");

    regex_t reg;
    regmatch_t pmatch[NUM];

    char * line                             = NULL;
    size_t len                              = 0;
    ssize_t count                           = 0;
    string str;

    while ((count = getline (&line, &len, sp.get ())) != -1) {
        if (can_match (&reg, p_duration, line, pmatch)) 
            print_match (line, pmatch);
        if (can_match (&reg, p_video, line, pmatch)) 
            print_match (line, pmatch);
        if (can_match (&reg, p_wxh, line, pmatch)) 
            print_match (line, pmatch);
        if (can_match (&reg, p_fps, line, pmatch)) 
            print_match (line, pmatch);
        if (can_match (&reg, p_audio, line, pmatch)) 
            print_match (line, pmatch);
    }

    if (line)
        free (line);

    return 0;
}

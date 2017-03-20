#include <regex>
#include <sstream>
#include <stdio.h>
#include <memory>

using namespace std;

int main (int argc, char * argv[])
{
    ostringstream oss;
    oss << "ffmpeg -i \"" << argv[1] << "\" 2>&1";

    shared_ptr <FILE> sp (popen (oss.str().c_str(), "r"), pclose);

    smatch m;
    regex e_duration ("Duration: (\\d{2}):(\\d{2}):(\\d{2})"); // Duration: 01:32:47.03
    regex e_video ("Stream #0:(\\d{1,})(.*)Video"); // Stream #0:0(und): Video
    regex e_wxh ("(\\d{2,})x(\\d{2,}) \\[SAR"); // 320x240 [SAR
    regex e_fps ("(\\d{1,})(\\.\\d{1,})* fps"); // "25 fps" or "29.97 fps"

     // Stream #0:1(chi): Audio: dts (DTS), 48000 Hz, 5.1(side), fltp, 1536 kb/s (default)
     // Stream #0:2(chi): Audio: ac3, 48000 Hz, 5.1(side), fltp, 640 kb/s (default)
     regex e_audio ("Stream #0:(\\d{1,})(.*): Audio: (.*), (\\d{2,}) Hz");

     char * line                             = NULL;
     size_t len                              = 0;
     ssize_t count                           = 0;
     string str;

     while ((count = getline (&line, &len, sp.get ())) != -1) {
         str                                 = line;
         if (regex_search (str, m, e_duration)) {
             printf ("%s\n", m.str (0).c_str ());
         }

         if (regex_search (str, m, e_video)) {
             printf ("%s\n", m.str (0).c_str ());
         }
         if (regex_search (str, m, e_wxh)) {
             printf ("%s\n", m.str (0).c_str ());
         }
        if (regex_search (str, m, e_fps)) {
             printf ("%s\n", m.str (0).c_str ());
        }
        if (regex_search (str, m, e_audio)) {
             printf ("%s\n", m.str (0).c_str ());
        }
    }

    if (line)
        free (line);

    return 0;
}

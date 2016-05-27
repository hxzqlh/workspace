#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;

string hex_escape (const char *s) {
    unsigned int len = strlen (s);
    string res (len * 2 + 1, '\0');
    unsigned int i = 0;
    for (i = 0; i < len; ++i) {
        snprintf (&res[i*2], 3, "%02x", (unsigned char)s[i]);
    }
    res.resize (len*2);
        
    return res;         
} 

int encode_filename (const char* before_encode_str,
        char* after_encode_str, const char* key)
{
    printf ("before:[%s]", before_encode_str);
    int ret = 0;
    unsigned int i = 0;

    unsigned int length  = strlen(before_encode_str);
    unsigned int key_length = strlen(key);
    char res_str [256];
    memset (res_str, '\0', 256);

    for(i = 0;i < length;i++)
    {
        int j = i % key_length;
        int t = before_encode_str[i] ^ key[j];
        res_str[i]=(char)t;
    }
    res_str[length] = '\0';
    printf ("res_str:[%s]", res_str);
    strcpy (after_encode_str, hex_escape (res_str).c_str ());
    printf ("after:[%s]", after_encode_str);
    return ret;
}

string encode_string (const string &before_encode_str, const string &key)
{
    char encode_str[256];
    encode_filename (before_encode_str.c_str (), encode_str, key.c_str ());
    return encode_str;
}     

int main (int argc, const char* argv[])
{
    string key = "2015-03-23 07:52:17 UTC";
    string before = argv[1];

    string res = encode_string (before, key);
    return 0;
}

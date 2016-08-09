#include <iostream>
using namespace std;


struct product_t {
    char * id;
    int desc_index;
};

const char * desc[] = {
    "description_1",
    "description_2",
    "description_3",
    "description_4",
    "description_5"
};

const product_t goods [] = {
    {"id_1", 2},
    {"id_2", 1},
    {"id_3", 3},
    {"id_4", 4},
    {"id_5", 2},
    {"id_6", 4},
    {"id_7", 5},
    {"id_8", 3},
    {"id_9", 2},
    {"id_10", 5}
};

const char* desc_product (const char* id) {
    for (const product_t & p : goods) {
        if (strcmp(p.id, id) == 0) {
            return desc[p.desc_index - 1];
        }
    }
    
    return NULL;
}


int main () 
{
    char prod_id[128];
    cout << "input product ID:";

    while (cin >> prod_id) {
        
        const char* desc =  desc_product (prod_id);
        if (desc) 
            cout << desc << endl;
        else
            cout << "no such product!" << endl;
        memset (prod_id, 0, sizeof(prod_id));
    }

    return 0;
}

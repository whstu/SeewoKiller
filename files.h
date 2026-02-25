#ifndef FILES_H
#define FILES_H

#include "./main.h"
using namespace std;
extern bool fileExist(const string& filename);
extern int SearchForAddress(string value[], string goal);
extern string read_config(string PATH);
extern void write_config(string PATH, string config);
extern void check_config_avaliable(string PATH,string config[],int config_n,string default_config);
extern void change_word(string string_class,int address,bool config=false,string PATH="NULL",string name="NULL");
#define TF {"true","false"}

#endif

#include <string>
#include <fstream>
#include <stdio.h>  
#include <stdlib.h> 
#include <unistd.h> 
#include <vector> 
#include <iostream> 
#include <json/json.h>
 
using namespace std;

 
main(int argc ,char **argv)
{
 
    int i,number;
    string p1,p2,p3;
    if (argc <2)  
    {
        cout << " give a jason filename\n";
        exit(1);
    }
    else 
    {
        p1 = argv[1];
    }
     
    Json::Reader jsonReader;
    Json::FastWriter writer;
    Json::Value jv;

    ifstream ifs;
//open file
    ifs.open(p1.c_str());
    jv.clear();
    if (!jsonReader.parse(ifs, jv)){
            ifs.close();
            cout << " parse error\n";
            exit(1);
    }
    ifs.close();
// write:
   string str;
   str =writer.write(jv);
   cout <<str<<endl;
}

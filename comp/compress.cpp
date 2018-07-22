#include <cstdio>
#include <iostream>
#include "d_compress.h"
#include "d_compress.cpp"
using namespace std;
int main(int argc,char *argv[])
{
   const char *s="a";
   if(argc==1){
     cout<<"please input filepath"<<endl;
     return 0;
   }
   comp obj(argv[1]);
   obj.count();
   obj.build();
   obj.encode();
   //cout<<"node info:"<<endl;
   //obj.nodeinfo();
   cout<<"encode over"<<endl;
   obj.comptofile(s);
   obj.decodetofile(s,"b");
   return 0;
}

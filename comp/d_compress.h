#ifndef D_COMPRESS_H
#define D_COMPRESS_H
#include <cstdio>
#include <vector>
#include <string>
using namespace std;

class comp{
  bool fof; 
  FILE *filepoint;
  vector<long> count_rec;
  //vector<float> count_t;
  //vector<string> ec;
  long long total;
  long long writetotal;
  vector<struct Node> node;
  int top;
  int maxencodelen;
  void encode_help(int n,unsigned int value,int len);
  int search(int n,char onebit);
public:
   comp(const char *filePath);
   const bool &getfof();
   void count();
   void build();
   int findmin();
   void encode();
   void nodeinfo();
   void comptofile(const char *filepath);
   void decodetofile(const char *ef,const char *df);
};

struct Node{
  int leftchild;
  int rightchild;
  unsigned int value;
  int len;
  float times;
  char watch;
  unsigned char c;
  //string code;
};
#endif

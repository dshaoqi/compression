#include "d_compress.h"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;
comp::comp(const char *filePath):count_rec(vector<long>(512,0)),node(vector<struct Node>(512))
   {
	filepoint=fopen(filePath,"rb");
	if(!filepoint){
		cout<<"fail to open the file"<<endl;
		fof=0;
		return;
	}
	fof=1;
	total=0;
	top=256;
        maxencodelen=0;
	cout<<"success to open the file"<<endl;
	/*cout<<node.size()<<endl;
	  for(int i=0;i<node.size();i++)
	  if(node[i].rightchild!=0)
	  cout<<"error"<<endl;
	 */
}

const bool &comp::getfof(){
	return fof;
}

void comp::count(){
	unsigned char buff[1024];
	int readcount=1024;
	int i=0;
	do{
		readcount=fread(buff,sizeof(char),1024,filepoint); 
		for(i=0;i<readcount;i++){
			count_rec[buff[i]]++;
		}
		total+=readcount;
	}while(!feof(filepoint));
	for(i=0;i<256;i++){
	        node[i].leftchild=node[i].rightchild=-1;
		if(count_rec[i]!=0){
			node[i].times=(float)count_rec[i]/total;
			node[i].c=i;
			cout<<i<<' '<<count_rec[i]<<' '<<node[i].times<<endl;
		}
	}
}

void comp::build(){
	int minloc1,minloc2;
	for(int i=0;i<256;i++){
		if(count_rec[i]==0)
			node[i].watch=1;
	} 
	while(1){ 
		minloc1=findmin();
		node[minloc1].watch=1;
		minloc2=findmin();
		if(minloc2==-1) break;
		node[minloc2].watch=1;
 
		node[top].leftchild=minloc1;
		node[top].rightchild=minloc2;
		node[top].times=node[minloc1].times+node[minloc2].times;
		top++;
	}
	cout<<node[top-1].times<<endl;
}
int comp::findmin(){
	int minloc=-1;
	float minvalue=1.1;
	for(int i=0;i<top;i++){
		if(node[i].watch==0&&node[i].times<minvalue){
			minvalue=node[i].times;
			minloc=i; 
		}
	}
        //cout<<"minloc"<<minloc<<endl;
	return minloc;
}


/*void comp::encode(){
	string s;
        int topnode=top-1;
        encode_help(topnode,s);	
}


void comp::encode_help(int n,string s){
	if(node[n].leftchild==-1){
	   node[n].code=s;
           cout<<(int)(node[n].c)<<' '<<s<<endl;
           return;
	}
	encode_help(node[n].leftchild,s+'0');
	encode_help(node[n].rightchild,s+'1');
}
*/
void comp::encode(){
        int topnode=top-1;
	unsigned int value=0;
	int len=0;
        encode_help(topnode,value,len);	
	cout<<"maxencodelen="<<maxencodelen<<endl;
}


void comp::encode_help(int n,unsigned int value,int len){
	if(node[n].leftchild==-1){
	   node[n].value=value;
           node[n].len=len;
	   maxencodelen=(len>maxencodelen? len:maxencodelen);
           cout<<(int)(node[n].c)<<' '<<value<<' '<<len<<endl;
           return;
	}
	encode_help(node[n].leftchild,value|(unsigned int)0<<len,len+1);
	encode_help(node[n].rightchild,value|(unsigned int)1<<len,len+1);
}



void comp::nodeinfo(){
	for(int i=0;i<top;i++)
             if(node[i].len!=0)
		cout<<"node:"<<i<<' '<<node[i].value<<' '<<node[i].len<<endl;
}


void comp::comptofile(const char *filepath){
	FILE *wf=fopen(filepath,"w");
	fseek(filepoint,0,0);
	unsigned char buffread[1024];
	unsigned char buffwrite[4096];
	memset(buffwrite,0,4096);
	int writebit=0;
        writetotal=0;
	while(1){
		int readnum=fread(buffread,sizeof(char),1024,filepoint);
                for(int i=0;i<readnum;i++){
                        //cout<<'i'<<i<<endl;
			int loc=writebit/8;
			int rear=writebit%8;
                        unsigned int writevalue=node[buffread[i]].value<<rear;
			//cout<<"l r w:"<<loc<<' '<<rear<<' '<<writevalue<<endl;
                	unsigned int *p=(unsigned int *)(buffwrite+loc);
			*p=*p|writevalue;
                        //cout<<*p<<endl;
			writebit+=node[buffread[i]].len;
		}
		fwrite(buffwrite,sizeof(char),writebit/8,wf);
                writetotal+=writebit/8;
                buffwrite[0]=buffwrite[writebit/8];
		writebit=writebit%8;
                memset(buffwrite+1,0,4095);
		if(feof(filepoint)==1){
			fwrite(buffwrite,sizeof(char),1,wf);
			writetotal+=1;
                        fclose(wf);
			fclose(filepoint);
                        return;
		}
	}
}


void comp::decodetofile(const char *encodefile,const char *decodefile){
	FILE *df=fopen(decodefile,"wb");
	FILE *ef=fopen(encodefile,"rb");
	unsigned char buffread[1024];
	unsigned char buffwrite[10240];
	int searchbit=0;
	long long wholewrite=0;
	unsigned int p;
	int nodenum=top-1;
	while(1){
		int readnum=fread(buffread,sizeof(char),1024,ef);
		int writeloc=0;
		memset(buffwrite,0,10240);
		while(1){
			if(searchbit==readnum*8) break;
			int loc=searchbit/8;
			unsigned char onebit=buffread[loc]>>(searchbit%8);
			nodenum=search(nodenum,onebit);
			if(node[nodenum].leftchild==-1){
				//cout<<node[nodenum].c<<endl;
				buffwrite[writeloc++]=node[nodenum].c;	
				nodenum=top-1;
				wholewrite++;
				if(wholewrite==total){
					fwrite(buffwrite,sizeof(char),writeloc,df);
					fclose(df);
					return;
				}
			}
			searchbit++;
		}
		fwrite(buffwrite,sizeof(char),writeloc,df);
	}
}


int comp::search(int nodenum,char onebit){
	if(onebit&1==1){
		return node[nodenum].rightchild;
	}	
	else{
		return node[nodenum].leftchild;
	}
}





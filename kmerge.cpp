#include <bits/stdc++.h>

using namespace std;
#define CLR(a) memset(a, 0, sizeof(a))
#define ABS(X) ( (X) > 0 ? (X) : ( -(X) ) )
#define SZ(V) (LL )V.size()
#define ALL(V) V.begin(), V.end()
#define RALL(V) V.rbegin(), V.rend()
#define FORN(i, n) for(i = 0; i < n; i++)
#define FORAB(i, a, b) for(i = a; i <= b; i++)
#define pll pair < long long int, long long int >
#define pii pair < int, int >
#define psi pair < string, int >
#define PB push_back  
#define MP make_pair
#define F first
#define S second

typedef pair<int,int> PII;
typedef pair<double, double> PDD;
typedef long long LL;

string folder_path;

string NumberToString ( long long int Number )
{
     ostringstream ss;
     ss << Number;
     return ss.str();
}

void merge(LL a,LL b){
	ofstream out;
	out.open((folder_path+"/prim_index").c_str());
	ifstream prim1;
	prim1.open((folder_path+NumberToString(a)+"/prim_index").c_str());
	// FILE *sec1=fopen((folder_path+NumberToString(a)+"/sec_index").c_str(),"r");
	ifstream prim2;
	prim2.open((folder_path+NumberToString(b)+"/prim_index").c_str());
	// FILE *sec2=fopen((folder_path+NumberToString(b)+"/sec_index").c_str(),"r");
	string curr1="",curr2="",post1="",post2="";
	prim1 >> curr1;prim1 >> post1;
	prim2 >> curr2;prim2 >> post2;
	while(true){
		if(curr1==curr2){
			out << curr1 << " " << post1 << post2 << endl;
			if(prim1 >> curr1){
				prim1 >> post1;
			}
			else{
				curr1="";
			}
			if(prim2 >> curr2){
				prim2 >> post2;
			}
			else{
				curr2="";
			}
			if(curr1=="" || curr2=="") {
				break;
			}
		}
		else if(curr1<curr2){
			out << curr1 << " " << post1 << endl;
			if(prim1 >> curr1){
				prim1 >> post1;
			}
			else{
				curr1="";
				break;
			}
		}
		else{
			out << curr2 << " " << post2 << endl;
			if(prim2 >> curr2){
				prim2 >> post2;
			}
			else{
				curr2="";
				break;
			}
		}
	}
	if(curr2!=""){
		out << curr2 << " " << post2 << endl;
		while(prim2 >> curr2){
			prim2 >> post2;
			out << curr2 << " " << post2 << endl;
		}
	}
	else if(curr1!=""){
		out << curr1 << " " << post1 << endl;
		while(prim1 >> curr1){
			prim1 >> post1;
			out << curr1 << " " << post1 << endl;
		}	
	}
	prim1.close();prim2.close();out.close();
}

pair<LL,string> get_pair(string &A){
	LL i,id=0;
	for(i=0;i<SZ(A);i++){
		if(A[i]==' '){
			i++;
			break;
		}
		id=id*10+A[i]-'0';
	}
	return MP(id,A.substr(i,SZ(A)));
}

void merge_doc(LL a,LL b){
	FILE *out;
	out=fopen((folder_path+"/doc_list").c_str(),"w");
	ifstream prim1;
	prim1.open((folder_path+NumberToString(a)+"/doc_list").c_str());
	// FILE *sec1=fopen((folder_path+NumberToString(a)+"/sec_index").c_str(),"r");
	ifstream prim2;
	prim2.open((folder_path+NumberToString(b)+"/doc_list").c_str());
	// FILE *sec2=fopen((folder_path+NumberToString(b)+"/sec_index").c_str(),"r");
	string line1="",line2="",post1="",post2="";
	LL doc1,doc2;
	pair <LL,string> p1,p2;
	getline(prim1,line1);
	getline(prim2,line2);
	p1=get_pair(line1);doc1=p1.first;post1=p1.second;
	p2=get_pair(line2);doc2=p2.first;post2=p2.second;
	while(true){
		if(doc1==doc2){
			LL i,num1=0,num2=0;
			for(i=0;i<SZ(post1);i++){
				if(post1[i]==' '){
					i++;
					break;
				}
				num1=num1*10+post1[i]-'0';
			}
			for(i=0;i<SZ(post2);i++){
				if(post2[i]==' '){
					i++;
					break;
				}
				num2=num2*10+post2[i]-'0';
			}
			fprintf(out, "%lld %s %s\n",doc1,(NumberToString(num1+num2)).c_str(),(post2.substr(i,SZ(post2))).c_str() );
			if(getline(prim1,line1)){
				if(line1==""){
					post1="";
					break;
				}
				p1=get_pair(line1);doc1=p1.first;post1=p1.second;
			}
			else{
				post1="";
				break;
			}
			if(getline(prim2,line2)){
				if(line2==""){
					post2="";
					break;
				}
				p2=get_pair(line2);doc2=p2.first;post2=p2.second;
			}
			else{
				post2="";
				break;
			}
		}
		else if(doc1<doc2){
			fprintf(out, "%lld %s\n",doc1,post1.c_str() );
			if(getline(prim1,line1)){
				if(line1==""){
					post1="";
					break;
				}
				p1=get_pair(line1);doc1=p1.first;post1=p1.second;
			}
			else{
				post1="";
				break;
			}
		}
		else{
			fprintf(out, "%lld %s\n",doc2,post2.c_str() );
			if(getline(prim2,line2)){
				if(line2==""){
					post2="";
					break;
				}
				p2=get_pair(line2);doc2=p2.first;post2=p2.second;
			}
			else{
				post2="";
				break;
			}
		}
	}
	if(post1!=""){
		fprintf(out, "%s\n", line1.c_str());
		while(getline(prim1,line1)){
			fprintf(out, "%s\n", line1.c_str());
		}
	}
	else if(post2!=""){
		fprintf(out, "%s\n", line2.c_str());
		while(getline(prim2,line2)){
			fprintf(out, "%s\n", line2.c_str());
		}
	}
	prim1.close();prim2.close();fclose(out);
}


int main(int argc,char* argv[]){
	if(argc>1){
		folder_path=argv[1];
		if(folder_path[folder_path.size()-1]!='/'){
			folder_path+="/";
		}
	}
	else{
		folder_path="./";
	}
	LL j,last;
	for(j=0;;j++){
		FILE *fp=fopen((folder_path+NumberToString(j)+"/prim_index").c_str(),"r");
		if(fp==NULL){
			j--;
			last=j;
			break;
		}
	}
	if(last==0){
		system(("mv "+ folder_path+"0/prim_index " + folder_path+"prim_index").c_str());
		system(("mv "+ folder_path+"0/doc_list " + folder_path+"doc_list").c_str());
		system(("rmdir "+ folder_path+"0").c_str());
		return 0;
	}
	merge(0,1);
	system(("mv "+ folder_path+"prim_index " + folder_path+NumberToString(0)+"/prim_index").c_str());
	FORAB(j,2,last){
		merge(0,j);
		system(("mv "+ folder_path+"prim_index " + folder_path+NumberToString(0)+"/prim_index").c_str());
	}
	system(("mv "+ folder_path+"0/prim_index " + folder_path+"prim_index").c_str());

	merge_doc(0,1);
	system(("mv "+ folder_path+"doc_list " + folder_path+NumberToString(0)+"/doc_list").c_str());
	FORAB(j,2,last){
		merge_doc(0,j);
		system(("mv "+ folder_path+"doc_list " + folder_path+NumberToString(0)+"/doc_list").c_str());
	}
	system(("mv "+ folder_path+"0/doc_list " + folder_path+"doc_list").c_str());


	FORAB(j,0,last){
		system(("rm -r "+ folder_path+NumberToString(j)).c_str());
	}
	return 0;
}
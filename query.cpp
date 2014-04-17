#include <bits/stdc++.h>
#include "porter.h"

using namespace std;
#define CLR(a) memset(a, 0, sizeof(a))
#define ABS(X) ( (X) > 0 ? (X) : ( -(X) ) )
#define SZ(V) (long long )V.size()
#define ALL(V) V.begin(), V.end()
#define RALL(V) V.rbegin(), V.rend()
#define FORN(i, n) for(LL i = 0; i < n; i++)
#define FORAB(i, a, b) for(LL i = a; i <= b; i++)
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

using namespace std;

char A[5000005];
vector < vector < pair<string,LL> > > index_words;
map < string,long long int > stopword;
map < pair<string,int>,map<LL,LL> > query_words;
map < LL,double > strength_word;
map < LL,LL > field_weight;
vector < pair<LL,LL> > sec_list_index;
string folder_path;
map < LL, pair<string,LL> > doc_list;
LL seperate_words=20,total_docs=0;

LL stringtonumber(string &S){
	LL num=0;
	FORN(i,SZ(S)){
		num=num*10+S[i]-'0';
	}
	return num;
}
string NumberToString ( long long int Number )
{
     ostringstream ss;
     ss << Number;
     return ss.str();
}
bool isalpha(char a){
	if((a>='a' && a<='z') || (a>='A' && a<='Z' )){
		return true;
	}
	return false;
}
bool isnum(char a){
	if(a>='0' && a<='9'){
		return true;
	}
	return false;
}
bool myfunction (pair<string,LL> i,string j) { 
	return (i.first<=j); 
}

bool myfunction1 (pair<LL,LL> i,LL j) { 
	return (i.first<=j); 
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void space_seperated_list(vector < string > &A,string &S){
	stringstream ss(S);
	string item;
	while(ss >> item){
		A.PB(item);
	}
	return;
}

string get_docs(string &word){
	// cout << word << endl;
	LL doc_num=0;
	if(isalpha(word[0])){
		doc_num=word[0]-'a'+10;
	}
	else{
		doc_num=word[0]-'0';	
	}
	vector < pair<string,LL> >::iterator pos;
	pos=lower_bound(index_words[doc_num].begin(),index_words[doc_num].end(),word,myfunction);
	// cout << (*pos).first << " ";
	if(pos==index_words[doc_num].end()){
		--pos;
	}
	if((*pos).first>word){
		if(pos-index_words[doc_num].begin()>0){
			--pos;
		}
	}
	// cout << pos-index_words[doc_num].begin() << " ";
	// cout << (*pos).second << endl;
	
	FILE *fp=fopen( (folder_path+"vocabulary"+NumberToString(doc_num)).c_str(),"r" );
	if(fseek(fp,(*pos).second,SEEK_SET)==-1){
		return "";
	}
	LL found=0;
	while(fscanf(fp,"%s",A)){
		LL upper_limit=min((LL)strlen(A),(LL)SZ(word));
		LL cmp=0,i;
		// cout << A << " " << word << endl;
		for(i=0;i<upper_limit;i++){
			if(A[i]>word[i]){
				cmp=1;
				break;
			}
			else if(A[i]<word[i]){
				cmp=-1;
				break;
			}
		}
		if(cmp>0){
			break;
		}
		if(cmp==0){
			found=1;
			break;
		}
	}
	fclose(fp);
	if(found==0){
		return "";
	}
	// printf("%s\n", A);
	LL i,flag=0,num=0;
	for(i=0;A[i]!=0;i++){
		if(A[i]==':'){
			flag=1;
		}
		else if(flag==1){
			num=10*num+A[i]-'0';
		}
	}
	fp=fopen( (folder_path+"prim_index"+NumberToString(doc_num)).c_str(),"r" );
	if(fseek(fp,num,SEEK_SET)==-1){
		return "";
	}
	fscanf(fp,"%s",A);
	fclose(fp);
	return A;
	// FORN(j=0;)
}

void add_stopwords(){
	ifstream stop_w;
	string W;
	stop_w.open("stopword.txt");
	while ( stop_w >> W )
    {
    	// stopword.insert(word);
    	stopword[W]++;
    }
	stop_w.close();

}

void load_index(){
	index_words.resize(36);
	ifstream sec_index;
	string S,A;
	LL num;
	FORN(i,36){
		sec_index.open((folder_path+"sec_index"+NumberToString(i)).c_str());
		while(sec_index >> S){
			A="";num=0;
			for(j=0;j<SZ(S);j++){
				if(S[j]==':'){
					j++;
					break;
				}
				A+=S[j];
			}
			for(;j<SZ(S);j++){
				num=num*10+S[j]-'0';
			}
			index_words[i].PB(MP(A,num));
			// cout << S << " " << A << " " << num << endl;
		}
		sec_index.close();
	}
}

LL need_space(char a,char b){
	/*if((isalpha(a) || isnum(a)) && (isalpha(b) || isnum(b))){
		return 0;
	}*/
	if(isalpha(a) && isalpha(b)){
		return 0;
	}
	if(isnum(a) && isnum(b)){
		return 0;
	}
	return 1;
}

void put_in_vector(vector < string > &A,string &given){
	string temp_str="";
	FORN(i,SZ(given)-1){
		temp_str+=tolower(given[i]);
		if(need_space(given[i],given[i+1])){
			if(SZ(temp_str)!=0) {
				if(isalpha(temp_str[0]) || isnum(temp_str[0])){
					A.PB(temp_str);
				}
				temp_str="";
			}
		}
	}
	if(isalpha(given[SZ(given)-1]) || isnum(given[SZ(given)-1])) {
		temp_str+=tolower(given[SZ(given)-1]);
	}
	if(isalpha(temp_str[0]) || isnum(temp_str[0])){
		A.PB(temp_str);
	}
	return;
}

vector < vector < string > > parse_query(string &query){
	vector < vector < string > > li;
	li.resize(6);
	vector < string > Q;
	space_seperated_list(Q,query);
	FORN(i,SZ(Q)){
		string temp=Q[i].substr(0,2);
		if(temp=="t:"){
			string X=Q[i].substr(2,SZ(Q[i]));
			put_in_vector(li[0],X);
		}
		else if(temp=="b:"){
			string X=Q[i].substr(2,SZ(Q[i]));
			put_in_vector(li[1],X);
		}
		else if(temp=="i:"){
			string X=Q[i].substr(2,SZ(Q[i]));
			put_in_vector(li[2],X);
		}
		else if(temp=="l:"){
			string X=Q[i].substr(2,SZ(Q[i]));
			put_in_vector(li[3],X);
		}
		else if(temp=="c:"){
			string X=Q[i].substr(2,SZ(Q[i]));
			put_in_vector(li[4],X);
		}
		else{
			put_in_vector(li[5],Q[i]);
		}
	}
	// FORN(i,6){
	// 	FORN(j,SZ(li[i])){
	// 		cout << li[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }
	return li;
}

pair <string,LL>  get_doc_list(LL id){
	if(doc_list.count(id)!=0){
		return doc_list[id];
	}
	vector < pair<LL,LL> >::iterator pos;
	pos=lower_bound(sec_list_index.begin(),sec_list_index.end(),id,myfunction1);
	if(pos==sec_list_index.end()){
		--pos;
	}
	else if((*pos).first>id){
		--pos;
	}
	FILE *fp=fopen( (folder_path+"doc_list").c_str(),"r" );
	if(fseek(fp,(*pos).second,SEEK_SET)==-1){
		fclose(fp);
		doc_list[id]=MP("",-1);
		return MP("",-1);
	}
	LL num;
	while(fscanf(fp,"%lld",&num)!=EOF){
		// cout << num << endl;
		fscanf(fp,"%[^\n]",A);
		// cout << A << endl;
		if(num==id){
			LL i,len=0;
			string title="";
			for(i=0;A[i]!=0;i++){
				if(A[i]!=' '){
					break;
				}
			}
			for(;A[i]!=0;i++){
				if(A[i]==' '){
					i++;
					break;
				}
				len=len*10+A[i]-'0';
			}
			for(;A[i]!=0;i++){
				title+=A[i];
			}
			fclose(fp);
			doc_list[id]=MP(title,len);
			return MP(title,len);
		}
	}
	fclose(fp);
	doc_list[id]=MP("",-1);
	return MP("",-1);	
}

LL get_doc_len(LL id){
	if(doc_list.count(id)!=0){
		return doc_list[id].second;
	}
	vector < pair<LL,LL> >::iterator pos;
	pos=lower_bound(sec_list_index.begin(),sec_list_index.end(),id,myfunction1);
	if(pos==sec_list_index.end()){
		--pos;
	}
	else if((*pos).first>id){
		--pos;
	}
	FILE *fp=fopen( (folder_path+"doc_list").c_str(),"r" );
	if(fseek(fp,(*pos).second,SEEK_SET)==-1){
		fclose(fp);
		doc_list[id]=MP("",-1);
		return 1000;
	}
	LL num;
	while(fscanf(fp,"%lld",&num)!=EOF){
		// cout << num << endl;
		fscanf(fp,"%[^\n]",A);
		// cout << A << endl;
		if(num==id){
			LL i,len=0;
			string title="";
			for(i=0;A[i]!=0;i++){
				if(A[i]!=' '){
					break;
				}
			}
			for(;A[i]!=0;i++){
				if(A[i]==' '){
					i++;
					break;
				}
				len=len*10+A[i]-'0';
			}
			fclose(fp);
			return len;
		}
		else if(num>id){
			break;
		}
	}
	fclose(fp);
	doc_list[id]=MP("",-1);
	return 1000;	


	pair<string,LL> doc_pair=get_doc_list(id);
	if(doc_pair.second==-1){
		return 1000;
	}
	return doc_pair.second;
}
void load_query_words(vector < vector < string > > &A){
	map <char,LL> mymap;
	mymap['t']=0;mymap['b']=1;mymap['i']=2;mymap['l']=3;mymap['c']=4;
	FORN(i,6){
		FORN(j,SZ(A[i])){
			if(stopword.count(A[i][j])!=0){
				continue;
			}
			string S=stemword(A[i][j]);
			string docs=get_docs(S);
			// cout << SZ(docs) << endl;
			// cout << docs << endl;
			vector < string > X;
			X=split(docs,',',X);
			// cout << SZ(X) << endl;
			FORN(k,SZ(X)){
				LL id=0,l;
				for(l=0;l<SZ(X[k]);l++){
					if(X[k][l]==':'){
						l++;
						break;
					}
					id=id*10+X[k][l]-'0';
				}
					// cout << X[k] << " " << id << endl;
				// cout << id << " " << X[k] << endl;
				FORN(p,5){
					if(l<SZ(X[k])){
						char c=X[k][l];l++;
						LL num=0;
						for(;l<SZ(X[k]);l++){
							if(isnum(X[k][l])){
								num=num*10+X[k][l]-'0';
							}
							else{
								break;
							}
						}
						// cout << c << " " << num << endl;
						// query_words[MP(S,mymap[c])][id]+=num;
						if(i==5 || i==mymap[c]){
							// cout << get_doc_len(id) << endl;
							strength_word[id]+=abs(( ( (num*1.0) /(1.0*1000) ) * ( log (total_docs*1.0/SZ(X)) ) )*field_weight[mymap[c]]);
							// cout << id << " " << (num*1.0/1000.0) << " " << log(13000.0/SZ(X)) << " " << field_weight[mymap[c]] << endl;
						}
						// cout << S << " " << mymap[c] << " " << id << " " << num << endl;
					}
				}
			}
		}
	}
}



void load_sec_list_index(){
	ifstream in;
	string line="";
	LL i,id,pos;
	in.open((folder_path+"sec_doc_list").c_str());
	// cout << folder_path << "sec_doc_list" << endl;
	while(getline(in,line)){
		id=0;pos=0;
		for(i=0;i<SZ(line);i++){
			if(line[i]==' '){
				i++;
				break;
			}
			id=id*10+line[i]-'0';
		}
		for(;i<SZ(line);i++){
			pos=pos*10+line[i]-'0';
		}
		// cout << id << " " << pos << endl;
		sec_list_index.PB(MP(id,pos));
	}
	in.close();
	return;
}

void get_total_docs(){
	ifstream x((folder_path+"docs").c_str());
	x >> total_docs;
	x.close();
}
// t b i l c
int main(int argc,char* argv[]){
	field_weight[0]=4.0;field_weight[1]=1.0;field_weight[2]=1.5;field_weight[3]=2.0;field_weight[0]=2.5;
	LL test,i;
	string word,stemmed_word,query;
	vector < vector < string > > word_list;
	if(argc > 1 ){
		folder_path = argv[1];
		if(folder_path[folder_path.size()-1]!='/'){
			folder_path+="/";
		}
	}
	else{
		folder_path="./";
	}
	load_sec_list_index();
	add_stopwords();
	load_index();
	get_total_docs();
	// return 0;
	getline(cin,query);
	test=stringtonumber(query);
	while(test--){
		getline(cin,query);
		//cout << query << endl;
		query_words.clear();
		strength_word.clear();
		word_list=parse_query(query);
		load_query_words(word_list);
		vector < pair<double,LL> > ans;
		for(map<LL,double>::iterator it=strength_word.begin();it!=strength_word.end();++it){
			ans.PB(MP(it->second,it->first));
		}
		sort(RALL(ans));
		FORAB(j,0,min(SZ(ans)-1,9LL) ) { 
			pair <string,LL> PQ=get_doc_list(ans[j].second);
			cout << ans[j].second << " " << PQ.F /*<< " " << ans[j].first*/ << endl;
		}
		// cout << get_docs(word) << endl;
		// for(i=0;i<(int)word.size();i++){
		// 	if(word[i]>='A' && word[i]<='Z'){
		// 		word[i]+='a'-'A';
		// 	}
		// }
		// if(stopword.count(word)!=0){
		// 	cout << endl;
		// 	continue;
		// }
		// stemmed_word=stemword(word);
	}
	return 0;
}
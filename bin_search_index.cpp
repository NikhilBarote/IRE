#include <bits/stdc++.h>
#include "porter.h"

using namespace std;
#define CLR(a) memset(a, 0, sizeof(a))
#define ABS(X) ( (X) > 0 ? (X) : ( -(X) ) )
#define SZ(V) (LL )V.size()
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
#define MOD 1000000007LL

typedef pair<int,int> PII;
typedef pair<double, double> PDD;
typedef long long LL;

string SORTED_FILE="/home/kunj/Downloads/ire/final_names.txt",sec_index="/media/shared/Downloads/index/sec_index",index_file="/media/shared/Downloads/index/pri_index";
map < string,long long int > stopword;
map < string,char > category;
LL fseek_temp_bytes;
char fseek_temp[50000005];
void add_stopwords(){
	ifstream stop_w;
	string W;
	stop_w.open("/home/kunj/Downloads/ire/stopword.txt");
	while ( stop_w >> W )
    {
    	// stopword.insert(word);
    	stopword[W]++;
    }
	stop_w.close();

}

void load_cat(){
	ifstream stop_w;
	string W;
	stop_w.open("/media/shared/Downloads/fnalCategory.txt");
	while ( getline(stop_w,W) )
    {
    	string tit="",temp;
    	FORN(i,SZ(W)){
    		if(W[i+1]==':'){
		    	// cout << tit << " " << W[i+3] << endl;
		    	category[tit]=W[i+3];
    			break;
    		}
    		tit+=W[i];
    	}
    }
	stop_w.close();	
}

int title_comp(string l,string r){
	stringstream ss(l);
	vector < string > A;
	string word;
	while(ss >> word){
		A.PB(word);
	}
	l="";
	FORN(i,SZ(A)-1){
		l+=A[i]+" ";
	}
	l+=A[SZ(A)-1];

	LL a=0,b=0;
	while(a<SZ(l) && b < SZ(r)){
		if(!((l[a]>='A' && l[a]<='Z') || (l[a]>='a' && l[a]<='z') || (l[a]>='0' && l[a]<='9'))){
			a++;
			continue;
		}
		if(!((r[b]>='A' && r[b]<='Z') || (r[b]>='a' && r[b]<='z') || (r[b]>='0' && r[b]<='9'))){
			b++;
			continue;
		}
		if(l[a]<='Z'){
			l[a]+='a'-'A';
		}
		if(r[b]<='Z'){
			r[b]+='a'-'A';
		}
		if(l[a]<r[b]){
			return -1;
		}
		else if(l[a]>r[b]){
			return 1;
		}
		a++;b++;
	}
	return 0;

	// stringstream ss(l),ss1(r);
	// vector < string > A,B;
	// string word,new_l="";
	// while(ss >> word){
	// 	A.PB(word);
	// }
	// while(ss1 >> word){
	// 	B.PB(word);
	// }
	// int a=0,b=0;
	// while(true){
	// 	cout << A[a] << " " << B[b] << (A[a]>B[b]) << endl;
	// 	if(a==SZ(A) && b==SZ(B)){
	// 		return 0;
	// 	}
	// 	if(A[a]>B[b]){
	// 		return 1;
	// 	}
	// 	else if(A[a]<B[b]){
	// 		return -1;
	// 	}
	// 	a++;b++;
	// }
	// return 0;
}

string get_id(string title){
	string filename="/home/kunj/Downloads/ire/title_to_id.txt";
	ifstream is(filename.c_str());
	string line;
	LL beg=0,end,mid;
	is.seekg (0, is.end);
	end = is.tellg();
	is.seekg (0, is.beg);
	// cout << beg << " " << end << endl;
	while(beg<=end){
		mid=beg+(end-beg)/2;
		is.seekg(mid,ios::beg);
		getline (is,line);
		getline (is,line);
		// cout << beg << " " << end << endl;
		LL val=title_comp(line,title);
		// cout << line << endl << val << endl << endl;
		if(line=="" || val>=0){
			end=mid-1;
		}
		else{
			beg=mid+1;
		}
		is.seekg (0, is.beg);
	}
	is.seekg (beg, is.beg);
	while(getline(is,line)){
		// cout << line << "---" << title << "--->" << title_comp(line,title) << endl;
		LL val=title_comp(line,title);
		if(val>0){
			break;
		}
		if(val==0){
			stringstream ss(line);
			vector < string > A;
			string word;
			while(ss >> word){
				A.PB(word);
			}
			// cout << SZ(A) << endl;
			is.close();
			return A[SZ(A)-2] + " " + A[SZ(A)-1];
		}
		// cout << line << endl;
	}
	is.close();
	return "-1";
}

int comp(string &l,string &r){
	LL a=0,b=0;
	while(a<SZ(l) && b < SZ(r)){
		if(l[a]=='#' || r[b]=='#'){
			return 0;
		}
		if(!((l[a]>='A' && l[a]<='Z') || (l[a]>='a' && l[a]<='z') || (l[a]>='0' && l[a]<='9'))){
			a++;
			continue;
		}
		if(!((r[b]>='A' && r[b]<='Z') || (r[b]>='a' && r[b]<='z') || (r[b]>='0' && r[b]<='9'))){
			b++;
			continue;
		}
		if(l[a]<='Z'){
			l[a]+='a'-'A';
		}
		if(r[b]<='Z'){
			r[b]+='a'-'A';
		}
		if(l[a]<r[b]){
			return -1;
		}
		else if(l[a]>r[b]){
			return 1;
		}
		a++;b++;
	}
	return 0;
}

int comp_exact(string &l,string &r){
	LL a=0,b=0;
	while(a<SZ(l) && b < SZ(r)){
		if(l[a]=='#'){
			break;
		}
		if(!((l[a]>='A' && l[a]<='Z') || (l[a]>='a' && l[a]<='z') || (l[a]>='0' && l[a]<='9'))){
			a++;
			continue;
		}
		if(!((r[b]>='A' && r[b]<='Z') || (r[b]>='a' && r[b]<='z') || (r[b]>='0' && r[b]<='9'))){
			b++;
			continue;
		}
		if(l[a]<='Z'){
			l[a]+='a'-'A';
		}
		if(r[b]<='Z'){
			r[b]+='a'-'A';
		}
		if(l[a]<r[b]){
			return -1;
		}
		else if(l[a]>r[b]){
			return 1;
		}
		a++;b++;
	}
	while(b<SZ(r)){
		if((r[b]>='A' && r[b]<='Z') || (r[b]>='a' && r[b]<='z') || (r[b]>='0' && r[b]<='9')){
			break;
		}
		b++;
	}
	while(a<SZ(l)){
		if((l[a]>='A' && l[a]<='Z') || (l[a]>='a' && l[a]<='z') || (l[a]>='0' && l[a]<='9') || l[a]=='#'){
			break;
		}
		a++;
	}
	if(a<SZ(l) ){
		if(l[a]=='#' && b>=SZ(r)){
			return 0;
		}
		return -1;
	}
	if(b<SZ(r)){
		return -1;
	}
	return 0;
}
vector < string > bin_search(string target){
	ifstream is(SORTED_FILE.c_str());
	vector < string > ret;
	string line;
	LL beg=0,end,mid;
	is.seekg (0, is.end);
	end = is.tellg();
	is.seekg (0, is.beg);
	// cout << beg << " " << end << endl;
	while(beg<=end){
		mid=beg+(end-beg)/2;
		is.seekg(mid,ios::beg);
		getline (is,line);
		getline (is,line);
		// cout << beg << " " << end << endl;
		// cout << line << endl << endl;
		if(line=="" || comp(line,target)>=0){
			end=mid-1;
		}
		else{
			beg=mid+1;
		}
		is.seekg (0, is.beg);
	}
	is.seekg (beg, is.beg);
	while(getline(is,line)){
		// cout << line << "---" << target << "--->" << comp_exact(line,target) << endl;
		LL val=comp_exact(line,target);
		if(val>0){
			break;
		}
		if(val==0){
			ret.PB(line);
		}
		// cout << line << endl;
	}
	is.close();
	return ret;
}

int offset_comp(string &l,string &r){
	string temp="";
	FORN(i,SZ(l)){
		if(l[i]==':'){
			break;
		}
		temp+=l[i];
	}
	if(temp>r){
		return 1;
	}
	if(temp==r){
		return 0;
	}
	return -1;
}

vector < LL > get_offset(string word){
	vector < LL > ret(5,-1);
	ifstream is(sec_index.c_str());
	string line;
	LL beg=0,end,mid;
	is.seekg (0, is.end);
	end = is.tellg();
	is.seekg (0, is.beg);
	// cout << beg << " " << end << endl;
	while(beg<=end){
		mid=beg+(end-beg)/2;
		is.seekg(mid,ios::beg);
		getline (is,line);
		getline (is,line);
		// cout << beg << " " << end << endl;
		// cout << line << endl << endl;
		if(line=="" || offset_comp(line,word)>=0){
			end=mid-1;
		}
		else{
			beg=mid+1;
		}
		is.seekg (0, is.beg);
	}
	is.seekg (beg, is.beg);
	while(getline(is,line)){
		// cout << line << "---" << word << "--->" << offset_comp(line,word) << endl;
		LL val=offset_comp(line,word);
		if(val>0){
			break;
		}
		if(val==0){
			LL i;
			for(i=0;i<SZ(line);i++){
				if(line[i]==':'){
					break;
				}
			}
			if(i+1>=SZ(line)){
				continue;
			}
			LL j=i;
			LL val=0;
			for(i=i+3;i<SZ(line);i++){
				val=val*10+line[i]-'0';
			}
			if(line[j+1]=='b'){
				ret[0]=val;
			}
			else if(line[j+1]=='c'){
				ret[1]=val;
			}
			else if(line[j+1]=='l'){
				ret[2]=val;
			}
			else if(line[j+1]=='i'){
				ret[3]=val;
			}
			else if(line[j+1]=='t'){
				ret[4]=val;
			}
		}
		// cout << line << endl;
	}
	is.close();
	return ret;	
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

string to_string_my(LL a){
	ostringstream ss;
	ss << a;
	return ss.str();
}

char get_category(string title){
	FORN(i,SZ(title)){
		if(title[i]>='A' && title[i]<='Z'){
			title[i]+='a'-'A';
		}
	}
	if(category.count(title)==0){
		return 'E';
		int a=rand()%3;
		if(a==0){
			return 'O';
		}
		else if(a==1){
			return 'L';
		}
		else{
			return 'P';
		}
	}
	return category[title];
}

void get_options(string query_file){
	LL window=5;
	ifstream infile(query_file.c_str());
	stringstream buffer;
	vector < string > elements;
	vector < vector < LL > > offset_elements;
	string word;
	buffer << infile.rdbuf();
	while(buffer >> word){
		elements.PB(word);
		// cout << word << "---";
	}
	FORN(i,SZ(elements)){
		word="";
		if(stopword.count(elements[i])!=0){
			vector < LL > tt(5,-1);
			offset_elements.PB(tt);
			continue;
		}
		FORN(j,SZ(elements[i])){
			if(elements[i][j]>='A' && elements[i][j]<='Z'){
				word+=(elements[i][j]+'a'-'A');
			}
			else{
				word+=elements[i][j];
			}
		}
		if(stopword.count(word)!=0){
			vector < LL > tt(5,-1);
			offset_elements.PB(tt);
			continue;
		}
		string S=stemword(word);
		offset_elements.PB(get_offset(S));
		// cout << S << " " << get_offset(S) << endl;
	}
	// FORN(i,SZ(offset_elements)){
	// 	cout << elements[i] << " --- ";
	// 	FORN(j,SZ(offset_elements[i])){
	// 		cout << offset_elements[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }
	FILE *index_fp=fopen(index_file.c_str(),"r");
	if(index_fp==NULL){
		// cout << "qwe" << endl;
	}
	vector < pair < double,pair < LL,pair <string,LL> > > > result;
	double over_mx=0;
	FORN(i,SZ(elements)){
		if(stopword.count(elements[i])!=0){
			continue;
		}
		string tar="";
		map < LL,string > id_title;
		map < LL,LL > id_len;
		map < LL,double > id_score;
		id_title.clear();
		id_len.clear();
		id_score.clear();
		// cout << elements[i] << endl;
		FORN(ngram,3){
			if(i+ngram>=SZ(elements)){
				break;
			}
			tar+=elements[i+ngram];
			vector < string > poss=bin_search(tar);

			// cout << tar << "--->" << SZ(poss) << " "<< endl;
			FORN(j,SZ(poss)){
				string temp="";
				int l;
				for(l=0;l<SZ(poss[j]);l++){
					if(poss[j][l]=='#'){
						l+=2;
						break;
					}
				}
				temp=poss[j].substr(l,SZ(poss[j]));
				if(temp==""){
					continue;
				}
				// cout << temp << " " << get_id(temp) << endl;
				string temp_str=get_id(temp);
				LL temp_len,temp_id;
				if(temp_str=="-1"){
					continue;
				}
				stringstream ss(temp_str);
				ss >> temp_id;
				ss >> temp_len;
				id_title[temp_id]=temp;
				id_len[temp_id]=max(temp_len,100LL);
				// cout << temp <<  " " << temp_id << " ->" << temp_len << endl;
			}
			tar+=" ";
			// cout << endl;
		}
		// return ;
		if(SZ(id_len)==0){
			continue;
		}
		LL j;
		for(j=max(0LL,i-window);j<=min(SZ(elements)-1,i+window);j++){
			// cout << elements[j] << " ---- " << endl;
			FORN(k,SZ(offset_elements[j])){
				// cout  << j << " " << k << endl;
				if(offset_elements[j][k]==-1){
					continue;
				}

				if(fseek(index_fp,offset_elements[j][k],SEEK_SET)==-1){
					continue;
				}
				// cout << "beg" << endl;
				fscanf(index_fp,"%s",fseek_temp);
				fscanf(index_fp,"%s",fseek_temp);
				// cout << "end" << endl;
				// printf("%s\n",fseek_temp );
				LL l,flag=0,temp_cnt=0,temp_id=0;
				// cout << elements[j] << "---" << endl;
				for(l=0;fseek_temp[l]!=0;l++){
					if(fseek_temp[l]==':'){
						flag=1;
					}
					else if(fseek_temp[l]==','){
						if(id_len.count(temp_id)!=0){
						// cout << temp_id << " " << temp_cnt << "\n";
							if(j-i>=0 && j-i<=2){
								id_score[temp_id]+=temp_cnt*1.0/(10.0);
							}
							else{
								id_score[temp_id]+=temp_cnt;//*1.0/(50.0);
							}
						}
						temp_id=0;
						temp_cnt=0;
						flag=0;
					}
					else{
						if(flag==0){
							temp_id=temp_id*10+fseek_temp[l]-'0';
						}
						else{
							temp_cnt=temp_cnt*10+fseek_temp[l]-'0';
						}
					}
				}
				// return ;
			}
			// cout << elements[j] << " ";
		}
		// vector < pair<double,LL> > ranking;
		double val=0;
		LL mx=-1;
		
		for(map<LL,double>::iterator it=id_score.begin();it!=id_score.end();++it){
			double temp=(((it->second)*1.0)/id_len[it->first]);
			if((((it->second)*1.0)/(id_len[it->first]*id_len[it->first]))>(val)){
				mx=(it->first);
				val=temp;
			}
			// ranking.PB(MP(it->second,it->first));
			// cout << it->first << " " << it->second << " " << id_title[it->first] << endl;
		}
		// sort(RALL(ranking));
		// FORN(j,SZ(ranking)){
		// 	cout << ranking[j].second << " " << ranking[j].first << " " << id_title[ranking[j].second] << "\n";
		// }
		if(mx==-1){
			continue;
		}
		// cout << mx <<  " " << id_title[mx] << " --- " << val << endl;
		over_mx=max(val,over_mx);
		result.PB(MP(val,MP(mx,MP(id_title[mx],i))));
	}
	fclose(index_fp);
	j=0;
	ofstream out("output.txt");
	string output1="";
	if(!out.is_open()){
		cout << "qwe" << endl;
	}
	if(over_mx<=1.0){
		over_mx=(over_mx)/2.0;
	}
	else{
		over_mx=sqrt(over_mx);
	}
	FORN(i,SZ(result)){
		while(j<SZ(elements) && ((((result[i].second).second).second)>j || stopword.count(elements[j])!=0) ) {
			out << elements[j] << " ";
			output1+=elements[j] + " ";
			j++;
		}
		if(result[i].first>min(2.0,over_mx*over_mx)){
			out << elements[j] << " <a href=\"http://en.wikipedia.org/wiki?curid=" + to_string_my((result[i].second).first) + "\">" + get_category(((result[i].second).second).first) +"</a> ";
			output1+=elements[j] + " <a href=\"http://en.wikipedia.org/wiki?curid=" + to_string_my((result[i].second).first) + "\">" + get_category(((result[i].second).second).first) +"</a> ";
			j++;
		}
		else if(result[i].first>over_mx && isupper( elements[j][0]) ){
			out << elements[j] << " <a href=\"http://en.wikipedia.org/wiki?curid=" + to_string_my((result[i].second).first) + "\">" + get_category(((result[i].second).second).first) +"</a> ";
			output1+=elements[j] + " <a href=\"http://en.wikipedia.org/wiki?curid=" + to_string_my((result[i].second).first) + "\">" + get_category(((result[i].second).second).first) +"</a> ";
			j++;
		}
		else if(i+1 <SZ(result) && ((result[i].second).first)==((result[i+1].second).first)){
			out << elements[j] << " " << elements[j+1] << " <a href=\"http://en.wikipedia.org/wiki?curid=" + to_string_my((result[i].second).first) + "\">" + get_category(((result[i].second).second).first) +"</a> ";
			output1+=elements[j] + " " + elements[j+1] + " <a href=\"http://en.wikipedia.org/wiki?curid=" + to_string_my((result[i].second).first) + "\">" + get_category(((result[i].second).second).first) +"</a> ";
			j+=2;
			i++;
		}
		// else if(j+1<SZ(elements) && result[i].first>10 && isupper( elements[j][0]) && isupper(elements[j+1][0]) ){
		// 	out << elements[j] << " " << elements[j+1] << " <a href=\"http://en.wikipedia.org/wiki?curid=" + to_string_my((result[i].second).first) + "\">" + get_category(((result[i].second).second).first) +"</a> ";
		// 	j+=2;
		// }
		
	}
	while(j<SZ(elements)){
		out << elements[j] << " ";
		output1+=elements[j] + " ";
		j++;
	}
	cout << output1 << endl;
	// out << endl;
	out.close();
	infile.close();
}

int main(){
	add_stopwords();
	load_cat();
	// string line;
	// getline (cin,line);
	// vector < string > opt=bin_search(line);
	// FORN(i,SZ(opt)){
	// 	cout << opt[i] << endl;
	// }

	get_options("mydata.txt");
	// cout << get_category("lake toba");
	// cout << get_id("Narendra Modi");

	// FILE* fp=fopen("tempp","r");
	// fscanf(fp,"%s",fseek_temp);
	// fscanf(fp,"%s%n",fseek_temp,&fseek_temp_bytes);
	// cout << search_post_list(12) << endl;



	// cout << title_comp("a b c 12 1","a bc");
	// cout << get_id("Zulu War") << endl;
	return 0;
}

#include <bits/stdc++.h>

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
#define MOD 1000000007LL

typedef pair<int,int> PII;
typedef pair<double, double> PDD;
typedef long long LL;

string final_folder;
LL sec_seperate_words=20;
string NumberToString ( long long int Number )
{
     ostringstream ss;
     ss << Number;
     return ss.str();
}

LL numbersize(long long int Number){
	if(Number==0){
		return 1;
	}
	LL cnt=0;
	while(Number!=0){
		cnt++;
		Number/=10;
	}
	return cnt;
}

void write_back_index(){
	FILE *prim_index[36],*vocabulary[36],*sec_index[36];
	ifstream input;
	string S,R;
	long long int len[36],curr_index_file=0,word_count[36],len_sec[36];
	memset(len,0,sizeof(len));
	memset(word_count,0,sizeof(word_count));
	memset(len_sec,0,sizeof(len_sec));
	input.open((final_folder+ "prim_index").c_str());
	FORN(i,36){
		prim_index[i]=fopen((final_folder+ "prim_index"+NumberToString(i)).c_str(),"w");
		vocabulary[i]=fopen((final_folder+"vocabulary"+NumberToString(i)).c_str(),"w");
		sec_index[i]=fopen((final_folder+"sec_index"+NumberToString(i)).c_str(),"w");
	}
	while(input >> S){
		input >> R;
		// cout << S << " " << R << endl;
		if(isalpha(S[0])){
			curr_index_file=10+S[0]-'a';
		}
		else{
			curr_index_file=S[0]-'0';	
		}
		if(word_count[curr_index_file]==0){
			fprintf(sec_index[curr_index_file], "%s:%lld\n",S.c_str(),len_sec[curr_index_file] );
		}
		fprintf(vocabulary[curr_index_file], "%s:%lld\n",S.c_str(),len[curr_index_file] );
		len_sec[curr_index_file]+=SZ(S)+2+numbersize(len[curr_index_file]);
		fprintf(prim_index[curr_index_file], "%s\n",R.c_str());
		len[curr_index_file]+=1+SZ(R);
		word_count[curr_index_file]=(word_count[curr_index_file]+1)%sec_seperate_words;
	}
	input.close();
	FORN(i,36){
		fclose(prim_index[i]);fclose(vocabulary[i]);fclose(sec_index[i]);
	}
}

void write_doc_list(){
	ifstream in;
	string line="";
	FILE* out=fopen(((final_folder)+"sec_doc_list").c_str(),"w");
	LL cnt=0,len=0,total=0;
	in.open((final_folder+"doc_list").c_str());
	while(getline(in,line)){
		if(cnt==0){
			LL id=0,i;
			for(i=0;i<SZ(line);i++){
				if(line[i]==' '){
					break;
				}
				id=id*10+line[i]-'0';
			}
			fprintf(out, "%lld %lld\n",id,len );
		}
		cnt=(cnt+1)%sec_seperate_words;
		total++;
		len+=SZ(line);
	}
	fclose(out);
	in.close();
	ofstream x((final_folder+"docs").c_str());
	x << total << endl;
	x.close();
	return ;
}

int main(int argc,char* argv[]){
	if(argc>1){
		final_folder=argv[1];
		if(final_folder[final_folder.size()-1]!='/'){
			final_folder+="/";
		}
	}
	else{
		final_folder="./";
	}
	write_back_index();
	system(("rm "+ final_folder+ "prim_index").c_str());
	write_doc_list();
	return 0;
}
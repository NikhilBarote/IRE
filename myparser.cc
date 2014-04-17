// block=0 >> body
// block=1 >> infobox
// block=2 >> external
// block=3 >> category
// block=4 >> title

#include "myparser.h"
#include "porter.h"
#include <glibmm/convert.h> //For Glib::ConvertError

#include <bits/stdc++.h>

#define SZ(V) (long long )V.size()
#define FORN(i, n) for(LL i = 0; i < n; i++)
#define FORAB(i, a, b) for(LL i = a; i <= b; i++)
#define PB push_back  
#define MP make_pair

typedef long long LL;

using namespace std;

string current_str="";
long long int doc_id=-1,sec_seperate_words=200,curr_dir=0,num_docs=0,index_size=10000,total_docs=0,doc_size=1000;
string title;
map < string, map < long long int, map < long long int,long long int > > > index_words;
map < LL ,pair< string,LL > > list_of_docs;
map < string,int > stopword;

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
bool isbracket(char a){
	if(a=='{' || a=='}' || a=='[' || a==']') {
		return true;
	}
	return false;
}

LL need_space(char a,char b){
	if(a==b){
		return 0;
	}
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
				if(isalpha(temp_str[0]) || isnum(temp_str[0]) || isbracket(temp_str[0]) || temp_str[0]=='='){
					A.PB(temp_str);
				}
				temp_str="";
			}
		}
	}
	if(isalpha(given[SZ(given)-1]) || isnum(given[SZ(given)-1]) || isbracket(given[SZ(given)-1]) || given[SZ(given)-1]=='='){
		temp_str+=tolower(given[SZ(given)-1]);
	}
	if(SZ(temp_str)!=0 && (isalpha(temp_str[0]) || isnum(temp_str[0]) || isbracket(temp_str[0]) || temp_str[0]=='=')){
		A.PB(temp_str);
	}
	return;
}

void update_index(vector < string > &A){
	LL block=0,infobox_counter=-1;
	FORN(i,SZ(A)-1){
		if(A[i]=="{{"){
			if(A[i+1]=="infobox"){
				block=1;
				infobox_counter=0;
				i++;
			}
			if(block==1){
				infobox_counter++;
			}
		}
		else if(A[i]=="}}" ){
			if(block==1){
				infobox_counter--;
			}
			if(infobox_counter==0){
				infobox_counter=-1;
				block=0;
			}
		}
		else if(A[i]=="==" && A[i+1]=="external" && (i+2<SZ(A) && A[i+2]=="links")){
			block=2;
			i+=2;
		}
		else if(A[i]=="[[" && A[i+1]=="category"){
			block=3;
			i++;
		}
		else if(A[i]=="]]" && block!=1){
			block=0;
		}
		else{
			if(isalpha(A[i][0]) || isnum(A[i][0])){
				if(stopword.count(A[i])==0){
					// cout << stemword(A[i]) << " " << block << endl;
					index_words[stemword( A[i])][doc_id][block]++;
				}
			}
		}
	}
	if(isalpha(A[SZ(A)-1][0]) || isnum(A[SZ(A)-1][0])){
		if(stopword.count(A[SZ(A)-1])==0){
			// cout << stemword(A[SZ(A)-1]) << " " << block << endl;
			index_words[stemword(A[SZ(A)-1])][doc_id][block]++;
		}
	}
}

void write_doc_list(){
	FILE* out;
	out=fopen((final_folder+NumberToString(curr_dir)+ "/doc_list").c_str(),"w");
	for(map < LL,pair<string,LL> >::iterator it=list_of_docs.begin();it!=list_of_docs.end();++it){
		fprintf(out, "%lld %lld %s\n",(it->first),(it->second).second,((it->second).first).c_str() );
	}
	total_docs+=SZ(list_of_docs);
	list_of_docs.clear();
	fclose(out);
	return;
}

void write_back_index1(){
	cout << "start " << curr_dir << endl;
	FILE *prim_index;//,*sec_index;
	system(("mkdir -p "+final_folder+NumberToString(curr_dir)).c_str());
	prim_index=fopen((final_folder+NumberToString(curr_dir)+ "/prim_index").c_str(),"w");
	// sec_index=fopen((final_folder+NumberToString(curr_dir)+ "/sec_index").c_str(),"w");
	string S;
	for(map<string ,map<LL ,map<LL,LL> > >::iterator it=index_words.begin();it!=index_words.end();++it){
		// fprintf(sec_index, "%s %lld\n",(it->first).c_str(),len );
		fprintf(prim_index,"%s ",(it->first).c_str());
		for(map<LL, map<LL,LL> >::iterator sec=(it->second).begin();sec!=(it->second).end();++sec){
			fprintf(prim_index,"%lld:",sec->first);
			LL X;
			if((sec->second).count(0)){
				X=max(((sec->second)[0]*1000)/doc_size,1LL);
				fprintf(prim_index,"b%lld",X);
			}
			if((sec->second).count(1)){
				// X=(sec->second)[1];
				X=max(((sec->second)[1]*1000)/doc_size,1LL);
				fprintf(prim_index,"i%lld",X);
			}
			if((sec->second).count(2)){
				// X=(sec->second)[2];
				X=max(((sec->second)[2]*1000)/doc_size,1LL);
				fprintf(prim_index,"l%lld",X);
			}
			if((sec->second).count(3)){
				// X=(sec->second)[3];
				X=max(((sec->second)[3]*1000)/doc_size,1LL);
				fprintf(prim_index,"c%lld",X);
			}
			if((sec->second).count(4)){
				// X=(sec->second)[4];
				X=max(((sec->second)[4]*1000)/doc_size,1LL);
				fprintf(prim_index,"t%lld",X);
			}
			fprintf(prim_index,",");
		}
		fprintf(prim_index,"\n");
	}
	fclose(prim_index);//fclose(sec_index);
	index_words.clear();
	write_doc_list();
	cout << "end " << curr_dir << endl;
	curr_dir++;
}


MySaxParser::MySaxParser()
: xmlpp::SaxParser()
{
}

MySaxParser::~MySaxParser()
{
}

void MySaxParser::on_start_document()
{
	ifstream stop_w;
	string word;
	stop_w.open("stopword.txt");
	while ( stop_w >> word )
    {
    	stopword[word]++;
    }
	stop_w.close();
	current_str="";
	num_docs=0;
	curr_dir=0;
	total_docs=0;
	title="";
}

void MySaxParser::on_end_document()
{
	write_back_index1();
}

void MySaxParser::on_start_element(const Glib::ustring& name,const AttributeList& attributes)
{
	string S=name.lowercase();
	if(S=="page"){
		if(num_docs%index_size==0 && num_docs!=0){
			write_back_index1();
		}
		num_docs++;
		doc_id=-1;
		title="";
	}
	current_str="";
}

void MySaxParser::on_end_element(const Glib::ustring& name)
{
	string S=name.lowercase();
	// cout << S << " " << endl;
	if(doc_id==-1 &&  S=="id"){
		if(current_str.size()!=0){
			doc_id=0;
			for(LL i=0;i<(LL)SZ(current_str);i++){
				if(isnum(current_str[i])){
					doc_id=doc_id*10+current_str[i]-'0';
				}
			}
			// cout << doc_id << endl;
			vector < string > seperated_list;
			put_in_vector(seperated_list,current_str);
			FORN(i,SZ(seperated_list)){			
				if(isalpha(seperated_list[i][0]) || isnum(seperated_list[i][0])) {
					index_words[stemword(seperated_list[i])][doc_id][4]++;
				}
			}
			seperated_list.clear();
		}		
	
	}
	else if(S=="title"){
		title=current_str;
	} 
	else if(S=="text"){
		vector < string > seperated_list;
		put_in_vector(seperated_list,current_str);
		list_of_docs[doc_id]=MP(title,SZ(seperated_list));
		doc_size=SZ(seperated_list);
		update_index(seperated_list);
		seperated_list.clear();
	}
}

void MySaxParser::on_characters(const Glib::ustring& text)
{
	try
	{
		string S=(string)text;
		current_str+=S;
	}
	catch(const Glib::ConvertError& ex)
	{
	//	cerr << "MySaxParser::on_characters(): Exception caught while converting text for cout: " << ex.what() << endl;
	}
}

void MySaxParser::on_comment(const Glib::ustring& text)
{
// 	try
// 	{
// 		cout << "on_comment(): " << text << endl;
// 	}
// 	catch(const Glib::ConvertError& ex)
// 	{
// //		cerr << "MySaxParser::on_comment(): Exception caught while converting text for cout: " << ex.what() << endl;
// 	}
}

void MySaxParser::on_warning(const Glib::ustring& text)
{
	// try
	// {
	// 	cout << "on_warning(): " << text << endl;
	// }
	// catch(const Glib::ConvertError& ex)
	// {
	// 	cerr << "MySaxParser::on_warning(): Exception caught while converting text for cout: " << ex.what() << endl;
	// }
}

void MySaxParser::on_error(const Glib::ustring& text)
{
	// try
	// {
	// 	cout << "on_error(): " << text << endl;
	// }
	// catch(const Glib::ConvertError& ex)
	// {
	// 	cerr << "MySaxParser::on_error(): Exception caught while converting text for cout: " << ex.what() << endl;
	// }
}

void MySaxParser::on_fatal_error(const Glib::ustring& text)
{
	// try
	// {
	// 	cout << "on_fatal_error(): " << text << endl;
	// }
	// catch(const Glib::ConvertError& ex)
	// {
	// 	cerr << "MySaxParser::on_characters(): Exception caught while converting value for cout: " << ex.what() << endl;
	// }
}

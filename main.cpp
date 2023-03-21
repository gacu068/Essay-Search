#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<list>

#include<time.h>
// #include<bits/stdc++.h>

using namespace std;


class trnode{
public:
    trnode *child[26];
    trnode(){
        isend=false;
        for(int i=0;i<26;i++){
            child[i]=nullptr;
        }
    }
    bool isend;
    friend class tries;
};

class tries{
public:
    trnode *root;
    string title;
    list<trnode*> alp[26];

    tries(){
        root=new trnode;
        title={};
        alp->clear();
    }

    void insert(const string& str){
        int len=str.length();
        trnode *tmp=root;
        for(int i=0;i<len;i++){
            int index=tolower(str[i])-'a';
            if(tmp->child[index]==nullptr){
                tmp->child[index]=new trnode;
            }
            tmp=tmp->child[index];
            alp[index].push_back(tmp);
        }
        tmp->isend=true;
    }

    bool search(const string &str,const int &way){
        if(way==0){//exact
            int len=str.length();
            trnode *tmp=root;
            // cout<<str<<endl;
            for(int i=0;i<len;i++){
                int index=tolower(str[i])-'a';
                if(tmp->child[index]==nullptr){
                    return false;
                }
                else tmp=tmp->child[index];
            }
            return tmp->isend;
        }
        else if(way==1){//noprob
            int len=str.length();
            trnode *tmp=root;
            // cout<<str<<endl;
            for(int i=0;i<len;i++){
                int index=tolower(str[i])-'a';
                if(tmp->child[index]==nullptr){
                    return false;
                }
                else tmp=tmp->child[index];
            }
            return true;
        }
        else{//fuck
            int len=str.length();
            //cout<<str<<endl; 
            int index=tolower(str[0])-'a';
            for(auto it=alp[index].begin();it!=alp[index].end();it++){
                trnode *tmp=(*it);
                int i;
                for(i=1;i<len;i++){
                    int index2=tolower(str[i])-'a';
                    if(tmp->child[index2]==nullptr) break;
                    else tmp=tmp->child[index2];
                }
                if(i==len&&tmp->isend) return true;
            }
            return false;
        }
    }

    void printitle(){
        cout<<title<<endl;
    }
};



// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}


int main(int argc, char *argv[]){
    double START,END; START = clock();


    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	// Read File & Parser Example


	string file, title_name, tmp;
	fstream fi,fq;
    std::ofstream ans(output, std::ofstream::out);
	vector<string> tmp_string;


    list<tries*> treelist;

	// from data_dir get file ....
	// eg : use 0.txt in data directory
    while(true){        
        static int i=0;
        fi.open(data_dir+to_string(i)+".txt",ios::in);
        if(!fi) break;
        tries *nt=new tries;
        
         // GET TITLENAME
        getline(fi, title_name);
        nt->title=title_name;
        // GET TITLENAME WORD ARRAY
        tmp_string = split(title_name, " ");
        vector<string> title = word_parse(tmp_string);
        for(auto &word : title){
            nt->insert(word);
        }
        // GET CONTENT LINE BY LINE
        while(getline(fi, tmp)){
            // GET CONTENT WORD VECTOR
            tmp_string = split(tmp, " ");
            // PARSE CONTENT
            vector<string> content = word_parse(tmp_string);
            for(auto &word : content){
                // cout << word << endl;
                nt->insert(word);
            }
            //......
        }
        treelist.push_back(nt);
        i++;
        fi.close();
    }

    fq.open(query,ios::in);//open query
	while(getline(fq, tmp)){       
        vector<string> operate,reflect; 
        operate.reserve(205);
        reflect.reserve(205);
		tmp_string = split(tmp, " ");
        for(auto &word : tmp_string){
            if(word=="+"||word=="/"){
                operate.push_back(word);
            }
            else{
                reflect.push_back(word);
            }
	 	}
        bool allnotfound=true;
         

         
        std::list<tries*>::iterator it;
        for(it=treelist.begin();it!=treelist.end();it++){
            bool have=false;
            if(reflect[0][0]=='"'){
                string q;
                int len=reflect[0].size()-2;
                for(int i=0;i<len;i++){
                    q=q+reflect[0][i+1];
                }                
                have=(*it)->search(q,0);
            }
            else if(reflect[0][0]=='*'){
                string q;
                int len=reflect[0].size()-2;
                for(int i=0;i<len;i++){
                    q=q+reflect[0][i+1];
                }   
                have=(*it)->search(q,2);
            }else{
                have=(*it)->search(reflect[0],1);
            }

            int size=operate.size();
            for(int i=0;i<size;i++){    
                bool havenxt=false;
                if(reflect[i+1][0]=='"'){
                    string q;
                    int len=reflect[i+1].size()-2;
                    for(int j=0;j<len;j++){
                        q=q+reflect[i+1][j+1];
                    }   
                    havenxt=(*it)->search(q,0);
                }
                else if(reflect[i+1][0]=='*'){
                    string q;
                    int len=reflect[i+1].size()-2;
                    for(int j=0;j<len;j++){
                        q=q+reflect[i+1][j+1];
                    }   
                    havenxt=(*it)->search(q,2);
                }
                else{
                    havenxt=(*it)->search(reflect[i+1],1);
                }


                if(operate[i]=="+"){
                    have=have&&havenxt;
                }
                else{
                    have=have||havenxt;
                }
            }
            if(have){
                ans<<(*it)->title<<endl;
                //(*it)->printitle();
                allnotfound=false;
            }
        }
        if(allnotfound){
            // cout<<"Not Found!"<<endl;
            ans<<"Not Found!"<<endl;
        }


	}
    

    // CLOSE FILE
	fq.close();

    
    END = clock();
    cout << (END - START) / CLOCKS_PER_SEC << endl;
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + 😎 / C

//

//////////////////////////////////////////////////////////
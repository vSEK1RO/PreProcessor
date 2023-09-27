//                          _      _       __   _        _       
//  _ __ ___    ___  _ __  | |__  (_)     / /  (_)  ___ (_) ___  
// | '_ ` _ \  / _ \| '_ \ | '_ \ | |    / /   | | / __|| |/ __| 
// | | | | | ||  __/| |_) || | | || |   / /    | || (__ | |\__ \ 
// |_| |_| |_| \___|| .__/ |_| |_||_|  /_/     |_| \___||_||___/ 
//                  |_|                                          
//
//              Created by Vlad Litvinov Б23-534
//

#include <dir.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

string charToString(char *a){
    string res="";
    int i=0;
    while(a[i]!='\0'){
        res+=a[i];
        i++;
    }
    return res;
}

string preProcess(string dir, string path){
    vector <string> data;
    string includeData = "", includePath;
    string buffer = "";
    ifstream fr(dir+"\\"+path);
    while(!fr.eof()){
        getline(fr,buffer);
        data.push_back(buffer);
    }
    fr.close();
    int top=0,bottom=data.size()-1;
    for(int i=0;i<data.size();i++){
        if(data[i].substr(0,10)=="#include \""){
            includePath=data[i].substr(10,data[i].substr(10,data[i].size()-10).find("\""));
            if(i!=0){
                includeData+="\n";
            }
            includeData+="//#include \""+includePath+"\"\n";
            includeData+=preProcess(dir,includePath);
            includeData+="\n//#endclude \""+includePath+"\"";
            continue;
        }
        if(data[i].substr(0,12)=="//#include \""){
            includePath=data[i].substr(12,data[i].substr(12,data[i].size()-12).find("\""));
            while(data[top].substr(0,12)!="//#include \""){
                top++;
            }
            while(data[bottom].substr(0,13)!="//#endclude \""){
                bottom--;
            }
            data.erase(data.begin()+top,data.begin()+bottom);
            if(i!=0){
                includeData+="\n";
            }
            includeData+="//#include \""+includePath+"\"\n";
            includeData+=preProcess(dir,includePath);
            includeData+="\n//#endclude \""+includePath+"\"";
            continue;
        }
        if(i==0){
            includeData+=data[i];
        }else{
            includeData+="\n"+data[i];
        }
    }
    return includeData;
}

int main(int argc, char **argv){
    if(argc>1){
        char dir_path[256];
        getcwd(dir_path, 255);
        string data=preProcess(charToString(dir_path),charToString(argv[1]));
        ofstream fw(charToString(dir_path)+"\\"+charToString(argv[1]));
        fw<<data;
        fw.close();
        system(("g++ "+charToString(argv[1])).c_str());
    }else{
        printf("Input file didn't set");
    }
    return 0;
}
//                          _      _       __   _        _
//  _ __ ___    ___  _ __  | |__  (_)     / /  (_)  ___ (_) ___
// | '_ ` _ \  / _ \| '_ \ | '_ \ | |    / /   | | / __|| |/ __|
// | | | | | ||  __/| |_) || | | || |   / /    | || (__ | |\__ \
// |_| |_| |_| \___|| .__/ |_| |_||_|  /_/     |_| \___||_||___/
//                  |_|
//
//              Created by Vlad Litvinov Б23-534
//
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <ctime>

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
    string untilSlash;
    ifstream fr(dir+"/"+path);
    while(!fr.eof()){
        getline(fr,buffer);
        data.push_back(buffer);
    }
    fr.close();
    int top=0,bottom=0;
    for(int i=0;i<data.size();i++){
        if(data[i].substr(0,6)=="//time"){
            if(i!=0){
                includeData+="\n";
            }
            struct tm *ptr;
            time_t It;
            It = time(NULL);
            ptr = localtime(&It);
            string timeBuff = charToString(asctime(ptr));
            timeBuff.pop_back();
            includeData+="/*";
            if(timeBuff.size()>76){
                timeBuff=timeBuff.substr(0,76);
            }
            for(int j=0;j<(76-timeBuff.size())/2;j++){
                includeData+="*";
            }
            if(timeBuff.size()%2==0){
                includeData+=" "+timeBuff+" ";
            }else{
                includeData+="* "+timeBuff+" ";
            }
            for(int j=0;j<(76-timeBuff.size())/2;j++){
                includeData+="*";
            }
            includeData+="*/";
            continue;
        }
        if(data[i].substr(0,6)=="//comt"){
            data[i].erase(0,7);
            if(i!=0){
                includeData+="\n";
            }
            includeData+="/*";
            if(data[i].size()>76){
                data[i]=data[i].substr(0,76);
            }
            for(int j=0;j<(76-data[i].size())/2;j++){
                includeData+="*";
            }
            if(data[i].size()%2==0){
                includeData+=" "+data[i]+" ";
            }else{
                includeData+="* "+data[i]+" ";
            }
            for(int j=0;j<(76-data[i].size())/2;j++){
                includeData+="*";
            }
            includeData+="*/";
            continue;
        }
        if(data[i].substr(0,10)=="#include \""){
            includePath=data[i].substr(10,data[i].substr(10,data[i].size()-10).find("\""));
            if(i!=0){
                includeData+="\n";
            }
            includeData+="//#include \""+includePath+"\"\n";
            if(includePath.find("/")!=-1){
                untilSlash=includePath.substr(0,includePath.rfind("/"));
                buffer=includePath;
                buffer.erase(0,includePath.rfind("/")+1);
                includeData+=preProcess(dir+"/"+untilSlash,buffer);
            }else{
                includeData+=preProcess(dir,includePath);
            }
            printf("---------\n");
            printf("%s\n",dir.c_str());
            printf("%s\n\n",includePath.c_str());
            includeData+="\n//#endclude \""+includePath+"\"";
            continue;
        }
        if(data[i].substr(0,12)=="//#include \""){
            includePath=data[i].substr(12,data[i].substr(12,data[i].size()-12).find("\""));
            // cout<<"\n\n//#endclude \""+includePath+"\n\n";
            // for(int i=0;i<data.size();i++){
            //     cout<<data[i]+"\n";
            // }
            // cout<<"\n\n";
            top=i;
            bottom=top;
            while(data[bottom].substr(0,13+includePath.size())!=("//#endclude \""+includePath)){
                bottom++;
            }
            // printf("%d\n%d\n",top,bottom);
            data.erase(data.begin()+top,data.begin()+bottom);
            if(i!=0){
                includeData+="\n";
            }
            includeData+="//#include \""+includePath+"\"\n";
            if(includePath.find("/")!=-1){
                untilSlash=includePath.substr(0,includePath.rfind("/"));
                buffer=includePath;
                buffer.erase(0,includePath.rfind("/")+1);
                includeData+=preProcess(dir+"/"+untilSlash,buffer);
            }else{
                includeData+=preProcess(dir,includePath);
            }
            printf("---------\n");
            printf("%s\n",dir.c_str());
            printf("%s\n\n",includePath.c_str());
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
        auto dir_path = std::filesystem::current_path();
        printf("\n%s\n\n",(dir_path.u8string()+"/"+charToString(argv[1])).c_str());
        string data=preProcess(dir_path.u8string(),charToString(argv[1]));
        ofstream fw(dir_path.u8string()+"/"+charToString(argv[1]));
        fw<<data;
        fw.close();
        system(("g++ "+charToString(argv[1])).c_str());
    }else{
        printf("Input file didn't set\n");
    }
    return 0;
}
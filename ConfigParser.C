#include <string>
#include <fstream>
#include <sstream>
#include <iostream>	
#include <map>

using namespace std;

bool isWhiteSpace(string str){
	for (int i = 0; i<str.size(); i++){
		if ( (! (str[i] == ' ')) && (! (str[i] == '\t') ) ){
			return false;	
		}
	}
	return true;
}

class ConfigParser{

private:
	ifstream *config_file;
	map<string, string> options;

	void extractOptFromLine(string line){
		/*Extract a value from a line*/
		string opt_key;
		string opt_value;
		opt_key=line.substr(0, line.find('='));
		opt_value=line.substr(line.find('=')+1);
		if(opt_key != "" && opt_value != ""){
			options[opt_key] = opt_value;
		}
		else{
			cout<<"Error parsing line number: "<<config_file->tellg()<<" : "<<line<<endl;
		}
	}


public:

	ConfigParser(string filename){
		config_file = new ifstream(filename);
	}

	bool loadConfig(string config_name){
		bool found_config=false;
		options.clear();
		string line; //Holds arbitrary lines.
		string opt_line; //Holds lines like option="value".
		string opt_key; //Holds 'option' in the above example
		string opt_value; //Holds "value" in the above example

		while(getline(*config_file, line)){

			//cout<<"MAIN LOOP"<<line<<endl;
			if (line.front() == '#') continue; //allow for comments

			if (line.substr(0, 9) == "DEFAULT::"){
				opt_line=line.substr(9);
				//cout<<"Found default opt: "<<opt_line<<endl;
				extractOptFromLine(opt_line);
			}

			else if (line == "Name="+config_name){
				found_config=true;
				extractOptFromLine(line);
				getline(*config_file, line);
				while( ! ( isWhiteSpace(line) || (line.substr(0,5) == "Name=" ) ) ){
					//cout<<"SECOND LOOP"<<line<<endl;
					extractOptFromLine(line);
					getline(*config_file, line);
					if (config_file->eof()){
						break;
					}
				}
				config_file->seekg(0,config_file->beg);
				break;
			}
		}
		
		config_file->seekg(0,config_file->beg);
		

		if (! found_config){
			cout<<"configuration not found!"<<endl;
			options.clear();
			return false;
		}

		return true;
	}

	void print(){
		for (std::map<string,string>::iterator it=options.begin(); it!=options.end(); it++){
	    	cout << it->first << " => " << it->second << endl;
		}
	}

	string get(string key){
		return options[key];
	}

	string operator [](string key) {
		return options[key];
	}

};


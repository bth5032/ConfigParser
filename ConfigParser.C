#ifndef INCLUDED_CONFIG_PARSER
#define INCLUDED_CONFIG_PARSER

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>	
#include <map>

using namespace std;

bool isWhiteSpace(const string &str){
	for (int i = 0; i<(int)str.size(); i++){
		if ( (! (str[i] == ' ')) && (! (str[i] == '\t') ) ){
			return false;	
		}
	}
	return true;
}

class ConfigParser{

private:
	ifstream *config_file;
	string conf_path;
	map<string, string> options;
	map<string, string> default_options;
	int currentLocation=0;

	string cleanedArg(const string &arg){
		string cleaned = arg.substr(0,arg.find("#")); //get argument before comment
		cleaned = cleaned.substr(0, cleaned.find_last_not_of(" \t")+1); //strip off tabs and spaces.

		return cleaned;
	}

	bool addOpt(string opt_key, string opt_value, bool default_opt=false){
		if(opt_key != "" && opt_value != ""){
			//if default option, add it to defaults dict
			if (default_opt){
				default_options[opt_key] = cleanedArg(opt_value);	
			} 
			else{
				options[opt_key] = cleanedArg(opt_value);
			}
			return true;
		}
		else{
			return false;
		}
	}

	void extractOptFromLine(string line, bool default_opt=false){
		/*Extract a value from a line*/
		string opt_key; // holds key from line
		string opt_value; // holds value for key
		opt_key=line.substr(0, line.find('='));
		opt_value=line.substr(line.find('=')+1);

		if ( ! addOpt(opt_key, opt_value, default_opt)){
			cout<<"Error parsing line number: "<<config_file->tellg()<<" : "<<line<<endl;
		}
	}

public:

	ConfigParser(string filename){
		//just opens the file stream 
		config_file = new ifstream(filename);
		conf_path=filename;
	}

	string findFirstConfig(){
		config_file->seekg(0,config_file->beg); //set to begining
		return findNextConfig();
	}

	bool hasKey(const map<string, string> &m, const string &key){
		//returns true if the map has the key, false otherwise.
		map<string, string>::const_iterator i = m.find(key);
		if (i != m.end()){
			return true;
		}
		else{
			return false;
		}
	}

	string findNextConfig(){
		//Finds and returns name token in next line with "Name="
		string line;  //holds line of config file
		while(getline(*config_file, line)){
			if (line.front() == '#') continue; 
			if (line.substr(0,5) == "Name="){
				return line.substr(5);
			}
		}
		return "";
	}

	bool loadNextConfig(){
		//loads the next configuration, will load the first one if called before any other config.
		config_file->seekg(currentLocation);
		string next_name=findNextConfig();
		if ( next_name != ""){
			return loadConfig(next_name);
		}
		else{
			return false;
		} 
	}

	bool loadConfig(string config_name){
		//
		//	Starts from the begining of the file and picks up all the defaults until it gets to 
		//  the config_name shown, and then marks the location in currentLocation. If it can not 
		//  find the configuration, it will return false, the currentLocation will be set to 0, and it will
		//  completely clear the defualts and options. Otherwise it will return true and set all the 
		//  defaults from the begining of the file until the next "Name=" line, blank space, or 
		//  the end of the file, overwriting defaults with the same name. No attempt is made to 
		//  put the file in a specific location after the configuration has been loaded, unless the EOF.
		//  is reached, at which point the file is set back to the beginning. 
		//

		bool found_config=false; //lets the user know whether we actually found a match. 
		config_file->clear();
		config_file->seekg(0,config_file->beg); //always start from the begining of the file to pick up defaults, will slow down everything some amount but whatever.
		
		options.clear(); // clear all the non-default options
		default_options.clear(); // clear default options as well

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
				extractOptFromLine(opt_line, true); //if we find an option this way, add it to the defaults map. 
			}

			else if (line == "Name="+config_name){
				addOpt("conf_path", conf_path);
				currentLocation=config_file->tellg();
				found_config=true;
				extractOptFromLine(line); // should extract the name
				getline(*config_file, line);
				//get lines while we don't have white space or a new config.
				while( ! ( isWhiteSpace(line) || (line.substr(0,5) == "Name=" ) ) ){
					//cout<<"SECOND LOOP"<<line<<endl;
					extractOptFromLine(line);
					if (config_file->eof()){
						//If we're at the end of the file, getline returns the last line again, but also sets EOF. 
						break; //break out of config filling loop if EOF is reached 
					}
					getline(*config_file, line); 
				}
				break; //break out of looking for configs/defaults if we've already loaded a config
			}
		}


		if (! found_config){
			cout<<"configuration not found!"<<endl;
			options.clear();
			currentLocation=0;
			default_options.clear();
		}

		return found_config;
	}

	void print(){
		for (std::map<string,string>::iterator it=default_options.begin(); it!=default_options.end(); it++){
	    	cout <<"DEFAULT::"<< it->first << " => " << it->second << endl;
		}
		for (std::map<string,string>::iterator it=options.begin(); it!=options.end(); it++){
	    	cout << it->first << " => " << it->second << endl;
		}
	}

	string get(string key){
		if (hasKey(options, key)){
			return options[key];
		}
		else if (hasKey(default_options, key)) {
			return default_options[key];
		}
		else{
			return "";
		}
	}

	string operator [] (string key) {
		if (hasKey(options, key)){
			return options[key];
		}
		else if (hasKey(default_options, key)) {
			return default_options[key];
		}
		else{
			return "";
		}
	}

};

#endif
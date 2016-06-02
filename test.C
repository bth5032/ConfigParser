#include "ConfigParser.C"
#include <string>

void testRandomAccess(){
	ConfigParser conf("test.conf");
	
	cout<<"==================\n";
	cout<<"Loading Configuration for Name=80"<<endl;
	conf.loadConfig("80");
	conf.print();
	cout<<"==================\n\n";

	cout<<"Loading Configuration for Name=76"<<endl;
	conf.loadConfig("76");
	conf.print();
	cout<<"==================\n\n";

	cout<<"==================\n";
	cout<<"Loading Configuration for Name=80_vtx"<<endl;
	conf.loadConfig("80_vtx");
	conf.print();
	cout<<"==================\n\n";

	cout<<"Loading Configuration for Name=76"<<endl;
	conf.loadConfig("76");
	conf.print();
	cout<<"==================\n\n";

	cout<<"Loading Configuration for Name=80_vtx_mf"<<endl;
	conf.loadConfig("80_vtx_mf");
	conf.print();
	cout<<"==================\n\n";


	cout<<"Loading Configuration for Name=76"<<endl;
	conf.loadConfig("76");
	conf.print();
	cout<<"==================\n\n";

	cout<<"==================\n";
	cout<<"Loading First Configuration"<<endl;
	string name = conf.findFirstConfig();
	cout<<"Config is named: "<<name<<endl;
	if (name=="76"){
		cout<<"Name is 76"<<endl;
	}
	else{
		cout<<"Name is not 76"<<endl;
	}
	conf.loadConfig(name);
	conf.print();
	cout<<"==================\n\n";

	cout<<"Loading Configuration for Name=80_vtx_mf"<<endl;
	conf.loadConfig("80_vtx_mf");
	conf.print();
	cout<<"==================\n\n";

	cout<<"Loading Configuration for Name=76"<<endl;
	conf.loadConfig("76");
	conf.print();
	cout<<"==================\n\n";

	cout<<"Loading Configuration for Name=NONE"<<endl;
	conf.loadConfig("NONE"); //should throw error
	conf.print();
	cout<<"==================\n\n";


	//No breaking after it can't find one
	cout<<"Loading Configuration for Name=76"<<endl;
	conf.loadConfig("76");
	conf.print();
	cout<<"==================\n\n";
}

void testSeqAccess(){
	ConfigParser conf("test.conf");
	while (conf.loadNextConfig()){
		cout<<"CONFIG:"<<endl;
		conf.print();
	}
}

void testConfigReset(){
	ConfigParser conf("test.conf");
	conf.loadNextConfig();
	conf.print();
	conf.loadNextConfig();
	conf.print();
	conf.loadConfig("80");
	conf.print();
	conf.loadNextConfig();
	conf.print();
}

int main(){
	cout<<"Random Access Testing:"<<endl;
	testRandomAccess();

	cout<<"\n\nSequential Testing:"<<endl;
	testSeqAccess();
	
	cout<<"\n\nReset Testing:"<<endl;
	testConfigReset();
}
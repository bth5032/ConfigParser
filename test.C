#include "ConfigParser.C"
#include <string>

void test(){
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

	cout<<"Loading Configuration for Name=80_vtx_mf"<<endl;
	conf.loadConfig("80_vtx_mf");
	conf.print();
	cout<<"==================\n\n";

	cout<<"Loading Configuration for Name=NONE"<<endl;
	conf.loadConfig("NONE"); //should throw error
	conf.print();
	cout<<"==================\n\n";
}

int main(){
	test();
}
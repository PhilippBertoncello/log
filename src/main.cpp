//=============================================
//This is just a little program to test the 
//actual log.cpp.
//You can do whatever you want with this
//program.
//Original author: Philipp Bertoncello
//Date: 2018-02-14
//=============================================

#include<stdio.h>
#include"log.cpp"

using namespace log;

int main(int argc, char *argv[]) {
	clearLogFile();
	loginit(false, true, true, 1, 14, "log.txt");
	progress("Start; ", (char*)"Mid; ", "End" );
	for(int i = 0; i < 1000000000; i++) {

	};
	finishprogress(false);
	progress("Logfile: ", logfile, "; ");
	finishprogress(true);
	info();
	str("Hello World!");
	newline();
	return 0;
}

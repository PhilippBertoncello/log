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
	info();
	str("Hello World!");
	newline();
	return 0;
}

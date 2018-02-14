//=============================================
//This is just a program that should help you
//logging and debugging.
//You can do whatever you want with this
//program.
//Original author: Philipp Bertoncello
//Date: 2018-02-14
//=============================================

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

namespace log {
	bool usespinner = false;
	bool timetolog = true;
	bool timetoterm = true;
	int timezone = 1;
	unsigned int maxinttostroutsize = 3;
	const char* logfile = "log.txt";

	enum State { Done, Fail, Running, Info };
	const char* spinner = "/-\\|";
	int spinnerframes = 4;
	int curspinnerframe = 0;
	int lastspinnerupdate;
	int spinnerupdateinterval = 1;
	bool activespinner = false;
	const char* startProg;
	const char* midProg;
	const char* endProg;
	int months[] = {
					31, 28, 31, 30,
					31, 30, 31, 31,
					30, 31, 30, 31,
					31, 28, 31, 30,
					31, 30, 31, 31,
					30, 31, 30, 31,
					31, 28, 31, 30,
					31, 30, 31, 31,
					30, 31, 30, 31,
					31, 29, 31, 30,
					31, 30, 31, 31,
					30, 31, 30, 31
	};
	
	float abs(float in) {
		if(in < 0) {
			return -in;
		}
		return in;
	}

	int floor(float in) {
		return (int)in;
	}
	
	int getlen(const char* str) {
		for(int i = 0; i < 255; i++) {
			if(str[i] == '\0') {
				return i;
			}
		}
		return -1;
	}

	char* inttofixstr(int inp, unsigned int fixlen) {
		char* strin = (char*)malloc(sizeof(char) * maxinttostroutsize);
		char* strout;
		int strinlen;
		sprintf(strin, "%d", inp);
		strinlen = getlen(strin);
		strout = (char*)malloc(sizeof(char) * (fixlen + 1));
		
		if(strinlen == fixlen) {
			free(strout);
			return strin;
		}
		if(strinlen < fixlen) {
			for(int i = 0; i < fixlen - strinlen; i++) {
				strout[i] = '0';
			} 
			for(int i = 0; i < strinlen; i++) {
				strout[i + fixlen - strinlen] = strin[i];
			}
			return strout;
		}
		for(int i = 0; i < fixlen; i++) {
			strout[i] = strin[i + strinlen - fixlen];
		}
		return strout;
	}

	char* gettime() {
		int secsfrommn = (int)time(NULL) % 86400;
		int hours = floor(secsfrommn / 3600) + timezone;
		int mins = floor(secsfrommn % 3600 / 60);
		int secs = floor(secsfrommn % 3600 % 60);
		char* ret = (char*)malloc(sizeof(char) * 14);
		sprintf(ret, "%s:%s:%s", inttofixstr(hours, 2),
														inttofixstr(mins, 2), inttofixstr(secs, 2));
		return ret;
	}

	char* getdate() { //very precise, but cpu intensive...
										//that's ok because it runs only once per program
		int unixts = (int)time(NULL);
		int q = floor((unixts - 63158400) / 126230400);
		int qs = floor(unixts - (q * 126230400) - 63158400);
		int qy = floor(qs / 31557600);
		int qm = 0;
		int qd = floor(qs / 86400);

		int n = 0;
		for(int i = 0; i < 48; i++) {
			n += months[i];
			if(qd < n) {
				qm = i;
				break;
			}
		}

		int year = 1972 + 4 * q + qy;
		int month = qm - 12 * qy + 1;
		int day = qd - n + 1;

		char *ret = (char*)malloc(sizeof(char) * 11);
		sprintf(ret, "%s.%s.%s", inttofixstr(year, 4),
										inttofixstr(month, 2), inttofixstr(day, 2));
		return ret;
	}

	void appendToFile(const char* file, const char* string) {
		FILE *fptr;
		fptr = fopen(file, "a");
		fwrite(string, 1, getlen(string), fptr);
		fclose(fptr);
	}

	void newline() {
		printf("\n");
		appendToFile(logfile, "\n");
	}

	void str(const char* string) {
		printf("%s", string);
		appendToFile(logfile, string);
	}

	void writeState(State state) {
		str(gettime());
		str(": ");
		switch (state) {
			case Done:
				printf("\e[32m[Done!]\e[0m ");
				break;
			case Fail:
				printf("\e[31m[Fail!]\e[0m ");
				break;
			case Running:
				printf("\e[33m[ ... ]\e[0m ");
				appendToFile(logfile, "Working:\n");
				break;
			case Info:
				printf("\e[33m[Info:]\e[0m ");
		}
		fflush(stdout);
	}

	bool loginit(bool spin, bool timelog, bool timeterm, int timez,
									unsigned int maxinttostr, const char* file) {
		usespinner = spin;
		timetolog = timelog;
		timetoterm = timeterm;
		timezone = timez;
		maxinttostroutsize = maxinttostr;
		logfile = file;
		appendToFile(logfile, getdate());
		appendToFile(logfile, ":\n");
	}

	void setSpinner() {
		printf("|");
		activespinner = true;
	}

	void cancelSpinner() {
		activespinner = false;
	}

	void updateSpinner() {
		if(!activespinner) return;
		curspinnerframe++;
		if (curspinnerframe >= spinnerframes) curspinnerframe = 0;
		printf("\b%c", spinner[curspinnerframe]);
		fflush(stdout);
	}

	void checkforspinnerupdate() {
		if(!activespinner) return;
		if((int)time(NULL) >= lastspinnerupdate + spinnerupdateinterval) {
			updateSpinner();
			lastspinnerupdate = (int)time(NULL);
		}
	}

	void info() {
		writeState((State)3);
		appendToFile(logfile, "Information: ");
	}

	void inte(int i) {
		printf("%d", i);
		char *str;
		sprintf(str, "%d", i);
		appendToFile(logfile, str);
	}

	void progress(const char* start, const char* part, const char* end) {
		writeState((State)2);
		printf("%s", start);
		printf("%s", part);
		printf("%s", end);
		printf("%s", "... ");
		if(usespinner) setSpinner();
		startProg = start;
		midProg = part;
		endProg = end;
		fflush(stdout);
	}

	void finishprogress(bool success) {
		int length = getlen(startProg) + getlen(midProg) + getlen(endProg)
						+ 12 + (usespinner ? 1 : 0) +
						(timetoterm ? (getlen(gettime()) + 2) : 0);
		for(int i = 0; i < length; i++) {
			printf("\b");
		}
		writeState((State)(success ? 0 : 1));	
		appendToFile(logfile, success ? "Success: " : "FAILED: ");
		str(startProg);
		str(midProg);
		str(endProg);
		printf("     ");
		newline();
	}

	void clearLogFile() {
		FILE *fptr = fopen(logfile, "w");
		fclose(fptr);
	}
};

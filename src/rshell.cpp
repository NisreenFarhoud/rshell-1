#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <vector>
#include "comParse.h"
#include <cstring>
#include <unistd.h>

using namespace boost;
using namespace std;

int main() {
	while (true) {
		char* login = getlogin();
		char* hostname = NULL;
		gethostname(hostname, strlen(hostname));

		if (login != NULL && hostname != NULL) cerr << login << "@" << hostname;
		else if (login != NULL) cerr << login;
		else if (hostname != NULL) cerr << hostname;
		cerr << "$ " << flush;
		
		string command = "";
		getline(cin, command);	// Get command

		comParse cp;
		char* const* argv = cp.parseLine(command); // Parse line

		char ex[5] = {'e','x','i','t','\0'};
		if (strcmp(argv[0], ex) == 0 && cp.size() == 1) return 0; // EXIT command

		int pid = fork();
		if (pid == -1) {	// Error in fork
			perror("error in fork");
			exit(-1);
		} else if (pid == 0) {	// Child Process
			if (-1 == execvp(argv[0], argv)) {
				perror("error in execvp");
				exit(-1);
			}

			exit(1); // should never happen, since child should exec
		} 
		
		int status = -1;
		if (-1 == waitpid(pid, &status, 0)) {	// Parent Process
			perror("error in wait");
			exit(-1);
		}
	}

	return 0;
}


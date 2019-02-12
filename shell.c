/* shell.c - William Spurr - 3 Feb 2019
 * This program is a basic command line shell written for Operating Systems
 * class. It lets you run programs in new processes, show the last 5 created
 * process IDs, and use cd. */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define TOKEN_COUNT 10
#define TOKEN_SIZE 80

void tok_str_cmd(char *tok_cmd[TOKEN_COUNT + 1], char *source);

void add_recent_num(int array[5], int pid, int array_size);


int main () {

	char *prompt = "prompt$ "; //TODO make this dynamic with pwd
	int recent_pids[5] = {0}; // 5 most recent pids created
	// main execution and input loop
	while (1) {
		// init input string and get a user input
		char input[TOKEN_COUNT * TOKEN_SIZE];
		printf("%s", prompt);
		fgets(input, TOKEN_COUNT * TOKEN_SIZE, stdin);
		// remove trailing '\n'
		input[strcspn(input, "\n")] = '\0';

		// test for different inputs
		// 'exit' shell -> break loop and quit
		if (strncmp(input, "exit", 4) == 0) {
			break;
		} 

		// 'showpid' to show most recent child pids created
		else if (strncmp(input, "showpid", 7) == 0) {
			for (int i = 0; i < sizeof(recent_pids) / sizeof(recent_pids[0]); i++) {
			printf("%i\n", recent_pids[i]);
			}
		}

		// TODO add check for cd
		
		// command -> fork -> execvp & waitpid -> return
		else {
			pid_t pid = fork();
			if (pid == 0) { // child process runs new command
				// create strings to hold command and arguments
					// array of pointers to strings, w/ TOKEN_COUNT strings
					// size + 1 for NULL pointer terminator for execvp()
				char *tok_cmd[TOKEN_COUNT + 1];
				// separate command and args
				tok_str_cmd(tok_cmd, input); 
				
				// execute new process by replacing this child
				execvp(tok_cmd[0], tok_cmd);
				
				// child should not reach here, something happened in error
				printf("Command not found\n");
				return 0;
			}
			else { // parent process waits
				waitpid(-1, NULL, 0);

				// add child pid to list of recent ones
				add_recent_num(recent_pids, pid, sizeof(recent_pids) / sizeof(recent_pids[0]));
			}
		} // END command if statement
		
	} // END while

	return 0;
} // END main()


/* Takes a source string and splits it on the string " ". It sets cmd to
 * the command to be executed, and args to the arguments to pass to that
 * command. */
void tok_str_cmd(char *tok_cmd[TOKEN_COUNT + 1], char *source) {

	const char s[2] = " ";
	int tok_cnt = 0;
	
	// get first token
	char *tok = strtok(source, s);
	
	//strcpy(tok_cmd[tok_cnt], tok);
	tok_cmd[tok_cnt] = tok;

	// loop through rest of tokens and add to list
	while (tok_cmd[tok_cnt] != NULL) {
		tok = strtok(NULL, s);
		tok_cmd[++tok_cnt] = tok;
	}
}	// END tok_str_cmd()

//void tok_str_cd(); // TODO

/* Adds pid to an array of integers. Puts new num in the last position.
 * Deletes the first num. */
void add_recent_num(int array[], int num, int array_size) {
	
	// loop through all array elements except last
	for (int i = 0; i < array_size - 1; i++) {
		array[i] = array[i+1];
	} // END for

	// set last element
	array[array_size - 1] = num;
} // END add_recent_num()

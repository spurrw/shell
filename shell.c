/* shell.c - William Spurr - 3 Feb 2019
 * This program is a basic command line shell written for Operating Systems
 * class. It lets you run programs in new processes, show the last 5 created
 * process IDs, and use cd. */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define TOKEN_COUNT 10
#define TOKEN_SIZE 80

void tok_str_cmd (char *tok_cmd[TOKEN_COUNT + 1], char *source);

void add_recent_num (int array[5], int pid, int array_size);

void cd (char *string);

int count_char (char *string, char target);

/* Main function. Loops to set the prompt, get user input, fork processses, 
 * and do cd and showpid.
 */
int main () {
	
	int recent_pids[5] = {0}; // 5 most recent pids created
	// main execution and input loop
	while (1) {
		// set prompt text with environment variables
		char *user = getenv("USER");
		char *pwd = getenv("PWD");
		printf("\033[33m");
		printf("%s::%s$ ", user, pwd);
		printf("\033[0m");

		// init input string and get a user input
		char input[TOKEN_COUNT * TOKEN_SIZE];

		fgets(input, TOKEN_COUNT * TOKEN_SIZE, stdin);
		// remove trailing '\n'
		input[strcspn(input, "\n")] = '\0';

		
		// test for different inputs
		// 'exit' shell -> break loop and quit
		if (strncmp(input, "exit", 4) == 0 || 
				strncmp(input, "quit", 4) == 0) {
			break;
		} 

		// 'showpid' to show most recent child pids created
		else if (strncmp(input, "showpid", 7) == 0) {
			for (int i = 0; i < sizeof(recent_pids) / sizeof(recent_pids[0]); i++) {
			printf("%i\n", recent_pids[i]);
			}
		}
		
		// 'cd' to change directory
		else if (input[0] == 'c' && input[1] == 'd' && 
				(strlen(input) == 2 || input[2] == ' ')) {
			cd(input);
		}
		
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

/* Processes a cd command and prints error messages if there are any. */
void cd (char *string) {

	char *pwd = getenv("PWD");

	// too many spaces
	if (count_char(string, ' ') > 1) {
		printf("Invalid number of arguments\n");
	}

	// cd to $HOME
	else if (strlen(string) == 2 || strlen(string) == 3) {
		char *home = getenv("HOME");
		chdir(home);
		setenv("PWD", home, 1);
	}

	// all other cd commands
	else {
		// Copy command, then tokenize
		// Only 2 tokens are ever necessary for cd, so only do it twice!
		char string_copy[strlen(string)];
		strcpy(string_copy, string);
		char *cd = strtok(string_copy, " ");
		char *dest = strtok(NULL, " ");

		// cd nowhere, just a placeholder so I remember it
		if (strncmp(dest, ".", strlen(dest)) == 0);
		
		// cd back one directory
		else if (strncmp(dest, "..", strlen(dest)) == 0) {
			// count number of directories in pwd
			int dir_count = count_char(pwd, '/');

			// cd into /
			if (dir_count == 1) {
				chdir("/");
				setenv("PWD", "/", 1);
			}

			// cd backwards anywhere else
			else {
				// copy pwd before strtok
				char pwd_copy[strlen(pwd)];
				strcpy(pwd_copy, pwd);

				// tokenize pwd
				char *tok_pwd[dir_count];
				char s[2] = "/";
				// get first token
				int tok_cnt = 0;
				tok_pwd[tok_cnt] = strtok(pwd_copy, s);
	
				// loop through rest of tokens and add to list
				while (tok_pwd[tok_cnt] != NULL) {
					tok_pwd[++tok_cnt] = strtok(NULL, s);
				}

				// create new pwd one directory back
			
				// get string size of all directories so we know how much space
					// to allocate for new pwd string
				int pwd_length = 0;
				for (int i = 0; i < dir_count - 1; i ++) {
					pwd_length += 1 + strlen(tok_pwd[i]);
				}
				char pwd_new[pwd_length + 1];
				pwd_new[0] = '\0';

				// concatenate new strings together
				for (int i = 0; i < dir_count - 1; i ++) {
					strcat(pwd_new, "/");
					strcat(pwd_new, tok_pwd[i]);
				}

				// set env to new pwd
				chdir(pwd_new);
				setenv("PWD", pwd_new, 1);
			
			} // END cd backwards anywhere else
		} // END else if ..
		
		// cd anywhere else
		else {
			int chdir_result = 0;

			if (dest[0] == '/') {
				chdir_result = chdir(dest);
				if (chdir_result == 0) {
					setenv("PWD", dest, 1);
				}
			}
			else {
				char pwd_new[strlen(pwd) + strlen(dest) + 1];
				strcpy(pwd_new, pwd);
				strcat(pwd_new, "/");
				strcat(pwd_new, dest);
				chdir_result = chdir(pwd_new);
				if (chdir_result == 0) {
					setenv("PWD", pwd_new, 1);
				}
			}

			if (chdir_result == -1) {
				printf("Error, directory not found: %s\n", dest);
			}
		} // END cd anywhere else

	} // END else
} // END cd ()

/* Counts the number of a char in a C string. */
int count_char (char *string, char target) {
	int count = 0;
	for (int i = 0; i < strlen(string); i ++) {
		if (string[i] == target) {
			count ++;
		}
	}
	return count;
} // END count_char	()

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define TOKEN_COUNT 10
#define TOKEN_SIZE 80

void tok_str_cmd(char *tok_cmd[TOKEN_COUNT + 1], char *source);



int main () {

	char *prompt = "prompt$ "; //TODO make this dynamic with pwd

	// main execution and input loop
	while (1) {
		// init input string and get a user input
		char input[TOKEN_COUNT * TOKEN_SIZE];
		printf("%s", prompt);
		fgets(input, TOKEN_COUNT * TOKEN_SIZE, stdin);
		// remove trailing '\n'
		input[strcspn(input, "\n")] = '\0';
		printf("the input was %s\n", input); // TODO debug delete

		// test for different inputs
		// 'exit' shell -> break loop and quit
		if (strncmp(input, "exit", 4) == 0) {
			break;
		}
		// TODO add checks for showpid and cd
		
		// command -> fork -> execvp & waitpid -> return
		else {
			pid_t pid = fork();
			if (pid == 0) {
				printf("I am child process with pid %i\n", pid);
			
							
				// create strings to hold command and arguments
					// array of pointers to strings, w/ TOKEN_COUNT strings
					// size + 1 for NULL pointer terminator for execvp()
				char *tok_cmd[TOKEN_COUNT + 1];
				// separate command and args
				tok_str_cmd(tok_cmd, input); 
				

				// TOKENIZE
				/*								
				printf("tokenizing string: %s\n", input); // TODO debug del
				const char s[2] = " ";
				int tok_cnt = 0;
	
				// get first token
				char *tok = strtok(input, s);
	
				//strcpy(tok_cmd[tok_cnt], tok);
				tok_cmd[tok_cnt] = tok;

				while (tok_cmd[tok_cnt] != NULL) {
					printf("in loop");
					printf("tokenized: %s\n", tok_cmd[tok_cnt]); // TODO debug del
					tok = strtok(NULL, s);
					tok_cmd[++tok_cnt] = tok;
				}
				*/
				// TOKENIZE

				// execute new process by replacing this child
				execvp(tok_cmd[0], tok_cmd);
				
					
				/*char const* mycmd[4];
				mycmd[0] = "ls";
				mycmd[1] = "-al";
				mycmd[2] = "/";
				mycmd[3] = NULL;
				execvp(mycmd[0], mycmd);
				*/
				
				// child should not reach here, something happened in error
				printf("Something happened!\n");
				return 0;
			}
			else {
				waitpid(-1, NULL, 0);
				printf("I am parent process with child pid %i and am done waiting\n", pid);
			}
		} // END command if statement
		
	} // END while

	return 0;
} // END main()


/* Takes a source string and splits it on the string " ". It sets cmd to
 * the command to be executed, and args to the arguments to pass to that
 * command. */
void tok_str_cmd(char *tok_cmd[TOKEN_COUNT + 1], char *source) {

				printf("tokenizing string: %s\n", source); // TODO debug del
				const char s[2] = " ";
				int tok_cnt = 0;
	
				// get first token
				char *tok = strtok(source, s);
	
				//strcpy(tok_cmd[tok_cnt], tok);
				tok_cmd[tok_cnt] = tok;

				while (tok_cmd[tok_cnt] != NULL) {
					printf("in loop");
					printf("tokenized: %s\n", tok_cmd[tok_cnt]); // TODO debug del
					tok = strtok(NULL, s);
					tok_cmd[++tok_cnt] = tok;
				}
}	// END tok_str_cmd()

//void tok_str_cd(); // TODO

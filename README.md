William Spurr
OS Project 1 - Shell in C

==== Submission Files ====

shell.c
- source code for the shell

README
- this file

==== Compiling Program and Running ====
	You can compile my shell program by typing the command
'gcc -o shell shell.c'

	To run the shell program, type ./shell from the directory you placed
the binary file in. Make sure it is executable by and use the command
'chmod a+x shell' if needed.

	You can run any executable file from this shell by typing its name.
The shell also has the following extra functions:

showpid = shows the last 5 process IDs created by the shell

cd = change directory and updates the PWD environment variable accordingly


==== Challenges ====

	The first challenge I encountered was how to remove a trailing '\n'
from a string gotten from stdin by fgets. I found a solution at
https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input using the function strcspn.

	I had a lot of problems using strtok to tokenize me input strings so
execvp could read them properly. I eventually fixed it, but I'm not entirely
sure how it was all fixed.
	I used https://www.geeksforgeeks.org/pass-2d-array-parameter-c/ to learn
how to pass an array of pointers to strings to a function. I was then having
issues where strtok wasn'y returning me any tokenized output past the first
token, or else I didn't know how to read it. I modified my strtok call loop
with help from https://stackoverflow.com/questions/15472299/split-string-into-tokens-and-save-them-in-an-array . I'm not really sure how, but
simplifying my loop using the example from that post made everything work.

	The following website told me how to change what colors printf printed:
http://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html

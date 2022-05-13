# C-Shell
A user-defined interactive shell program that can create and manage new processes.

## Overview
C-Shell supports the following features :

1. A prompt that displays username, systemname and current working directory.
2. Builtin Commands: `pwd`, `cd`, `echo`, `ls`, `pinfo`, `history`, `repeat`, `jobs`, `fg`, `bg`, `sig`, `replay`.
3. All other System Commands (with and wihout arguments).
4. Process management (executing foreground and background) processes.
5. Input/Output Redirection
6. Piping
7. Signal Handling for signals like SIGCHLD, SIGINT and SIGTSTP.

## Running and exiting the shell
In the source directory

- Run the make command.
- Run ./a.out to start the shell.
- Enter exit to exit the shell.

## Implementation Details

- The directory from which the shell is invoked is the home directory of the shell, represented by `~`.

- C-Shell supports `;` separated list of commands. C-Shell also handles random multiple `tabs` and `spaces` in the command.

- `Prompt` For every next command a prompt of the format `<username@systemname:curr_dir>` is printed.

#### Built-in Commands
1. Apart from basic functionality `cd` command supports multiple flags `.`, `..`, `-`, `~`. More than one command-line arguments are not allowed. `cd` with no flags or arguments cd into home directory.

2. `echo` command displays the line of text/string that is passed as a command line argument.

3. `pwd` command prints the path of the current working directory.

4. `ls` commands with multiple flags `-a`, `-l`, multiple directory/file names. Handles arguments in any order.

5. `pinfo` prints process related information. `pinfo` accepts one command line argument that is PID and prints the process info of the given `PID`. With no command-line arguments, it prints the process info of the `shell program` itself. Process info printed includes, `pid`, `process status`, `virtual memory (in kB)`, `executable path`. `+` in process status signifies that the process is running in `foreground`.  

6. `repeat` command executes the given instruction multiple times. Format of the command will be `repeat n instruction`, where n is a positive integer specifying the number of times `instruction` should be repeated. `Limitation` - `repeat` does not work with `background` processes.

7. `history` command prints the last `10` commands executed by the shell across all sessions. `history` accepts a positive integer (say `n)` as a command line argument and prints the last n commands executed by the shell across all sessions. At `max`, the shell stores `20` commands in history.

8. `jobs` command prints a list of all currently running background processes spawned by the shell, along with their job number, process ID and state, which can either be `Running`​ or `​Stopped`​. It also supports flags `-r` and `-s` for printing only running or stopped processes respectively.

9. `sig` command takes two command-line arguments, the first is the `job number` and second is the `signal number`. It sends the signal corresponding to signal number​ to that process specified by the job number. 

10. `fg` takes `job number` as a command line argument and brings the running or stopped background job corresponding to `job number`​ to the foreground, and changes its state to ​`running`.​ If no job with the given job number exists, it throws an error.

11. `bg` takes `job number` as a command line argument and changes the state of the background job corresponding to the `job number` from `stopped` to `running` (in the background). If the process was already running in the background, it has no effects.

12. `replay` command takes 3 command line arguments, `command`, `interval` and `period`. It executes `command` in a fixed `interval` of time for a certain `period`. 

#### System Commands - Foreground and Background Processes

Other than the builtin commands, C-Shell executes all other system commands either in foreground or background. It supports the `&` operator which lets a program run in the background after printing the `pid` of the newly created process. Running a process in background implies that the shell will spawn that process and doesn't wait for the process to exit. It will keep taking other user commands. SeaShell can handle multiple background processes at a time. This implies that your shell will spawn that process and doesn't wait for the process to exit. It will keep taking other user commands. When the background process exits the shell, it display the appropriate message to the user.

#### I/O Redirection and Piping

Using the symbols `<`, `>` and `>>`, the output of commands ​can be redirected to a file other than stdout, or the input taken from a file other than ​stdin​. Input and output redirection can be used simultaneously. Error message is displayed if the input file does not exist. If the output file does not exist, it is created (with permissions `​0644`​).
Output file is overwritten in case of `>`​ and appended to in case of ​`>>`.

A pipe `|`, redirects the output of the command on the left as input to the command on the right. `Multiple` commands can piped. 

Simultaneous I/O Redirection and Piping is also supported.
- Syntax for I/O redirection: <command> < <inputfilename> > <outputfilename>
- Syntax for piping: <command1> | <command2> | <command3> .... | <commandn>
- Syntax for Both implemented together: <command1> < <inputfilename> | <command2> | ... | <commandn> > <outputfilename>

#### Signal Handling

`Ctrl C` - interrupts any currently running foreground job, by sending it the `​SIGINT`​ signal. It has no effect in case no foreground process is running on the shell.

`Ctrl Z` (`SIGTSTP`)- pushes any currently running foreground job into the background, and change its state from running to stopped.
If no foreground process is running on the shell, it has no effect.

`Ctrl D` - logs you out of your shell, without having any effect on the actual terminal. 

### Assumptions:
 - Each command is no longer than 200 characters.
 - There are no more than 200 background processes running at a time.
 - In ls command, if the file is 6 months old, year is diplayed instead of time.
   Criterion for 6 months old: 
       - If the year differ by 1 or more, then true.
       - If year is same and difference between present month and file's month is more than 6 then true.
       - If year is same and difference between present month and file's month is 6, if present day of month is more than file's day of month then true.
       - Rest all cases false.



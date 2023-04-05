# pipex - a 42 project

## Description
This project is about reproducing the behaviour of the bash piping ("|") and redirection operators ("<", ">", "<<", ">>"). It introduces the use of forking new processes, using pipes and redirecting file descriptors.

Read [pipex.pdf](https://github.com/leonyannick/pipex/blob/main/pipex.pdf) for the detailed project instruction. </br>

## Installation
Compile executable with executing `make`.

## Usage
`./pipex <infile> <cmd1> <cmd2> ... <cmdn> <outfile>` (behaves like: `< infile cmd1 | cmd2 | ... | cmdn > outfile`)  
`./pipex here_doc <LIMITER> <cmd> <cmd1> <file>` (behaves like: `<< LIMITER cmd1 | cmd2 | ... | cmdn >> outfile`)

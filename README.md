# Linux-Basic-Command

This repository contains custom implementations of basic Linux shell commands written in C. These commands replicate basic functionalities of their standard counterparts while offering insights into how they work internally. This project is suitable for students and developers looking to deepen their understanding of system programming and shell utilities.

# Features
Myshell to execute commands with basic support for pipes and command parsing.
Individual implementations of Linux commands:
myls: List directory contents.
myps: Display active processes.
mycat: Display file contents.
mywc: Count lines, words, and characters in files.
mymkdir: Create directories.
myecho: Print messages to standard output.
# Technologies Used
C Programming Language: For system-level programming.
Linux Environment: Development and testing platform.

# Setup
Prerequisites
GCC compiler
Linux environment
Steps to Run
Clone the repository:
bash
Copy code
git clone https://github.com/yourusername/shell-command-implementations.git
Navigate to the project directory:
bash
Copy code
cd shell-command-implementations
Build the commands:
bash
Copy code
make build
Add the ~/.bin directory to your PATH if not already done:
bash
Copy code
echo 'export PATH=$PATH:~/.bin' >> ~/.bashrc
source ~/.bashrc
Execute any command using its name:
bash
Copy code
myls
myps
Directory Structure
php
Copy code
.
├── mysh.c        # Shell implementation
├── ls.c          # myls implementation
├── ps.c          # myps implementation
├── cat.c         # mycat implementation
├── wc.c          # mywc implementation
├── mkdir.c       # mymkdir implementation
├── echo.c        # myecho implementation
├── Makefile      # Build instructions
└── README.md     # Project documentation
Contributing
Contributions are always welcome! You can contribute by:

Reporting bugs.
Suggesting features or improvements.
Submitting pull requests for new features or fixes.

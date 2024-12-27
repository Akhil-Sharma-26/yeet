### Notice: 
This has not been fully tested on Windows yet, so if you are using Windows and have found some issues, please give me feedback. :)
using the filesystem library to work with different filesystems.

# Introduction:
- This is a project to make my own git with some improvements.
- Through this, I also want to learn the backend of version control and make my grasp stronger on C++.
- I am trying to solve some of my personal problems and gain an understanding of the Computer filesystem and its management.

## HOW TO USE IN LINUX/UNIX ENVIRONMENT
1. Clone/Download this repository.
2. Open the terminal and navigate to this directory.
3. Run the following command:
```bash
 g++ main.cpp controllers.cpp -std=c++17 -lcryptopp -lz -o yeet 
```
4. Set the permissions for the executable file:
```bash
chmod a+x yeet
```
5. Set the environment variable:
```bash
export PATH=$PATH:/path/to/yeet/repo/
```
6. Run the executable file and initialize an empty yeet repo:
```bash
yeet init .
```

## CAREFUL
- For now, You can only make git init in the pwd, You can not do git init /home/abc/some/folder. This will not work. It will be executed but will give to rise complications. If you want to init in a folder, you have to cd to that folder and then run the command. 
- You can only yeet init in a already made directory. You can not yeet init in a directory that does not exist.

## FUTURE PLANS
- Implement diffs algo on the full repository.
-  Check using diffs algo that whether there are any changes in the files before letting the user to commit it changes.


## USE COMMAND:
- For Commit:
` g++ main.cpp controllers.cpp -std=c++17 -lcryptopp -lz -o yeet && ./yeet commit`
- For Stats:
` g++ main.cpp controllers.cpp -std=c++17 -lcryptopp -lz -o yeet && ./yeet status`

## Make Sure:
you have lcryptopp installed in your system. I am using fedora and I installed it using:
```bash
sudo dnf install cryptopp-devel
```
Search according to your OS.

## Testing The compression:
Use the Testing_inflating.cpp file as follow for decompressing your blob data: 
` g++ -lz Testing_Inflating.cpp -o Testing_inflate && ./Testing_inflate`

You can also use the following command:
`zlib-flate -uncompress < .yeet/objects/5d/8d5a1314ec0f6a5b7f8ac27dfbf53ed82bf3fe > test-abc`




`g++ main.cpp controllers.cpp -std=c++17 -lcryptopp -lz -o yeet && ./yeet init .`
`g++ main.cpp controllers.cpp -std=c++17 -lcryptopp -lz -o yeet && ./yeet commit`
`g++ main.cpp controllers.cpp -std=c++17 -lcryptopp -lz -o yeet && ./yeet status`

## FOR BETTER DEBUGGING:
`g++ main.cpp controllers.cpp -std=c++17 -lcryptopp -lz -fdiagnostics-all-candidates`
or 
use `gdb ./executable`
them `(gdb) run` with argument

## COMMENTS/TODO:
- For now, I am leaving the locks and concurrency of file system and other stuff that I learnt in OS. I will implement them later.
- I am also leaving the error handling for now.
- Temparary Path approach for storing objects will do it later.



https://en.cppreference.com/w/cpp/filesystem

using the filesystem library to work with different filesystems.


## HOW TO USE IN LINUX/UNIX ENVIROMENT
1. Clone/Download this repository.
2. Open the terminal and navigate to the directory where the repository is located.
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
6. Run the executable file:
```bash
yeet
```

## CAREFUL
- For now, You can only make git init in the pwd, You can not do git init /home/abc/some/folder. This will not work. It will be executed but will give to rise complications. If you want to init in a folder, you have to cd to that folder and then run the command. 
- You can only yeet init in a already made directory. You can not yeet init in a directory that does not exist.

## FUTURE PLANS
-  The repo_find() function


## USE COMMAND:
` g++ main.cpp controllers.cpp -std=c++17 -lcryptopp -lz -o yeet && ./yeet commit`

## Make Sure:
you have lcryptopp installed in your system. I am using fedora and I installed it using:
```bash
sudo dnf install cryptopp-devel
```
Search according to your OS.

## Testing The compression:
Use the Testing_inflating.cpp file as follow:
` g++ -lz Testing_Inflating.cpp -o Testing_inflate && ./Testing_inflate`

### Notice: 
This has not been fully tested on Windows yet, so if you are using Windows and have found some issues, please Raise an issue. :)
I am using the filesystem library to make it cross-platform. Use Visual Studio, I've provided the `.sln` file or generate it from the `CMAKE` provided in VS.

# Introduction:
- This is a project to make my own version control with some improvements.
- Through this, I wanted to learn the backend of version control and make my grasp stronger on C++.
- I will be learning a lot about file system, hashing and generating diffs between files.
- and use case of Graphs and Linked lists for history traversal.

## HOW TO USE IN LINUX ENVIRONMENT
1. Clone/Download this repository.
2. Open the terminal and navigate to this directory.
3. Run the following command:
```bash
mkdir build
cd build
cmake ..
make 
```
4. The `cmake ..` will make the a "makefile" and then you can "make" it using `make` command.
5. Go to `build/bin` and you will find the executable. 
6. Run the executable file and initialize an empty yeet repo:
```bash
./yeet init [path]
```
(Or better, don;t go to `build/bin` and
```bash
./build/bin/yeet init . 
```

## Quick Reference

| Command | Description | Example |
|---------|-------------|---------|
| `yeet init .` | Initialize repository | `./yeet init .` |
| `yeet status` | Show changes | `./yeet status` |
| `yeet commit` | Create commit | `./yeet commit` |
| `yeet history` | View commit log | `./yeet history` |
| `yeet branch` | Manage branches | `./yeet branch` |
| `yeet checkout <branch>` | Switch branch | `./yeet checkout feature` |

---

## Known Limitations (working on them!)

- `yeet checkout` changes branch pointer but doesn't update working directory files yet
- All files are automatically staged - no manual `add` support yet
- Cannot choose base branch when creating new branches (uses current branch)
- Executable files are tracked but diffs are not displayed in status


## REPO STRUCTURE:
```
YEET/
├── YEET.sln                  # Main solution file
├── YeetCore/                 # Core functionality as static library
├── YeetUtils/                # Utility functions as static library
├── YeetCLI/                  # Command-line interface executable
└── YeetTests/                # Test project
```

## Make Sure:
For now, I have added cryptopp into the repositry itself, but later I will be removing it. So you might need to download it. (For now, no need to do the following steps)
> you have lcryptopp installed in your system. I am using fedora and I installed it using:
> ```bash 
> sudo dnf install cryptopp-devel
> ```
> Search according to your OS.

## Workflow diagrams:
The below are very minimal and abstracted view of the working of the project. You have to go throught the code to learn the details of the implementation.
1. ![Sequence Diagram](assets/Sequence_Diagram.png)
2. ![Data Flow](assets/Data_FLow.png)
3. ![Branching Data Flow](assets/Branching_FLow.png)
4. ![Class Diagram](assets/Class_Diagram.png)


## Testing The compression:
Use the Testing_inflating.cpp file as follow for decompressing your blob data: 
```bash
g++ -lz Testing_Inflating.cpp -o Testing_inflate && ./Testing_inflate
```

You can also use the following command:
```bash 
zlib-flate -uncompress < .yeet/objects/5d/8d5a1314ec0f6a5b7f8ac27dfbf53ed82bf3fe > test-abc
```

## COMMENTS/TODOs:
- For now, I am leaving the locks and while writting and reading file. It might cause error at some cases.
- No structure Error handling for now.
- Temparary Path approach for storing objects => will do it later.
- More robust Testing is needed. (Trying to implement it using GoogleTests.)
- The env variables approach need to change to config file approach.

### 🚧 Coming Soon

- **Checkout** - Reset working directory to match branch  
- **Add** - Stage specific files  
- **Merge** - Merge branches  
- **Remote repositories** - Push/pull to remote  
- **Tags** - Create version tags  
- **Stash** - Save work in progress  
- **Rebase** - Rewrite commit history  
- **Delta encoding** - Space-efficient storage  
- **`.yeetignore`** - Custom ignore patterns  

## Brach
- [ ] Option to choose which branch I want to make my new branch from. (Currently the new branch will automatically start from the prev branch ie the branch you were in)

## Checkout:
- [ ] logic to reset the working directory to the target branch’s commit using tree traversal.

## Commit:
- [ ] Find solution of the problem: what if branches are changed? then how to check for changes?

## Status:
- [ ] Design a new file structure `.diff` to store the diffs content


`Wall -Wextra -pedantic`

Well, what can I do?

1. First of all when Commit happens: Store the name of the path of the file and its generated hash. ie. the folder and the its file name in it.
It can be done by making a key value DS and then writing it in a file. (set or unordered set) ----> DONE ... DONE (Store file successfully created.)

2. Then When I do diffs command with the path. I will:
first go to that commit file where the key value store is present. Find the key/value of the path and takes the hashed value. It will then open the file that stores that files previous content. ----> DONE

It will then pass that compressed content file to the Inflating function which will inflate it. ---> DONE

Then I put Mayer Byers Algo on the data of the current file and previous file and output in terms of lines:
---->> Done

--> My Mayer Byers algo is a little bit wrong. Have to properly understand it and then optimize it.

--> Solve the saving to file issue. SOLVED
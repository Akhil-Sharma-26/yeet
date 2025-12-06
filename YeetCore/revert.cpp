// show the history

// ask for which commit you want to go to

// Tell that this will remove all the commits after that commit

// go back one by one commit removing all the hashes of that commit till we reach that commit id

// now remove all files from the folder

// construct the dirctory from ground up using the file path stored during the creating of each file's blob

// and yeah that's it

void Revert(char* commit_id){
    // check current commit
        // call the helper function, read the hash
    // if curr_oid = commit_id -> return

    // Recurrsivly till commit_id == curr_id{
        // read the contetn of the commit file. 
            // inflate it
        // get the parent commit
        // open the tree oid file
            // inflate it
        // {
            // idr if path is in blob or in tree.
            // in either. delete them
        // }
    // }

    // delete the whole repo except .yeet
    // read the contetn of the commit file. 
        // inflate it
    // get the parent commit
    // open the tree oid file
        // inflate it
    // {
        // idr if path is in blob or in tree.
        // in either. make file
        // inflate content and paste in the files!
    // }
    // you have successfully reverted back to the $commit_id commit!
}
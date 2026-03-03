cd ./build
cmake --build .
cd ..
mv ./build/bin/yeet .
cd testing_dir
touch abc.def
touch poe.sq
../yeet commit
touch fasf.ss
../yeet commit
../yeet revert
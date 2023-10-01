#!/bin/bash
directory='attachments'
if [ ! -d $directory ]; then
    mkdir $directory
    echo "Folder  \"$directory\" created"
fi
for (( i=1; i<= 20; i++ )) do
    curl https://loremflickr.com/800/400 -sL > ./$directory/$i.jpg
    echo "File \"$i.jpg\" downloaded"
done
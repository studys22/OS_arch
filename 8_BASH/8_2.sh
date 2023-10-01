#!/bin/bash
directory='attachments'
if [ -d $directory ]; then
    for filename in ./$directory/*; do
        rm $filename
        echo "$filename deleted"
    done
else
    echo "Folder \"attachments\" doesn't exist"
fi

#!/bin/bash

# Name of the text file (taken from the first argument)
prg_file="$1"

# Output directory
dir_output=$(mktemp -d)

# Name of the new text file
new_prg_file="${dir_output}/$1.new.prg"

# Regular expression to find quoted strings
regex='"[^"]+"'

# Create the output directory if it doesn't exist
mkdir -p "$dir_output/romfs"

# Copy the content of the original file to the new file
cp "$prg_file" "$new_prg_file"

# Get the quoted strings and copy the files if they exist
while IFS= read -r string; do
    # Remove double quotes
    string="${string#\"}"
    string="${string%\"}"

    # Check if the file exists before copying
    if [[ "$string" != *".prg" && "$string" != *".h" && "$string" != *".inc" && "$string" != *".c" && "$string" != *".def" && -e "$string" ]]; then

        if [[ "$string" == *".."* || "$string" == /* ]]; then
            # Use realpath to obtain the full path of the file
            full_path="$(realpath "$string")"

            # Get the filename from the full path
            file_name="$(basename "$full_path")"

            # Get the current directory path and add a slash
            cut_path=$(dirname $PWD)/

            # Get the relative path within "output"
            relative_path="${full_path#$cut_path}"

            curr_dir=$(basename $PWD)/

            relative_path="${relative_path#$curr_dir}"

            if [[ -d "$relative_path" ]]; then
                relative_path="$relative_path/"
            fi

            sed -i "s|$string|$relative_path|g" "$new_prg_file"

            # Copy the file to the output folder while preserving the relative structure
            mkdir -p "$dir_output/romfs/$(dirname "$relative_path")"
            cp "$string" "$dir_output/romfs/$relative_path"

        else
            # Copy the file to the output folder while preserving the relative structure
            mkdir -p "$dir_output/romfs/$(dirname "$string")"
            cp "$string" "$dir_output/romfs/$string"
        fi

    fi
done < <(grep -oE $regex "$new_prg_file")

bgdc $new_prg_file -o ${dir_output}/romfs/game.dcb
NAME=$(basename $1 .prg)

$DEVKITPRO/tools/bin/build_romfs ${dir_output}/romfs ${dir_output}/romfs.bin
$DEVKITPRO/tools/bin/nacptool --create $NAME SplinterGU 1.0 ${dir_output}/$NAME.nacp
$DEVKITPRO/tools/bin/elf2nro $BGD2DEV/build/aarch64-none-elf/bin/bgdi.elf ${dir_output}/$NAME.nro --romfs=${dir_output}/romfs.bin --nacp=${dir_output}/$NAME.nacp --icon=icon.png
cp ${dir_output}/$NAME.nro /home/hdd/Switch-Games

rm -Rf ${dir_output}

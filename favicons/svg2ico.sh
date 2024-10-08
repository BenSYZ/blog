#!/bin/zsh
#https://graphicdesign.stackexchange.com/questions/77359/how-to-convert-a-square-svg-to-all-size-ico
# cc-by-sa

for filename in {favicon_B_Light,favicon_B_Dark,favicon_Smile_Dark,favicon_Smile_Light};do
    for i in {16,32,48};do
	inkscape -w $i -h $i -o $i.png $filename.svg
    done
    convert 16.png 32.png 48.png $filename.ico
done

fixedImg="C1-2830l.vlr.tif"
movingImg="C1-2833l.vlr.tif"
outputImg="output.tif"

open("\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-samples\\"+fixedImg);
open("\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-samples\\"+movingImg);
open("\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-output\\"+outputImg);

imageCalculator("Difference create stack", fixedImg, outputImg);
saveAs("Tiff", "\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-output\\diff.tif")
run("Maximize");

run("Z Project...", "projection=[Max Intensity]");
saveAs("PNG", "\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-output\\diff_max.png")
run("Maximize");

run("Merge Channels...", "c2="+fixedImg+" c6="+outputImg+" create");
saveAs("Tiff", "\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-output\\merged.tif")
run("Maximize");

selectImage(movingImg);
run("Maximize");

run("Tile");
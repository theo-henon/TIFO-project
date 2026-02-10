originImg="C1-2833l.vlr.tif"
transformedImg="output.tif"

open("\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-samples\\"+originImg);
open("\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-output\\"+transformedImg);

imageCalculator("Difference create stack", originImg, transformedImg);
saveAs("Tiff", "\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-output\\diff.tif")
run("Maximize");

run("Z Project...", "projection=[Max Intensity]");
saveAs("PNG", "\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-output\\diff_max.png")
run("Maximize");

run("Merge Channels...", "c2="+originImg+" c6="+transformedImg+" create");
saveAs("Tiff", "\\\\wsl.localhost\\Ubuntu-24.04\\home\\lla\\epita\\tifo-project-output\\merged.tif")
run("Maximize");

run("Tile");
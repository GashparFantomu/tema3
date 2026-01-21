this project uses win flex bison parser to create states and actions from a LR(1) Grammar
steps to run the project
1) download Bison and move the extracted directory to your C: drive (recommended) and rename your folder from win_flex_bison_latest to win_flex_bison (recommended) 
2) in the extracted directory rename the two exe files, from win_bison to bison and from win_flex to bison
3) add the folder to yout path enviroment variables
4) open cmd where you have your .y (yacc) file (idealy in your project folder) and run "bison your_file.y -v"

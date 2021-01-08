# FileReader

Files Included:
  Makefile -> Used to compile and link files
  pandemic.tsv -> Composed of covid-19 stats to be parsed
  tsvToBin.c -> Parses the data in pandemic.tsv into 3 seperate .bin files
  pandemicStats.c -> Allows you to process and display the 3 .bin files produced by tsvToBin.c

Compilation Instructions:
  Compile and link both tsvToBin.c and pandemicStats.c by running the 'Makefile'
  
Running Instruction:
  No arguments or flags are required
  
  First run './tsvToBin' to generate binary file of parsed data
  Then run './pandemicStats' to display an organization of data

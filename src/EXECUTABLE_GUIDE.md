#ALL FUNCTIONS

***dtoul***
Usage: dtoul\_b infilename outfilename

Reads an input binary file full of 8-byte double values, and convert them
into unsigned long representation of the same values.

***to_epoch***
Usage: to\_epoch infilename outfilename

Reads an input binary file full of 8 byte double values that represents
seconds since 1900-01-01 00:00 and convert them to seconds since Epoch.

***analyze***
Usage: temp\_analyze LOW HIGH threshold ul\_ts\_filename infilename..

Reads from one or more binary data files(with data encoded as doubles) and a corresponding
timestamps file, and prints to the output each and every intervals of time where data values 
are between user-defined upper and lower bounds. Only intervals with length greater than 
user-defined threshold value will be display, but all intervals are going to be included in the
summary section as part of the total statistics

***len_cmp***
Usage: len\_cmp binary\_file\_1 binary\_file\_2
Reads from and compares the length of 2 files in terms of number of 8-bytes, and prints to the
output the count for each of the files
####(DON'T USE; JUST GO WITH 'du --apparent-size -B 8'; I DIDN'T KNOW ABOUT THAT WHEN I WROTE IT)


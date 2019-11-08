# ALL FUNCTIONS

### **dtoul**  
> dtoul\_b infilename outfilename

Reads an input binary file full of 8-byte double values, and convert them
into unsigned long representation of the same values.  

### **to\_epoch**  
> to\_epoch infilename outfilename

Reads an input binary file full of 8 byte double values that represents
seconds since 1900-01-01 00:00 and convert them to seconds since Epoch.  

### **get\_interval**  
> get\_interval LOW HIGH threshold ul\_ts\_filename infilenames...

Reads from one or more binary data files(with data encoded as doubles) and a corresponding
timestamps file, and prints to the output each and every intervals of time where data values 
are between user-defined upper and lower bounds. Only intervals with length greater than 
user-defined threshold value will be display, but all intervals are going to be included in the
summary section as part of the total statistics  

### **len\_cmp**  
> len\_cmp binary\_file\_1 binary\_file\_2

Reads from and compares the length of 2 files in terms of number of 8-bytes, and prints to the
output the count for each of the files  
_(DON'T USE; JUST GO WITH 'du --apparent-size -B 8'; I DIDN'T KNOW ABOUT THAT WHEN I WROTE IT)_  

### **nc\_extract**  
> nc\_extract input.nc outfilename variable\_name

Takes a netcdf file, extract the data of a 1-D variable out of it and encode the data as binary,
(i.e. for double precision floating points, it'll encode as 8-byte chunks one after another). If
the specified output file already exists, the data will be appended to the file, otherwise a new
file will be created.  
This program uses the NetCDF library so it requires manual linking when compiling, an example of
compiling cammand using GCC would be:  
> gcc -lnetcdf -lhdf5\_hl -lhdf5 -lz -lm -o nc\_extract nc\_extract.c  

# Visualization-OOI-TMPSF
Generalized tools and procedures to work with data from the TMPSF instruments

## Working with the Raw Data
Since the data is in NetCDF format, you need to install the NetCDF package to be able to work with
them.  
On OS X, this can be accomplished in the terminal by running:  
> brew install netcdf

If the prompt says you don't have homebrew installed, you can then follow its instruction to install
homebrew.

Included in the src/ folder is a C file *nc_extract.c* that can extract a variable out of a *.nc*
file and encode it in binary. Because it uses the netcdf library, compiling the program requires
some manual linking. For example, to compile it with gcc, you'll need to specify options as:  
> gcc -lnetcdf -lhdf5\_hl -lhdf5 -lz -lm -o nc\_extract [src\_file]  

## Sample Raw Data
This repo provides some sample data to experiment with. The *.nc* file contains one month's worth of
data collected by the TMPSF thermistor array installed at the ASHES Vent field during September, 
along with some header informations. (p.s. the NetCDF package provides command-line tools to work
with the *.nc* files as well; more info [here][1].)

<!--- ALL LINKS GO HERE -->
[1]: https://www.unidata.ucar.edu/software/netcdf/docs/netcdf_utilities_guide.html

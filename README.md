# Visualization-OOI-TMPSF
Generalized tools and procedures to work with data from the TMPSF instruments

## Working with the Raw Data
Since the data is in NetCDF format, you need to install the NetCDF package to be able to work with
them. The NetCDF package can be easily installed for most package managers; for example on OS X,
this can be accomplished in the terminal by running:  
> brew install netcdf

If the prompt says you don't have homebrew installed, you can then follow its instruction to install
homebrew.

## The Source Code
In the src/ folder are codes written in C that might help with your data processing. Their rough
documentation is [here][2].  
*Because the project is at a very early stage, codes are not guaranteed to work unless marked so on
the documentation above*  

## Sample Raw Data
In the sample\_raw\_data folder, this repo provides some sample data to experiment with. The *.nc*
file contains one month's worth of data collected by the TMPSF thermistor array installed at the
ASHES Vent field during September, along with some header informations. (p.s. the NetCDF package
provides command-line tools to work with the *.nc* files as well; more info [here][1].)

## Extracted Sample Data
The extracted\_sample\_data/ folder contains binary data files extracted with the tools in src/.

<!--- ALL LINKS GO HERE -->
[1]: https://www.unidata.ucar.edu/software/netcdf/docs/netcdf_utilities_guide.html
[2]: src/guide.md

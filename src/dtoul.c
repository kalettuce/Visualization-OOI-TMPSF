#include <stdio.h>

int main(int argc, char *argv[]) {
    // argument check
    if (argc != 3) {
        fprintf(stderr, "Usage: dtoul_b infilename outfilename\n");
        return -1;
    }

    // set up io files and containers
    FILE *input = fopen(argv[1], "r");

    if (!input) {
        fprintf(stderr, "Input file (%s) does not exist\n", argv[1]);
        return 1;
    }

    FILE *output = fopen(argv[2], "w");

    if (!output) {
        fprintf(stderr, "Insufficient privilege to access/create output file (%s).\n", argv[2]);
        return 2;
    }

    double input_buffer;
    unsigned long output_buffer;
    unsigned long counter = 0;
    
    // traversing through the input file
    while (fread(&input_buffer, sizeof(double), 1, input)) {
        output_buffer = (unsigned long) input_buffer;
        fwrite(&output_buffer, sizeof(unsigned long), 1, output);
        counter++;
    }

    // see if the input is completely traversed through
    if (feof(input)) {
        printf("Conversion complete. Converted %lu values.\n", counter);
    } else {
        printf("Encountered unknown errors, operation stopped.\n");
    }

    // housekeeping
    fclose(input);
    fclose(output);
    return 0;
}

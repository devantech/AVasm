# Avalanche assembler

### building avasm  
clone the project:  

`$ git clone https://github.com/devantech/AVasm.git`

Navigate to the location where you cloned the project and run the make file.  

`$ cd location/of/AVasm`  
`$ make`

This should create the avasm binary in the build directory. Copy this binary to a location on your drive that is in your _$PATH_ or run loacally with `./avasm`. 

### Useage  
To assemble a file run avasm as such:

`$ avasm file.s`

This should produce the output files needed to create the avalanche processor.  
- pc_data       - Contains the starting locations in memory of every process defined in the program.
- inst_data     - Contains the optcodes that make the program.
- dta_data      - Contains the values for any data registers defined.
- reg_data      - Contains the values for any registers defined.
- config.v      - Assembler created config file.
- <file>lst     - listing file.
  
#### Options  
-d <name> the name of the dta_data.  
-p <name> the name of the inst_data file.  
-l <name> the name of the pc_data file.  
-r <name> the name of the reg_data file.  
-h Help text.  
-v Display the version number.  
-q Quitet mode, turn off the assembler memory useage output.  


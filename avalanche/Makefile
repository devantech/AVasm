# Project setup
PROJ = avalanche
DEVICE = hx8k

# Files
FILES = cpu.v

.PHONY: iceFUN clean burn

iceFUN:
	# synthesize using Yosys
	yosys -p "synth_ice40 -top top -json $(PROJ).json" $(FILES)
	# Place and route using nextpnr
	nextpnr-ice40 -r --hx8k --json $(PROJ).json --package cb132 --asc $(PROJ).asc --freq 90 --opt-timing --pcf iceFUN.pcf
	# Convert to bitstream using IcePack
	icepack $(PROJ).asc $(PROJ).bin

burn:
	iceFUNprog -v $(PROJ).bin

clean:
	rm *.asc *.bin *blif

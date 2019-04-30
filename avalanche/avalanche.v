/*
 *  
 *  Copyright(C) 2018 Gerald Coe, Devantech Ltd <gerry@devantech.co.uk>
 * 
 *  Permission to use, copy, modify, and/or distribute this software for any purpose with or
 *  without fee is hereby granted, provided that the above copyright notice and 
 *  this permission notice appear in all copies.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 *  THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. 
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL 
 *  DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */
 
 //  Avalanche revision 1.0. 
 
`include "config.v"

module avalanche (
	input clk, 
	input [15:0] avDataIn,
	
	output [15:0] avRdAddr,
	output avRd,
	
	output reg avWr,
	output reg[15:0] avWrAddr,
	output reg[15:0] avDataOut
);

wire [8:0] seq;
wire [7:0] PCNum1;

wire [7:0] PCNum2;
wire [15:0] PC2;

wire [15:0] PC3;
wire [7:0] PCNum3;
wire [7:0] Rd_3;
wire [7:0] Rs1_3;
wire [7:0] Rs2_3;
wire [7:0] opcode_3;

wire [15:0] PC4;
wire [7:0] PCNum4;
wire [15:0] Rd_4;
wire [7:0] Rs1_4;
wire [15:0] Rs2_4;
wire [7:0] opcode_4;

wire [15:0] alu_a;
wire [15:0] alu_b;
wire [15:0] immd_5;
wire [15:0] PC5;
wire [7:0] PCNum5;
wire [15:0] Rd_5;
wire [7:0] opcode_5;

wire [15:0] alu_add;
wire [15:0] alu_logic;
wire [15:0] immd_6;
wire [7:0] PCNum6;
wire [15:0] Rd_6;
wire [7:0] opcode_6;
wire [15:0] PC6;

wire equal;
wire less;
wire jbit;
wire zero;

wire [15:0] PCwriteback_data;
wire [7:0] PCwriteback_addr;
wire [15:0] Reg_data;
wire [15:0] Reg_data_addr;
wire PortWRen;
wire RamWRen;
wire Reg_WRen;

	stage1 stage1 (
		.clk(clk),
		.seq(seq)
	);


	stage2 stage2 (
		.clk(clk),
		.seq(seq),
		.addr(PCNum1)
	);
	
	stage3 stage3 (
		.clk(clk),
		.PCwriteback_data(PCwriteback_data),
		.PCwriteback_addr(PCwriteback_addr),
		.PCNum1(PCNum1),		
		.PCNum2(PCNum2),
		.PC2(PC2)
	);
	
	stage4 stage4 (
		.clk(clk),
		.PCNum2(PCNum2),
		.PC2(PC2),
		.Rd(Rd_3),
		.Rs1(Rs1_3),
		.Rs2(Rs2_3),
		.opcode(opcode_3),		
		.PC3(PC3),
		.PCNum3(PCNum3)
	);

	stage5 stage5 (
		.clk(clk),
		.Rd_in(Rd_3),
		.Rs1_in(Rs1_3),
		.Rs2_in(Rs2_3),
		.PCNum_in(PCNum3),
		.opcode_in(opcode_3),
		.PC_in(PC3),
		.Reg_data_addr(Reg_data_addr),
		.Reg_data(Reg_data),
		.WRen(Reg_WRen),
		.Rd(Rd_4),
		.Rs1(Rs1_4),
		.Rs2(Rs2_4),
		.opcode(opcode_4),		
		.PCNum(PCNum4),
		.PC(PC4)
	);	

	stage6 stage6 (
		.clk(clk),
		.Rd_in(Rd_4),
		.Rs1_in(Rs1_4),
		.Rs2_in(Rs2_4),
		.PCNum_in(PCNum4),
		.opcode_in(opcode_4),
		.PC_in(PC4),
		.Reg_data_addr(Reg_data_addr),
		.Reg_data(Reg_data),
		.avDataIn(avDataIn),
		.WRen(Reg_WRen),
		.RamWRen(RamWRen),
		.alu_a(alu_a),
		.alu_b(alu_b),
		.Rd(Rd_5),
		.opcode(opcode_5),		
		.PCNum(PCNum5),
		.PC(PC5),
		.avRdAddr(avRdAddr),
		.avRd(avRd),
		.immd(immd_5)
	);

	stage7 stage7 (
		.clk(clk),
		.alu_a(alu_a),
		.alu_b(alu_b),
		.immd_in(immd_5),
		.PCNum_in(PCNum5),
		.opcode_in(opcode_5),
		.PC_in(PC5),
		.Rd_in(Rd_5),
		.alu_add(alu_add),
		.alu_logic(alu_logic),
		.Rd(Rd_6),
		.opcode(opcode_6),		
		.PCNum(PCwriteback_addr),
		.PC(PC6),
		.immd(immd_6),
		.equal(equal),
		.less(less),
		.jbit(jbit),
		.zero(zero)
	);

	stage8 stage8 (
		.equal(equal),
		.less(less),
		.jbit(jbit),
		.zero(zero),
		.Rd_in(Rd_6),
		.opcode_in(opcode_6),
		.PC_in(PC6),
		.immd_in(immd_6),
		.alu_add(alu_add),
		.alu_logic(alu_logic),
		.PC(PCwriteback_data),
		.data(Reg_data),
		.data_addr(Reg_data_addr),
		.wr_en(Reg_WRen),
		.wr_port(PortWRen),
		.wr_ram(RamWRen)
	);
	
 
	
	always @(posedge clk) begin
      	if( PortWRen ) begin	
      		avWr = 1'b1;
      		avWrAddr = Reg_data_addr;
      		avDataOut = Reg_data;
      	end
      	else begin
      		avWr = 1'b0;
      		avWrAddr = avWrAddr;
      		avDataOut = avDataOut;
      	end
	end

endmodule



// Stage 1 is the sequence counter.
// It provides the address for the 512x8 sequence rom. 
module stage1 (
    input clk,
    output [8:0] seq
);

reg [8:0] PcNumCtr = 0;
	
    always @(posedge clk) begin
	    if (PcNumCtr[8:0] == (sequence_count-1) )  PcNumCtr <= 9'b0;
	    else PcNumCtr[8:0] <= PcNumCtr[8:0] + 9'b1;
    end

    assign seq = PcNumCtr;

endmodule



// Stage 2 is the sequence rom lookup.
// It provides the address in a 256x16 RAM of the next PC to use.
module stage2 (
	input clk,
	input [8:0] seq,
	output [7:0] addr
);

	SeqRam512x8 SeqRam (			// Stores PC addr sequence
		.din(8'b0),
		.write_en(1'b0),
		.waddr(9'b0),
		.raddr(seq),
		.clk(clk),
		.dout(addr)	
	);

endmodule



// Stage 3 is the PC lookup RAM, and the PC address to forward through the pipeline. 
// The PC address is used later to write back the incremented PC or the new jump addres.
module stage3 (
    input clk,
    input [15:0] PCwriteback_data,
    input [7:0] PCwriteback_addr,
    input [7:0] PCNum1,
    output reg [7:0] PCNum2,
    output [15:0] PC2
);

reg [15:0] StartTmr;
reg WrEn;

	initial begin 
		StartTmr = 16'b0;
		PCNum2 = 8'hff;
	end
	
	PCram256x16 PCram (			// Stores up to 256 PC's
		.din(PCwriteback_data),
		.write_en(WrEn),
		.waddr(PCwriteback_addr),
		.raddr(PCNum1),
		.clk(clk),
		.dout(PC2)	
	);

	always @(*) begin
			if((StartTmr==16'hffff)) WrEn = 1;
			else WrEn = 1'b0;
	end


always @(posedge clk) begin
		PCNum2 <= PCNum1; 	
    end

    always @(posedge clk) begin
		if(StartTmr<16'hffff) begin 
			StartTmr <= StartTmr+1;
		end			
		else begin
			StartTmr <= StartTmr;
		end
	end 
		 
endmodule



// Stage 4 is instruction lookup. It also increments and forwards the PC down the pipeline as
// well as the PC address from stage 3.
module stage4 (
    input clk,
    input [7:0] PCNum2,
    input [15:0] PC2,
    output [7:0] Rd,
    output [7:0] Rs1,
    output [7:0] Rs2,
    output [7:0] opcode,
    output reg [15:0] PC3,
    output reg [7:0] PCNum3
);

wire [31:0] instruction;

	initial PCNum3 = 8'hff;
	
	InstructionRam InstRom (			// Stores up to 1024 instructions
		.din(32'b0),
		.write_en(1'b0),
		.waddr(9'b0),
		.raddr(PC2[9:0]),
		.clk(clk),
		.dout(instruction)
	);

    always @(posedge clk) begin
		PC3 <= PC2 + 16'h0001; 	
		PCNum3 <= PCNum2;
    end


    assign Rs2[7:0] = instruction[7:0];
    assign Rs1[7:0] = instruction[15:8];
    assign Rd[7:0] = instruction[23:16];
    assign opcode[7:0] = instruction[31:24];

endmodule



// Stage 5 performs in indirect lookup. 
// If a indirect bit is set, then Rd/Rs2 is treated as the memory address of the data,
// otherwise the data is used directly.
// PC, PCNum and opcode are registered a passed through to the next stage.
module stage5 (
	input clk,
	input [7:0] Rd_in,
	input [7:0] Rs1_in,
	input [7:0] Rs2_in,
	input [7:0] PCNum_in,
	input [7:0] opcode_in,
	input [15:0] PC_in,
	input [15:0] Reg_data_addr,
	input [15:0] Reg_data,
	input WRen,
	
	output reg [15:0] Rd,
	output reg [7:0] Rs1,
	output reg [15:0] Rs2,
	output reg [7:0] PCNum,
	output reg [7:0] opcode,
	output reg [15:0] PC
);

reg [7:0] Rd_d;			// registered prior to the multiplexer
reg [7:0] Rs2_d;		// which selects direct/indirect registers
wire [15:0] Rd_i;
wire [15:0] Rs2_i;

	initial begin 
 		PCNum = 8'hff;
 		Rd = 16'hff;	
 	end

	REGram256x16 Rs2RAM (			// Used for Rs2 indirection
		.din(Reg_data),
		.write_en(WRen),
		.waddr(Reg_data_addr[7:0]),
		.raddr(Rs2_in[7:0]),
		.clk(clk),
		.dout(Rs2_i)	
	);

	REGram256x16 RdRAM (				// Used for Rd indirection
		.din(Reg_data),
		.write_en(WRen),
		.waddr(Reg_data_addr[7:0]),
		.raddr(Rd_in[7:0]),
		.clk(clk),
		.dout(Rd_i)	
	);

    always @(posedge clk) begin
		PC <= PC_in; 	
		PCNum <= PCNum_in;
		opcode <= opcode_in;
		Rs1 <= Rs1_in;
		Rd_d <= Rd_in[7:0];
		Rs2_d <= Rs2_in;
    end

	always @(*) begin
		if(opcode[6]) Rs2[15:0] = Rs2_i[15:0];
		else begin 
			Rs2[7:0] = Rs2_d[7:0];
			Rs2[15:8] = 8'b00000000;
		end
	end
			
	always @(*) begin
		if(opcode[7]) Rd[15:0] = Rd_i[15:0];
		else begin 
			Rd[7:0] = Rd_d[7:0];
			Rd[15:8] = 8'b00000000;
		end
	end		

endmodule



// stage 6 reads the registers for the ALU.
// also combines Rs1/Rs2 for a 16bit immd.
module stage6 (
	input clk,
	input [15:0] Rd_in,
	input [7:0] Rs1_in,
	input [15:0] Rs2_in,
	input [7:0] PCNum_in,
	input [7:0] opcode_in,
	input [15:0] PC_in,
	input [15:0] Reg_data_addr,
	input [15:0] Reg_data,
	input [15:0] avDataIn,
	input WRen,
	input RamWRen,
	
	output reg [15:0] alu_a,
	output reg [15:0] alu_b,
	output reg [15:0] Rd = 16'hff,
	output reg [7:0] PCNum,
	output reg [7:0] opcode,
	output reg [15:0] PC,
	output reg [15:0] avRdAddr,
	output reg avRd,
	output reg [15:0] immd
);

wire [15:0] regRam;
wire [7:0] dataRam;
reg [7:0] Rs2_high;


	REGram256x16 Rs1RAM (				// retrieves Rs1 register
		.din(Reg_data),
		.write_en(WRen),
		.waddr(Reg_data_addr[7:0]),
		.raddr(Rs1_in[7:0]),
		.clk(clk),
		.dout(alu_a)	
	);

	REGram256x16 Rs2RAM (				// retrieves Rs2 register
		.din(Reg_data),
		.write_en(WRen),
		.waddr(Reg_data_addr[7:0]),
		.raddr(Rs2_in[7:0]),
		.clk(clk),
		.dout(regRam)	
	);

	DataRam DataRAM (					// 1024x8 bytewide data ram
		.din(Reg_data[7:0]),
		.write_en(RamWRen),
		.waddr( { !Reg_data_addr[9], Reg_data_addr[8:0] } ),	// Because data ram starts on a 512 byte boundry, we invert address bit 9. This makes the 
		.raddr( { !Rs2_in[9], Rs2_in[8:0]} ),					// block addressing the same as $readmemh so initialised data is in the correct position
		.clk(clk),
		.dout(dataRam)	
	);


	always @(*) begin					// memory decoder
		case (Rs2_high[7:0])
			8'b0: alu_b[15:0] = regRam;
			8'b1: alu_b = avDataIn;
			default: alu_b = {8'b0, dataRam[7:0]};
		endcase
	end
	
	always @(posedge clk) begin
		immd[15:8] <= Rs1_in[7:0];		// for ldi/jal instructions
		immd[7:0] <= Rs2_in[7:0];
	end
	
	
    always @(posedge clk) begin
		PC <= PC_in; 	
		PCNum <= PCNum_in;
		opcode <= opcode_in;
		Rd <= Rd_in;
		Rs2_high[7:0] <= Rs2_in[15:8];	// for the memory decoder, above
		avRdAddr[15:0] <= Rs2_in[15:0];
		if(Rs2_in[9:8] == 2'b01) avRd <= 1'b1;
		else avRd <= 1'b0;
    end

//	assign avRdAddr[15:0] = Rs2_in[15:0];
	
endmodule



// stage 7 is the ALU stage. 
// It also provides outputs for "zero", "equal", "less than" and "bit test".
module stage7 (
	input clk,
	input [15:0] alu_a,
	input [15:0] alu_b,
	input [15:0] Rd_in,
	input [7:0] PCNum_in,
	input [7:0] opcode_in,
	input [15:0] PC_in,
	input [15:0] immd_in,

	output reg [15:0] alu_add,
	output reg [15:0] alu_logic,
	output reg [15:0] Rd = 16'hff,
	output reg [7:0] PCNum,
	output reg [7:0] opcode,
	output reg [15:0] PC,
	output reg [15:0] immd,
	output reg equal,
	output reg less,
	output reg jbit,
	output reg zero
);



	always @(posedge clk) begin
		alu_add <= alu_a + alu_b;				// add
	end
	
	always @(posedge clk) begin
		case (opcode_in[3:0])
			4'b0010: alu_logic <= alu_a | alu_b;				// or
			4'b0011: alu_logic <= alu_a & alu_b;				// and
			4'b0100: alu_logic <= alu_a ^ alu_b;				// xor					
			4'b0101: alu_logic <= alu_a >> 1;					// srl		
			4'b0110: begin
					 case (immd_in[11:8]) 
					 	4'd00: alu_logic <= alu_b & 16'hfffe;		// clr bit
					 	4'd01: alu_logic <= alu_b & 16'hfffd;
					 	4'd02: alu_logic <= alu_b & 16'hfffb;
					 	4'd03: alu_logic <= alu_b & 16'hfff7;
					 	4'd04: alu_logic <= alu_b & 16'hffef;
					 	4'd05: alu_logic <= alu_b & 16'hffdf;
					 	4'd06: alu_logic <= alu_b & 16'hffbf;
					 	4'd07: alu_logic <= alu_b & 16'hff7f;
					 	4'd08: alu_logic <= alu_b & 16'hfeff;
					 	4'd09: alu_logic <= alu_b & 16'hfdff;
					 	4'd10: alu_logic <= alu_b & 16'hfbff;
					 	4'd11: alu_logic <= alu_b & 16'hf7ff;
					 	4'd12: alu_logic <= alu_b & 16'hefff;
					 	4'd13: alu_logic <= alu_b & 16'hdfff;
					 	4'd14: alu_logic <= alu_b & 16'hbfff;
					 	4'd15: alu_logic <= alu_b & 16'h7fff;
					 endcase
				end
			4'b0111: begin
					 case (immd_in[11:8]) 
					 	4'd00: alu_logic <= alu_b | 16'h0001;		// set bit
					 	4'd01: alu_logic <= alu_b | 16'h0002; 
					 	4'd02: alu_logic <= alu_b | 16'h0004; 
					 	4'd03: alu_logic <= alu_b | 16'h0008;
					 	4'd04: alu_logic <= alu_b | 16'h0010;
					 	4'd05: alu_logic <= alu_b | 16'h0020;
					 	4'd06: alu_logic <= alu_b | 16'h0040;
					 	4'd07: alu_logic <= alu_b | 16'h0080;
					 	4'd08: alu_logic <= alu_b | 16'h0100;
					 	4'd09: alu_logic <= alu_b | 16'h0200;
					 	4'd10: alu_logic <= alu_b | 16'h0400;
					 	4'd11: alu_logic <= alu_b | 16'h0800;
					 	4'd12: alu_logic <= alu_b | 16'h1000;
					 	4'd13: alu_logic <= alu_b | 16'h2000;
					 	4'd14: alu_logic <= alu_b | 16'h4000;
					 	4'd15: alu_logic <= alu_b | 16'h8000;
					 endcase
				end
			default: alu_logic <= alu_logic;		
		endcase		
	end

    always @(posedge clk) begin
		PC <= PC_in; 	
		PCNum <= PCNum_in;
		opcode <= opcode_in;
		Rd <= Rd_in;
		immd <= immd_in;
    end

	always @(posedge clk) begin
		if (alu_a == alu_b) 
			equal <= 1;
		else equal <= 0;
	end
	
	always @(posedge clk) begin
		if (alu_a < alu_b) 
			less <= 1;
		else less <= 0;
	end

	always @(posedge clk) begin
		if (Rd_in == 16'b0) 
			zero <= 1'b1;
		else zero <= 1'b0;
	end

	always @(posedge clk) begin
		case (immd_in[11:8])
			4'b0000: jbit <= alu_b[0];
			4'b0001: jbit <= alu_b[1];
			4'b0010: jbit <= alu_b[2];
			4'b0011: jbit <= alu_b[3];
			4'b0100: jbit <= alu_b[4];
			4'b0101: jbit <= alu_b[5];
			4'b0110: jbit <= alu_b[6];
			4'b0111: jbit <= alu_b[7];
			4'b1000: jbit <= alu_b[8];
			4'b1001: jbit <= alu_b[9];
			4'b1010: jbit <= alu_b[10];
			4'b1011: jbit <= alu_b[11];
			4'b1100: jbit <= alu_b[12];
			4'b1101: jbit <= alu_b[13];
			4'b1110: jbit <= alu_b[14];
			4'b1111: jbit <= alu_b[15];
		endcase
	end

endmodule


// stage 8 is write back.
// it decodes jmp istructions against the equal/less than/bit flages to drive the pc writeback mux.
module stage8 (
	input equal,
	input less,
	input jbit,
	input zero,
	input [15:0] Rd_in,
	input [7:0] opcode_in,
	input [15:0] PC_in,
	input [15:0] immd_in,
	input [15:0] alu_add,
	input [15:0] alu_logic,

	output reg [15:0] PC,
	output reg [15:0] data,
	output reg [15:0] data_addr,
	output reg wr_en,
	output reg wr_port,
	output reg wr_ram
);

reg jmp;
wire [15:0] alu_in;

	assign alu_in = (opcode_in[3:0]==4'b0000) ? alu_add : alu_logic;

	always @(*) begin
//		if(opcode_in[7:3] == 5'b10011) begin
		if(opcode_in[5:3] == 3'b011) begin
			case (opcode_in[2:0])
				3'b000: jmp = 1'b1;			// jalr
				3'b001: jmp = equal;		// jeq
				3'b010: jmp = !equal;		// jne
				3'b011: jmp = less;			// jlt
				3'b100: jmp = !less;		// jge
				3'b101: jmp = jbit;			// jbs
				3'b110: jmp = !jbit;		// jbc
				3'b111: jmp = 1'b0;		// default
			endcase
		end
		else jmp = 1'b0;
	end

	always @(*) begin				// always write something back to the PC, either next address or jmp address
		PC = PC_in;
		if(jmp) PC = Rd_in;
		else if(opcode_in==8'h12) PC = immd_in;	// jal
		else if(opcode_in[7:1]==7'b1001010) begin 
			case (opcode_in[0])
				1'b0: if(zero==1'b1) PC = immd_in;	// jz
				1'b1: if(zero==1'b0) PC = immd_in;	// jnz
			endcase
		end
	end	


	always @(*) begin
		case(opcode_in[4:3])
			2'b00:	begin			// applies to add,xor,or,and,srl
						case(Rd_in[15:8])
							8'h00:  begin
								wr_port = 0;
								wr_ram = 0;
								data = alu_in;
								data_addr = Rd_in;
								if(data_addr[7:0]==8'h00) wr_en = 0;	// make reg 0 read only
								else wr_en = 1;							// else allow register write								
							end
							8'h01: begin
								wr_en = 0;
								wr_port  = 1;		// port write
								wr_ram = 0;
								data = alu_in;
								data_addr = Rd_in;
							end
							default: begin
								wr_en = 0;
								wr_port  = 0;
								wr_ram = 1;
								data = alu_in;
								data_addr = Rd_in;
							end
						endcase
				  	end
			2'b10:	begin												// ldi/jal/jz/jnz instructions
						wr_en  = 0;
						wr_port = 0;
						wr_ram = 0;
						data = PC_in;									// for jal
						data_addr = Rd_in;						
						if(opcode_in[7:1]!=7'b1001010) begin			// jz, jnz - no data write back - nothing else to do
							if(data_addr[7:0]!=8'h00) wr_en = 1;		// make reg 0 read only
							if(opcode_in[0]==1) data = immd_in; 		// ldi
						end
					end
			default: begin			// jump instructions
						wr_port = 0;
						wr_ram = 0;
						if(opcode_in[7:0]==8'b10011000) begin
							wr_en  = 1;						// jalr instruction
							data = PC_in;
							data_addr = immd_in[15:8];			// grab RS1 from high byte						
							if(data_addr[7:0]==8'h00) wr_en  = 0;	// make reg 0 read only
							else wr_en = 1;					// else allow register write
						end
						else begin 
							wr_en  = 0;			// other jmp instructions
							data = 16'b0;
							data_addr = 16'b0;						
						end
					end

		endcase
	end
	
endmodule



module SeqRam512x8 (din, write_en, waddr, raddr, clk, dout);
	parameter addr_width = 9;
	parameter data_width = 8;
	input [addr_width-1:0] waddr, raddr;
	input [data_width-1:0] din;
	input write_en, clk;
	output reg [data_width-1:0] dout;
	
	reg [data_width-1:0] mem [(1<<addr_width)-1:0];

   initial begin
     $readmemh("seq_data", mem);
   end

	always @(posedge clk) // Write memory.
	begin
		if (write_en)
			mem[waddr] <= din; // Using write address bus.
	end

	always @(posedge clk) // Read memory.
	begin
		dout <= mem[raddr]; // Using read address bus.
	end

endmodule


module PCram256x16 (din, write_en, waddr, raddr, clk, dout);
	parameter addr_width = 8;
	parameter data_width = 16;
	input [addr_width-1:0] waddr, raddr;
	input [data_width-1:0] din;
	input write_en, clk;
	output reg [data_width-1:0] dout;
	
	reg [data_width-1:0] mem [(1<<addr_width)-1:0];

   initial begin
     $readmemh("pc_data", mem);
   end

	always @(posedge clk) // Write memory.
	begin
		if (write_en)
			mem[waddr] <= din; // Using write address bus.
	end

	always @(posedge clk) // Read memory.
	begin
		dout <= mem[raddr]; // Using read address bus.
	end

endmodule


module REGram256x16 (din, write_en, waddr, raddr, clk, dout);
	parameter addr_width = 8;
	parameter data_width = 16;
	input [addr_width-1:0] waddr, raddr;
	input [data_width-1:0] din;
	input write_en, clk;
	output reg [data_width-1:0] dout;
	
	reg [data_width-1:0] mem [(1<<addr_width)-1:0];

   initial begin
     $readmemh("reg_data", mem);
   end

	always @(posedge clk) // Write memory.
	begin
		if (write_en)
			mem[waddr] <= din; // Using write address bus.
	end

	always @(posedge clk) // Read memory.
	begin
		dout <= mem[raddr]; // Using read address bus.
	end

endmodule


module DataRam (din, write_en, waddr, raddr, clk, dout);
	parameter addr_width = 10;
	parameter data_width = 8;
	input [addr_width-1:0] waddr, raddr;
	input [data_width-1:0] din;
	input write_en, clk;
	output reg [data_width-1:0] dout;
	
	reg [data_width-1:0] mem [(1<<addr_width)-1:0];

   initial begin
     $readmemh("ram_data", mem);	// dta_data - the initialised 8-bit ram
   end

	always @(posedge clk) // Write memory.
	begin
		if (write_en)
			mem[waddr] <= din; // Using write address bus.
	end

	always @(posedge clk) // Read memory.
	begin
		dout <= mem[raddr]; // Using read address bus.
	end

endmodule


module InstructionRam (din, write_en, waddr, raddr, clk, dout);
	parameter addr_width = 11;
	parameter data_width = 32;
	input [addr_width-1:0] waddr, raddr;
	input [data_width-1:0] din;
	input write_en, clk;
	output reg [data_width-1:0] dout;
	
	reg [data_width-1:0] mem [(1<<addr_width)-1:0];

   initial begin
     $readmemh("inst_data", mem);
   end

	always @(posedge clk) // Write memory.
	begin
		if (write_en)
			mem[waddr] <= din; // Using write address bus.
	end

	always @(posedge clk) // Read memory.
	begin
		dout <= mem[raddr]; // Using read address bus.
	end

endmodule





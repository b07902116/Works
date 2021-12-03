module control #(
	parameter ADDR_W = 64,
	parameter INST_W = 32,
	parameter DATA_W = 64
)(
	input		[6:0]	opcode,
	output reg			Branch,
	output reg			MemRead,
	output reg			MemWrite,
	output reg			MemtoReg,
	output reg	[1:0]	ALUOp,
	output reg			ALUSrc,
	output reg			RegWrite
);

	always @(opcode) begin
		case (opcode)
			//LD
			7'b0000011 : begin
				Branch = 0;
				MemRead = 1;
				MemWrite = 0;
				MemtoReg = 1;
				ALUOp = 0;
				ALUSrc = 1;
				RegWrite = 1;
			end
			//SD
			7'b0100011 : begin
				Branch = 0;
				MemRead = 0;
				MemWrite = 1;
				MemtoReg = 0;
				ALUOp = 0;
				ALUSrc = 1;
				RegWrite = 0;
			end
			//BEQ, BNE
			7'b1100011 : begin
				Branch = 1;
				MemRead = 0;
				MemWrite = 0;
				MemtoReg = 0;
				ALUOp = 2'b11;
				ALUSrc = 0;
				RegWrite = 0;
			end
			//ADDI, XORI, ORI, ANDI, SLLI, SRLI
			7'b0010011 : begin
				Branch = 0;
				MemRead = 0;
				MemWrite = 0;
				MemtoReg = 0;
				ALUOp = 0;
				ALUSrc = 1;
				RegWrite = 1;
			end
			//ADD, SUB, XOR, OR, AND
			7'b0110011 : begin
				Branch = 0;
				MemRead = 0;
				MemWrite = 0;
				MemtoReg = 0;
				ALUOp = 0;
				ALUSrc = 0;
				RegWrite = 1;
			end
			//STOP
			7'b1111111 : begin
				Branch = 0;
				MemRead = 0;
				MemWrite = 0;
				MemtoReg = 0;
				ALUOp = 0;
				ALUSrc = 0;
				RegWrite = 0;
			end
		endcase
	end

endmodule
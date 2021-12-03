module control #(
	parameter ADDR_W = 64,
	parameter INST_W = 32,
	parameter DATA_W = 64
)(
	input		[6:0]	opcode,
	output reg			control_ID,
	output reg	[2:0]	control_EX,
	output reg	[1:0]	control_MEM,
	output reg	[1:0]	control_WB
);
	// control_ID : {Branch}
	// control_EX : {ALUOp, ALUSrc}
	// control_MEM : {MEMRead, MEMWrite}
	// control_WB : {MemtoReg, RegWrite}
	always @(opcode) begin
		case (opcode)
			// LD
			7'b0000011 : begin
				control_ID = 0;
				control_EX = 3'b001;
				control_MEM = 2'b10;
				control_WB = 2'b11;
			end
			// SD
			7'b0100011 : begin
				control_ID = 0;
				control_EX = 3'b001;
				control_MEM = 2'b01;
				control_WB = 2'b00;
			end
			// BEQ, BNE
			7'b1100011 : begin
				control_ID = 1;
				control_EX = 3'b110;
				control_MEM = 2'b00;
				control_WB = 2'b00;
			end
			// ADDI, XORI, ORI, ANDI, SLLI, SRLI
			7'b0010011 : begin
				control_ID = 0;
				control_EX = 3'b001;
				control_MEM = 2'b00;
				control_WB = 2'b01;
			end
			// ADD, SUB, XOR, OR, AND
			7'b0110011 : begin
				control_ID = 0;
				control_EX = 3'b000;
				control_MEM = 2'b00;
				control_WB = 2'b01;
			end
			// STOP
			7'b1111111 : begin
				control_ID = 0;
				control_EX = 3'b000;
				control_MEM = 2'b00;
				control_WB = 2'b00;
			end
			// OTHER
			default : begin
				control_ID = 0;
				control_EX = 3'b000;
				control_MEM = 2'b00;
				control_WB = 2'b00;
			end
		endcase
	end

endmodule
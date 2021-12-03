module ImmGen #(
	parameter INST_W = 32,
	parameter DATA_W = 64
)(
	input		[INST_W - 1 : 0]	inst,
	output reg	[DATA_W - 1 : 0]	imm
);

	reg	[6:0]	opcode;
	
	always @(inst) begin
		opcode = inst[6:0];
		case (opcode)
			// LD
			7'b0000011 : begin
				imm = {52'd0, inst[31:20]};	
			end
			// SD
			7'b0100011 : begin
				imm = {52'd0, inst[31:25], inst[11:7]};
			end
			// BEQ, BNE
			7'b1100011 : begin
				imm = {51'd0, inst[31], inst[7], inst[30:25], inst[11:8], 1'd0};
			end
			// ADDI, XORI, ORI, ANDI, SLLI, SRLI
			7'b0010011 : begin
				imm = {52'd0, inst[31:20]};
			end
			// ADD, SUB, XOR, OR, AND
			7'b0110011 : begin
				imm = 0;
			end
			// STOP
			7'b1111111 : begin
				imm = 0;
			end
			// OTHER
			default : begin
				// do nothing
			end
		endcase
	end

endmodule
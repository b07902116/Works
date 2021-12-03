module ALUcontrol #(
	parameter ADDR_W = 64,
	parameter INST_W = 32,
	parameter DATA_W = 64
)(
	input				inst30,
	input		[2:0]	inst,
	input		[1:0]	ALUOp,
	output reg	[4:0]	Optype
);

	always @(*) begin
		if (ALUOp == 2'b11) begin
			// instruction is BEQ or BNE
			Optype = {ALUOp, inst};
		end
		else begin
			if (inst30) begin
				// instruction is SUB
				Optype = {2'b01, inst};
			end
			else begin
				Optype = {2'b00, inst};
			end
		end
	end


endmodule
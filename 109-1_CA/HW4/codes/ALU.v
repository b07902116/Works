module ALU #(
	parameter DATA_W = 64
)(
	input		[DATA_W - 1 : 0]	data_a,
	input		[DATA_W - 1 : 0]	data_b,
	input		[4:0]				Optype,
	output reg	[DATA_W - 1 : 0]	result
);

	always @(*) begin
		case (Optype)
			// SUB
			5'b01000 : begin
				result = data_a - data_b;
			end
			// ADD, ADDI
			5'b00000 : begin
				result = data_a + data_b;
			end
			// SLLI
			5'b00001 : begin
				result = data_a << data_b[4:0];
			end
			// LD, SD
			5'b00011 : begin
				result = data_a + data_b;
			end
			// XOR, XORI
			5'b00100 : begin
				result = data_a ^ data_b;
			end
			// SRLI
			5'b00101 : begin
				result = data_a >> data_b[4:0];
			end
			// OR, ORI
			5'b00110 : begin
				result = data_a | data_b;
			end
			// AND, ANDI
			5'b00111 : begin
				result = data_a & data_b;
			end
			// OTHER
			default : begin
				// do nothing
			end
		endcase
	end

endmodule
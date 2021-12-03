module ALU #(
	parameter ADDR_W = 64,
	parameter INST_W = 32,
	parameter DATA_W = 64
)(
	input		[DATA_W - 1 : 0]	data_a,
	input		[DATA_W - 1 : 0]	data_b,
	input		[4:0]				Optype,
	output reg	[DATA_W - 1 : 0]	result,
	output reg						zero
);

	always @(*) begin
		case (Optype)
			//BEQ
			5'b11000 : begin
				zero = (data_a - data_b == 0)? 1 : 0;
				result = 0;
			end
			//BNE
			5'b11001 : begin
				zero = (data_a - data_b != 0)? 1 : 0;
				result = 0;
			end
			//SUB
			5'b01000 : begin
				zero = 0;
				result = data_a - data_b;
			end
			//ADD, ADDI
			5'b00000 : begin
				zero = 0;
				result = data_a + data_b;
			end
			//SLLI
			5'b00001 : begin
				zero = 0;
				result = data_a << data_b[4:0];
			end
			//LD, SD
			5'b00011 : begin
				zero = 0;
				result = data_a + data_b;
			end
			//XOR, XORI
			5'b00100 : begin
				zero = 0;
				result = data_a ^ data_b;
			end
			//SRLI
			5'b00101 : begin
				zero = 0;
				result = data_a >> data_b[4:0];
			end
			//OR, ORI
			5'b00110 : begin
				zero = 0;
				result = data_a | data_b;
			end
			//AND, ANDI
			5'b00111 : begin
				zero = 0;
				result = data_a & data_b;
			end
		endcase
	end

endmodule
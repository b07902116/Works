module adder #(
	parameter DATA_W = 64
)(
	input		[DATA_W - 1 : 0]	data0,
	input		[DATA_W - 1 : 0]	data1,
	output reg	[DATA_W - 1 : 0]	result
);
	reg	[DATA_W - 1:0]	tmp;
	reg	[12:0]			imm;


	always @(*) begin
		imm = ~data1[12:0] + 1;
		if (data1[12] == 1) begin
			tmp = {51'b0, imm};
			result = data0 - tmp;
		end
		else begin
			result = data0 + data1;
		end
	end

endmodule
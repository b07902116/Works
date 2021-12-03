module mux #(
	parameter DATA_W = 64
)(
	input							control,
	input		[DATA_W - 1 : 0]	data_a,
	input		[DATA_W - 1 : 0]	data_b,
	output reg	[DATA_W - 1 : 0]	result
);

	always @(*) begin
		if (control == 0) begin
			result = data_a;
		end
		else begin
			result = data_b;
		end
	end

endmodule
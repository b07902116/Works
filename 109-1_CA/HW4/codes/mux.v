module mux #(
	parameter DATA_W = 64
)(
	input		[1:0]				control,
	input		[DATA_W - 1 : 0]	data0,
	input		[DATA_W - 1 : 0]	data1,
	input		[DATA_W - 1 : 0]	data2,
	input		[DATA_W - 1 : 0]	data3,
	output reg	[DATA_W - 1 : 0]	result
);

	always @(*) begin
		if (control == 2'b00) begin
			result = data0;
		end
		else if (control == 2'b01) begin
			result = data1;
		end
		else if (control == 2'b10) begin
			result = data2;
		end
		else begin
			result = data3;
		end
	end

endmodule
module register #(
	parameter ADDR_W = 64,
	parameter INST_W = 32,
	parameter DATA_W = 64
)(
	input	[4:0]				rs1_id,
	input	[4:0] 				rs2_id,
	input	[4:0] 				rd_id,
	input						RegWrite,
	input	[DATA_W - 1 : 0]	WBdata,
	output reg	[DATA_W - 1 : 0]	rs1_data,
	output reg	[DATA_W - 1 : 0]	rs2_data,
	output reg						DataEqual
);

	//32 64-bits registers in total
	reg 	[DATA_W - 1 : 0]	register[0:31];
	integer i;

	initial begin
		for (i = 0; i < 32; i = i + 1)
			register[i] = 0;
	end

	always @(*) begin
		// WB stage update register first
		if (RegWrite) begin
			register[rd_id] = WBdata;
		end
		else begin
			// do nothing
		end
		rs1_data = register[rs1_id];
		rs2_data = register[rs2_id];
		DataEqual = (register[rs1_id] == register[rs2_id])? 1 : 0;
	end

endmodule
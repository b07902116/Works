module HazardDetectionUnit #(
	parameter ADDR_W = 64,
	parameter INST_W = 32,
	parameter DATA_W = 64
)(
	input	[4:0]		IFID_rs1,
	input	[4:0] 		IFID_rs2,
	input	[4:0]		IDEX_rd,
	input				IDEX_MEMRead,
	output reg			PCWrite,
	output reg			IFIDWrite,
	output reg	[1:0]	ControlSrc
);
	initial begin
		PCWrite = 1;
		IFIDWrite = 1;
		ControlSrc = 2'b01;
	end

	always @(*) begin
		// load-use hazard
		if (IDEX_MEMRead) begin
			if ((IFID_rs1 == IDEX_rd) || (IFID_rs2 == IDEX_rd)) begin
				PCWrite = 0;
				IFIDWrite = 0;
				ControlSrc = 2'b00;
			end
			else begin
				PCWrite = 1;
				IFIDWrite = 1;
				ControlSrc = 2'b01;
			end
		end
		else begin
			PCWrite = 1;
			IFIDWrite = 1;
			ControlSrc = 2'b01;
		end
	end


endmodule
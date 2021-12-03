module ForwardingUnit #(
	parameter ADDR_W = 64,
	parameter INST_W = 32,
	parameter DATA_W = 64
)(
	input	[4:0]		IDEX_rs1,
	input	[4:0] 		IDEX_rs2,
	input	[4:0]		EXMEM_rd,
	input				EXMEM_RegWrite,
	input	[4:0]		MEMWB_rd,
	input				MEMWB_RegWrite,
	output reg	[1:0]	Forward1,
	output reg	[1:0]	Forward2
);

	always @(*) begin
		// forward for rs1
		if (IDEX_rs1 == EXMEM_rd && EXMEM_rd != 0 && EXMEM_RegWrite == 1) begin
			Forward1 = 2'b10;
		end
		else begin
			if (IDEX_rs1 == MEMWB_rd && MEMWB_rd != 0 && MEMWB_RegWrite == 1) begin
				Forward1 = 2'b01;
			end
			else begin
				Forward1 = 2'b00;
			end
		end
		// forward for rs2
		if (IDEX_rs2 == EXMEM_rd && EXMEM_rd != 0 && EXMEM_RegWrite == 1) begin
			Forward2 = 2'b10;
		end
		else begin
			if (IDEX_rs2 == MEMWB_rd && MEMWB_rd != 0 && MEMWB_RegWrite == 1) begin
				Forward2 = 2'b01;
			end
			else begin
				Forward2 = 2'b00;
			end
		end
	end


endmodule
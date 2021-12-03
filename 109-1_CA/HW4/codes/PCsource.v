module PCsource #(
	parameter DATA_W = 64
)(
	input		BEQorBNE,
	input		Branch,
	input		DataEqual,
	output reg	PCSrc
);
	initial begin
		PCSrc = 0;
	end

	always @(*) begin
		if (Branch) begin
			if (BEQorBNE == 0 && DataEqual == 1) begin
				PCSrc = 1;
			end
			else if (BEQorBNE == 1 && DataEqual == 0) begin
				PCSrc = 1;
			end
			else begin
				PCSrc = 0;
			end
		end
		else begin
			PCSrc = 0;
		end
	end

endmodule
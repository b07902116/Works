module alu #(
    parameter DATA_WIDTH = 32,
    parameter INST_WIDTH = 4
)(
    input                   i_clk,
    input                   i_rst_n,
    input  [DATA_WIDTH-1:0] i_data_a,
    input  [DATA_WIDTH-1:0] i_data_b,
    input  [INST_WIDTH-1:0] i_inst,
    input                   i_valid,
    output [DATA_WIDTH-1:0] o_data,
    output                  o_overflow,
    output                  o_valid
);

	//wires, registers, variables
	reg			[DATA_WIDTH-1:0] o_data_r, o_data_w;
	reg 						 o_overflow_r, o_overflow_w;
	reg							 o_valid_r, o_valid_w;
	reg			[DATA_WIDTH-1:0] neg_a, neg_b;
	reg			[DATA_WIDTH-1:0] mul_overflow;
	integer						 index;

	//continuous assign
	assign o_overflow = o_overflow_r;
	assign o_data = o_data_r;
	assign o_valid = o_valid_r;

	//combination part
	always @(*) begin
		if (i_valid) begin
			case (i_inst)
				4'd0 : begin
					{o_overflow_w, o_data_w} = i_data_a + i_data_b;
					//overflow detection
					if (i_data_a[31] == i_data_b[31]) begin
						if (i_data_a[31] != o_data_w[31]) begin
							o_overflow_w = 1;
						end
					end
					else begin
						o_overflow_w = 0;
					end
				end
				4'd1 : begin
					neg_b = ~i_data_b + 1;
					{o_overflow_w, o_data_w} = i_data_a + neg_b;
					//overflow detection
					if (i_data_a[31] == neg_b[31]) begin
						if (i_data_a[31] != o_data_w[31]) begin
							o_overflow_w = 1;
						end
					end
					else begin
						o_overflow_w = 0;
					end
				end
				4'd2 : begin
					mul_overflow = 0;
					neg_a = ~i_data_a + 1;
					neg_b = ~i_data_b + 1;
					if (i_data_a[31] == i_data_b[31]) begin
						if (i_data_a[31] == 0) begin
							{mul_overflow, o_data_w} = i_data_a * i_data_b;
						end
						else begin
							{mul_overflow, o_data_w} = neg_a * neg_b;
						end
						if (o_data_w[31] != 0 || mul_overflow != 0) begin
							o_overflow_w = 1;
						end
					end
					else begin
						if (i_data_a[31] == 0) begin
							{mul_overflow, o_data_w} = i_data_a * neg_b;
						end
						else begin
							{mul_overflow, o_data_w} = neg_a * i_data_b;
						end
						if (mul_overflow != 0) begin
							o_overflow_w = 1;
						end
						else if (o_data_w[31] == 1) begin
							if ((o_data_w << 1) != 0) begin
								//smaller than 2^(-31)
								o_overflow_w = 1;
							end
							else begin
								//equal to 2^(-31)
								o_overflow_w = 0;
							end
						end
						else begin
							o_data_w = ~o_data_w + 1;
						end
					end
				end
				4'd3 : begin
					if (i_data_a[31] == i_data_b[31]) begin
						if (i_data_a[31] == 0) begin
							o_data_w = i_data_a > i_data_b? i_data_a : i_data_b;
						end
						else begin
							o_data_w = ~i_data_a + 1 < ~i_data_b + 1? i_data_a : i_data_b;
						end
					end
					else begin
						if (i_data_a[31] == 0) begin
							o_data_w = i_data_a;
						end
						else begin
							o_data_w = i_data_b;
						end
					end
					o_overflow_w = 0;
				end
				4'd4 : begin
					if (i_data_a[31] == i_data_b[31]) begin
						if (i_data_a[31] == 0) begin
							o_data_w = i_data_a < i_data_b? i_data_a : i_data_b;
						end
						else begin
							o_data_w = ~i_data_a + 1 > ~i_data_b + 1? i_data_a : i_data_b;
						end
					end
					else begin
						if (i_data_a[31] == 0) begin
							o_data_w = i_data_b;
						end
						else begin
							o_data_w = i_data_a;
						end
					end
					o_overflow_w = 0;
				end
				4'd5 : {o_overflow_w, o_data_w} = i_data_a + i_data_b;
				4'd6 : {o_overflow_w, o_data_w} = i_data_a - i_data_b;
				4'd7 : {o_overflow_w, o_data_w} = i_data_a * i_data_b;
				4'd8 : begin
					o_overflow_w = 0;
					o_data_w = i_data_a > i_data_b? i_data_a : i_data_b;
				end
				4'd9 : begin
					o_overflow_w = 0;
					o_data_w = i_data_a < i_data_b? i_data_a : i_data_b;
				end
				4'd10 : o_data_w = i_data_a & i_data_b;
				4'd11 : o_data_w = i_data_a | i_data_b;
				4'd12 : o_data_w = i_data_a ^ i_data_b;
				4'd13 : o_data_w = ~i_data_a;
				4'd14 : begin
					for (index = 0; index < DATA_WIDTH; index = index + 1)
						o_data_w[index] = i_data_a[DATA_WIDTH - 1 - index];
				end
				default : o_data_w = 0;
			endcase
			o_valid_w = 1;
		end
		else begin
			o_overflow_w = 0;
			o_data_w = 0;
			o_valid_w = 0;
		end
	end

	//sequential part
	always @(posedge i_clk or negedge i_rst_n) begin
		if (~i_rst_n) begin
			o_data_r <= 0;
			o_overflow_r <= 0;
			o_valid_r <= 0;
		end
		else begin
			o_data_r <= o_data_w;
			o_overflow_r <= o_overflow_w;
			o_valid_r <= o_valid_w;
		end
	end
    

endmodule
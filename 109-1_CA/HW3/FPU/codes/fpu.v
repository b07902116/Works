module fpu #(
    parameter DATA_WIDTH = 32,
    parameter INST_WIDTH = 1
)(
    input                   i_clk,
    input                   i_rst_n,
    input  [DATA_WIDTH-1:0] i_data_a,
    input  [DATA_WIDTH-1:0] i_data_b,
    input  [INST_WIDTH-1:0] i_inst,
    input                   i_valid,
    output [DATA_WIDTH-1:0] o_data,
    output                  o_valid
);

    //wires, registers, variables
    reg [DATA_WIDTH-1:0] o_data_r, o_data_w;
    reg                  o_valid_r, o_valid_w;
    reg                  sign_a, sign_b, sign_result;
    reg [7:0]            exponent_a, exponent_b, exponent_result;
    reg [23:0]           fraction_a, fraction_b, fraction_result, precision_a, precision_b, precision_result;
    reg                  R, S;
    reg [1:0]            int;
    integer              exponent_diff, i;

    //continuous assign
    assign o_data = o_data_r;
    assign o_valid = o_valid_r;

    //combinational part
    always @(*) begin
        if (i_valid) begin
            //assign data and ensure abs a > b
            if ((i_data_a[30:23] > i_data_b[30:23]) || ((i_data_a[30:23] == i_data_b[30:23]) && (i_data_a[22:0] > i_data_b[22:0]))) begin
                {sign_a, exponent_a, fraction_a} = {i_data_a[31], i_data_a[30:23], 1'd1, i_data_a[22:0]};
                {sign_b, exponent_b, fraction_b} = {i_data_b[31], i_data_b[30:23], 1'd1, i_data_b[22:0]};
            end
            else begin
                {sign_a, exponent_a, fraction_a} = {i_data_b[31], i_data_b[30:23], 1'd1, i_data_b[22:0]};
                {sign_b, exponent_b, fraction_b} = {i_data_a[31], i_data_a[30:23], 1'd1, i_data_a[22:0]};
            end
            {int, precision_a, precision_b} = 0;

            case (i_inst)
                1'd0 : begin
                    //signed add
                    sign_result = sign_a;
                    exponent_result = exponent_a;
                    //shift smaller one right
                    exponent_diff = exponent_a - exponent_b;
                    {fraction_b, precision_b} = {fraction_b, precision_b} >> exponent_diff;

                    if (sign_a == sign_b) begin
                        {int, fraction_result, precision_result} = {fraction_a, precision_a} + {fraction_b, precision_b};
                        while (int) begin
                            {int, fraction_result, precision_result} = {int, fraction_result, precision_result} >> 1;
                            exponent_result = exponent_result + 1;
                        end
                    end
                    else begin
                        {fraction_result, precision_result} = {fraction_a, precision_a} - {fraction_b, precision_b};
                        fraction_result[23] = exponent_diff? 1'd1 : 1'd0;
                        if (fraction_result || precision_result) begin
                            while (~fraction_result[23]) begin
                                {fraction_result, precision_result} = {fraction_result, precision_result} << 1;
                                exponent_result = exponent_result - 1;
                            end
                        end
                    end
                end
                1'd1 : begin
                    sign_result = sign_a ^ sign_b;
                    {int, fraction_result, precision_result} = (fraction_a * fraction_b) << 1;      //shift to keep fraction_result format the same
                    exponent_result = exponent_a + (exponent_b - 8'd127);
                    while (int) begin
                        {int, fraction_result, precision_result} = {int, fraction_result, precision_result} >> 1;
                        exponent_result = exponent_result + 1;
                    end
                end
            endcase

            //rounding
            R = precision_result[23];
            for (i = 22; i >= 0; i = i - 1)
                S = S | precision_result[i];
            while ((R && S) || (R && ~S && fraction_result[0])) begin
                {int, fraction_result} = fraction_result + 1;
                if (int) begin
                    {int, fraction_result, precision_result} = {int, fraction_result, precision_result} >> 1;
                    exponent_result = exponent_result + 1;
                    R = precision_result[23];
                    S = 1;  //R = 1 shift left
                end
                else begin
                    //no need to round => break the loop
                    {R, S} = 0;
                end
            end
            o_data_w = {sign_result, exponent_result, fraction_result[22:0]};
            o_valid_w = 1;
        end
    end



    //sequential part
    always @(posedge i_clk or negedge i_rst_n) begin
        if (~i_rst_n) begin
            o_data_r <= 0;
            o_valid_r <= 0;
        end
        else begin
            o_data_r <= o_data_w;
            o_valid_r <= o_valid_w;
        end
    end
    

endmodule
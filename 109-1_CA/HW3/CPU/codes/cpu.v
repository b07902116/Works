module cpu #( // Do not modify interface
	parameter ADDR_W = 64,
	parameter INST_W = 32,
	parameter DATA_W = 64
)(
    input                   i_clk,
    input                   i_rst_n,
    input                   i_i_valid_inst, // from instruction memory
    input  [ INST_W-1 : 0 ] i_i_inst,       // from instruction memory
    input                   i_d_valid_data, // from data memory
    input  [ DATA_W-1 : 0 ] i_d_data,       // from data memory
    output                  o_i_valid_addr, // to instruction memory
    output [ ADDR_W-1 : 0 ] o_i_addr,       // to instruction memory
    output [ DATA_W-1 : 0 ] o_d_data,       // to data memory
    output [ ADDR_W-1 : 0 ] o_d_addr,       // to data memory
    output                  o_d_MemRead,    // to data memory
    output                  o_d_MemWrite,   // to data memory
    output                  o_finish
);

    //wires, registers, variables
    reg [INST_W - 1 : 0]        inst_reg;
    reg [DATA_W - 1 : 0]        mem_data_reg;
    reg [ADDR_W - 1 : 0]        o_i_addr_r, o_i_addr_w;
    reg [DATA_W - 1 : 0]        o_d_data_r, o_d_data_w;
    reg [ADDR_W - 1 : 0]        o_d_addr_r, o_d_addr_w;
    reg                         o_d_MemRead_r, o_d_MemRead_w, o_d_MemWrite_r, o_d_MemWrite_w, o_i_valid_addr_r, o_i_valid_addr_w, o_finish_r;
    wire                        Branch, MemRead, MemWrite, MemtoReg, ALUSrc, RegWrite, zero;
    wire    [1:0]               ALUOp;
    wire    [4:0]               Optype;
    wire    [DATA_W - 1 : 0]    WBdata, rs1_data, rs2_data, ALU_in2, ALUresult, imm;

    //continuous assign
    assign o_i_valid_addr = o_i_valid_addr_r;
    assign o_i_addr = o_i_addr_r;
    assign o_d_data = o_d_data_r;
    assign o_d_addr = o_d_addr_r;
    assign o_d_MemRead = o_d_MemRead_r;
    assign o_d_MemWrite = o_d_MemWrite_r;
    assign o_finish = o_finish_r;


    //combinational part
    initial begin
        o_i_addr_w = 0;
        mem_data_reg = 0;
        o_i_valid_addr_w = 1;
        o_d_MemRead_w = 0;
        o_d_MemWrite_w = 0;
    end

    //instruction memory
    always @(i_i_inst) begin
        if (i_i_valid_inst) begin
            inst_reg = i_i_inst;
            #90
            //wait enough time to finish read/write from/to data memory
            if (inst_reg == 32'hffffffff) begin
                o_finish_r = 1;
            end
            o_i_addr_w = (Branch & zero)? o_i_addr_w + {imm[63:1], 1'd0} : o_i_addr_w + 4;
            o_i_valid_addr_w = 1;
        end
    end

    //data memory
    always @(inst_reg) begin
        #5
        //wait control unit change MemRead and MemWrite to correct value
        if (MemWrite) begin
            o_d_data_w = rs2_data;
            o_d_addr_w = ALUresult;
            o_d_MemRead_w = 0;
            o_d_MemWrite_w = MemWrite;
        end
        else if (MemRead) begin
            o_d_addr_w = ALUresult;
            o_d_MemRead_w = MemRead;
            o_d_MemWrite_w = 0;
        end
        else begin
            o_d_MemRead_w = 0;
            o_d_MemWrite_w = 0;
        end
    end

    always @(i_d_valid_data) begin
        if (i_d_valid_data) begin
            mem_data_reg = i_d_data;
            o_d_MemRead_w = 0;
        end
    end

    //sequential part
    always @(posedge i_clk or negedge i_rst_n) begin
        if (~i_rst_n) begin
            o_i_valid_addr_r <= 1;
            o_i_addr_r <= 0;
            o_d_data_r <= 0;
            o_d_addr_r <= 0;
            o_d_MemRead_r <= 0;
            o_d_MemWrite_r <= 0;
        end
        else begin
            o_i_valid_addr_r <= o_i_valid_addr_w;
            o_i_addr_r <= o_i_addr_w;
            o_d_data_r <= o_d_data_w;
            o_d_addr_r <= o_d_addr_w;
            o_d_MemRead_r <= o_d_MemRead_w;
            o_d_MemWrite_r <= o_d_MemWrite_w;
        end
    end


    control #(
        .ADDR_W(ADDR_W),
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) crl (
        .opcode(inst_reg[6:0]),
        .Branch(Branch),
        .MemRead(MemRead),
        .MemWrite(MemWrite),
        .MemtoReg(MemtoReg),
        .ALUOp(ALUOp),
        .ALUSrc(ALUSrc),
        .RegWrite(RegWrite)
    );

    register #(
        .ADDR_W(ADDR_W),
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) rgf (
        .rs1_id(inst_reg[19:15]),
        .rs2_id(inst_reg[24:20]),
        .rd_id(inst_reg[11:7]),
        .RegWrite(RegWrite),
        .WBdata(WBdata),
        .rs1_data(rs1_data),
        .rs2_data(rs2_data)
    );

    ALUcontrol #(
        .ADDR_W(ADDR_W),
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) ALUcrl (
        .inst30(inst_reg[30]),
        .inst(inst_reg[14:12]),
        .ALUOp(ALUOp),
        .Optype(Optype)
    );

    ALU #(
        .ADDR_W(ADDR_W),
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) ALU (
        .data_a(rs1_data),
        .data_b(ALU_in2),
        .Optype(Optype),
        .result(ALUresult),
        .zero(zero)
    );

    ImmGen #(
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) immgen (
        .inst(inst_reg),
        .imm(imm)
    );

    mux #(
        .DATA_W(DATA_W)
    ) mux_ALU_source (
        .data_a(rs2_data),
        .data_b(imm),
        .control(ALUSrc),
        .result(ALU_in2)
    );

    mux #(
        .DATA_W(DATA_W)
    ) mux_WB_source (
        .data_a(ALUresult),
        .data_b(mem_data_reg),
        .control(MemtoReg),
        .result(WBdata)
    );
endmodule

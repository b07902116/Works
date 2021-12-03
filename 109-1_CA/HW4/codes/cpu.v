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
    output [ DATA_W-1 : 0 ] o_d_w_data,     // to data memory
    output [ ADDR_W-1 : 0 ] o_d_w_addr,     // to data memory
    output [ ADDR_W-1 : 0 ] o_d_r_addr,     // to data memory
    output                  o_d_MemRead,    // to data memory
    output                  o_d_MemWrite,   // to data memory
    output                  o_finish
);


    // wires and registers
    reg                         o_i_valid_addr_w, o_i_valid_addr_r;
    reg     [ADDR_W - 1 : 0]    o_i_addr_w, o_i_addr_r;
    reg     [DATA_W - 1 : 0]    o_d_w_data_r;
    reg     [ADDR_W - 1 : 0]    o_d_w_addr_r;
    reg     [ADDR_W - 1 : 0]    o_d_r_addr_r;
    reg                         o_d_MemRead_r;
    reg                         o_d_MemWrite_r;
    reg                         o_finish_r;

    // IFIDreg
    reg     [ADDR_W - 1 : 0]    IFID_PC_w, IFID_PC_r;
    reg     [INST_W - 1 : 0]    IFID_inst_w, IFID_inst_r;
    reg                         IFID_flush_w, IFID_flush_r;
    reg                         IFID_write_w, IFID_write_r;
    reg                         IF_PCWrite_w, IF_PCWrite_r;

    // IDEXreg
    reg     [DATA_W - 1 : 0]    IDEX_data1_r;
    reg     [DATA_W - 1 : 0]    IDEX_data2_r;
    reg     [DATA_W - 1 : 0]    IDEX_imm_r;
    reg                         IDEX_inst30_r;
    reg     [2:0]               IDEX_func3_r;
    reg     [4:0]               IDEX_rs1_r;
    reg     [4:0]               IDEX_rs2_r;
    reg     [4:0]               IDEX_rd_r;
    reg     [2:0]               IDEX_EXcontrol_r;
    reg     [1:0]               IDEX_MEMcontrol_r;
    reg     [1:0]               IDEX_WBcontrol_r;

    // EXMEMreg
    reg     [DATA_W - 1 : 0]    EXMEM_ALUresult_r;
    reg     [DATA_W - 1 : 0]    EXMEM_writedata_r;
    reg     [4:0]               EXMEM_rd_r;
    reg     [1:0]               EXMEM_MEMcontrol_r;
    reg     [1:0]               EXMEM_WBcontrol_r;

    // MEMWBreg
    reg     [DATA_W - 1 : 0]    MEMWB_memorydata_r;
    reg     [DATA_W - 1 : 0]    MEMWB_ALUresult_r;
    reg     [4:0]               MEMWB_rd_r;
    reg     [1:0]               MEMWB_WBcontrol_r;

    // others can be wire?
    reg     [DATA_W - 1 : 0]    MEM_memorydata;
    wire    [DATA_W - 1 : 0]    ID_data1, ID_data2;
    wire    [DATA_W - 1 : 0]    ID_imm;
    wire                        ID_DataEqual;
    wire                        ID_Branch;
    wire    [2:0]               ID_control_EX;
    wire    [1:0]               ID_control_MEM;
    wire    [1:0]               ID_control_WB;
    wire    [1:0]               ID_Control_Src;
    wire    [DATA_W - 1 : 0]    ID_control_signal;
    wire    [DATA_W - 1 : 0]    ID_PC_branch_pos;
    wire    [DATA_W - 1 : 0]    IF_PC_addfour_pos;
    wire    [DATA_W - 1 : 0]    IF_PC_next_pos;
    wire                        IF_PCWrite;
    wire                        IF_PCSrc;
    wire                        IFID_write;
    wire    [1:0]               EX_Forward1;
    wire    [1:0]               EX_Forward2;
    wire    [DATA_W - 1 : 0]    EX_ALUdata1;
    wire    [DATA_W - 1 : 0]    EX_ALUdata2;
    wire    [DATA_W - 1 : 0]    EX_ALUforwarddata;
    wire    [DATA_W - 1 : 0]    EX_ALUresult;
    wire    [4:0]               EX_Optype;
    wire    [DATA_W - 1 : 0]    WB_WBdata;
    integer                     count, load_use, same_inst;

    // continuous assign
    assign o_i_valid_addr = o_i_valid_addr_r;
    assign o_i_addr = o_i_addr_r;
    assign o_d_w_data = o_d_w_data_r;
    assign o_d_w_addr = o_d_w_addr_r;
    assign o_d_r_addr = o_d_r_addr_r;
    assign o_d_MemRead = o_d_MemRead_r;
    assign o_d_MemWrite = o_d_MemWrite_r;
    assign o_finish = o_finish_r;



    // sequential part
    always @(posedge i_clk or negedge i_rst_n) begin
        if (~i_rst_n) begin
            o_i_valid_addr_w <= 1;
            o_i_addr_w <= 0;
            count <= 0;
        end
        else begin
            o_i_valid_addr_r <= o_i_valid_addr_w;
            o_i_addr_r <= o_i_addr_w;
            IFID_write_r <= IFID_write;
            IF_PCWrite_r <= IF_PCWrite;
            if (count == 0) begin
                count <= 1;
            end
            else if (count == 1) begin
                count <= 2;
            end
            else if (count == 2) begin
                count <= 3;
            end
            else if (count == 3) begin
                count <= 4;
            end
            else begin
                count <= 5;
            end
        end
    end

    always @(IFID_write or IF_PCWrite or ID_Control_Src) begin
        if (IFID_write == 0) begin
            load_use = 1;
        end
    end

    // IFID
    always @(count) begin
        if (count == 4) begin
            if (load_use == 0) begin
                IFID_PC_r <= IFID_PC_w;
                IFID_inst_r <= IFID_inst_w;
                //IFID_write_r <= IFID_write;
                IFID_flush_r <= IFID_flush_w;
            end
            else begin
            end
        end
        else begin
            // do nothing
        end
    end

    // IDEX
    // @(every 4 cycles) update registers
    always @(count) begin
        if (count == 4) begin
            IDEX_data1_r <= ID_data1;
            IDEX_data2_r <= ID_data2;
            IDEX_imm_r <= ID_imm;
            IDEX_inst30_r <= IFID_inst_r[30];
            IDEX_func3_r <= IFID_inst_r[14:12];
            IDEX_rs1_r <= IFID_inst_r[19:15];
            IDEX_rs2_r <= IFID_inst_r[24:20];
            IDEX_rd_r <= IFID_inst_r[11:7];
            IDEX_EXcontrol_r <= ID_control_signal[6:4];
            IDEX_MEMcontrol_r <= ID_control_signal[3:2];
            IDEX_WBcontrol_r <= ID_control_signal[1:0];
        end
        else begin
            // do nothing
        end
    end

    // EXMEM
    always @(count) begin
        if (count == 4) begin
            EXMEM_ALUresult_r <= EX_ALUresult;
            EXMEM_writedata_r <= EX_ALUforwarddata;
            EXMEM_rd_r <= IDEX_rd_r;
            EXMEM_MEMcontrol_r <= IDEX_MEMcontrol_r;
            EXMEM_WBcontrol_r <= IDEX_WBcontrol_r;
        end
        else begin
            // do nothing
        end
    end

    // MEMWB
    always @(count) begin
        if (count == 4) begin
            MEMWB_memorydata_r <= MEM_memorydata;
            MEMWB_ALUresult_r <= EXMEM_ALUresult_r;
            MEMWB_rd_r <= EXMEM_rd_r;
            MEMWB_WBcontrol_r <= EXMEM_WBcontrol_r;
        end
    end

    // IF stage
    // instruction memory
    always @(i_i_valid_inst) begin
        if (i_i_valid_inst) begin
            if (IFID_inst_w == i_i_inst) begin
                same_inst = 1;
            end
            else begin
                same_inst = 0;
            end
            IFID_inst_w = i_i_inst;
            IFID_PC_w = o_i_addr_r;
            if (IFID_inst_w == 32'hffffffff) begin
                // let CPU finish instruction in pipeline
                #50
                count = 0;
                #50
                count = 0;
                #50
                count = 0;
                o_finish_r = 1;
            end
            else begin
                // do nothing
            end
        end
        else begin
            // do nothing
        end
    end

    always @(IF_PC_next_pos or IF_PCWrite_r or same_inst) begin
        if (load_use == 0) begin
            o_i_addr_w = IF_PC_next_pos;
        end
        else begin
            o_i_addr_w = IFID_PC_w;
        end
        o_i_valid_addr_w = 1;
        load_use = 0;
        count = 0;
    end

    always @(count) begin
        if (count == 1) begin
            o_i_valid_addr_w = 0;
        end
        else begin
            
        end
    end

    adder #(
        .DATA_W(DATA_W)
    ) adder_addfour (
        .data0(IFID_PC_w),
        .data1(64'd4),
        .result(IF_PC_addfour_pos)
    );
    mux #(
        .DATA_W(DATA_W)
    ) PC_source_mux (
        .control({1'b0, IF_PCSrc}),
        .data0(IF_PC_addfour_pos),
        .data1(ID_PC_branch_pos),
        .data2(64'b0),
        .data3(64'b0),
        .result(IF_PC_next_pos)
    );


    // ID stage
    register #(
        .ADDR_W(ADDR_W),
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) register_file (
        .rs1_id(IFID_inst_r[19:15]),
        .rs2_id(IFID_inst_r[24:20]),
        .rd_id(MEMWB_rd_r),
        .RegWrite(MEMWB_WBcontrol_r[0]),
        .WBdata(WB_WBdata),
        .rs1_data(ID_data1),
        .rs2_data(ID_data2),
        .DataEqual(ID_DataEqual)
    );
    control #(
        .ADDR_W(ADDR_W),
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) control (
        .opcode(IFID_inst_r[6:0]),
        .control_ID(ID_Branch),
        .control_EX(ID_control_EX),
        .control_MEM(ID_control_MEM),
        .control_WB(ID_control_WB)
    );
    mux #(
        .DATA_W(DATA_W)
    ) control_signal_mux (
        .control({1'b0, load_use[0]}),
        .data0({57'b0, ID_control_EX, ID_control_MEM, ID_control_WB}),
        .data1(64'b0),
        .data2(64'b0),
        .data3(64'b0),
        .result(ID_control_signal)
    );
    ImmGen #(
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) immgen (
        .inst(IFID_inst_r),
        .imm(ID_imm)
    );
    PCsource #(
        .DATA_W(DATA_W)
    ) PCsource (
        .BEQorBNE(IFID_inst_r[12]),
        .Branch(ID_Branch),
        .DataEqual(ID_DataEqual),
        .PCSrc(IF_PCSrc)
    );
    adder #(
        .DATA_W(DATA_W)
    ) adder_branch (
        .data0(IFID_PC_r),
        .data1({51'b0, ID_imm[12:1], 1'b0}),
        .result(ID_PC_branch_pos)
    );
    HazardDetectionUnit #(
        .ADDR_W(ADDR_W),
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) HDU (
        .IFID_rs1(IFID_inst_r[19:15]),
        .IFID_rs2(IFID_inst_r[24:20]),
        .IDEX_rd(IDEX_rd_r),
        .IDEX_MEMRead(IDEX_EXcontrol_r[1]),
        .PCWrite(IF_PCWrite),
        .IFIDWrite(IFID_write),
        .ControlSrc(ID_Control_Src)
    );

    // EX stage
    mux #(
        .DATA_W(DATA_W)
    ) ALUrs1_mux (
        .control(EX_Forward1),
        .data0(IDEX_data1_r),
        .data1(WB_WBdata),
        .data2(EXMEM_ALUresult_r),
        .data3(64'b0),
        .result(EX_ALUdata1)
    );
    mux #(
        .DATA_W(DATA_W)
    ) ALUrs2_mux (
        .control(EX_Forward2),
        .data0(IDEX_data2_r),
        .data1(WB_WBdata),
        .data2(EXMEM_ALUresult_r),
        .data3(64'b0),
        .result(EX_ALUforwarddata)
    );
    mux #(
        .DATA_W(DATA_W)
    ) ALUrs2_src_mux (
        .control({1'b0, IDEX_EXcontrol_r[0]}),
        .data0(EX_ALUforwarddata),
        .data1(IDEX_imm_r),
        .data2(64'b0),
        .data3(64'b0),
        .result(EX_ALUdata2)
    );
    ALUcontrol #(
        .ADDR_W(ADDR_W),
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) ALUcontrol (
        .inst30(IDEX_inst30_r),
        .inst(IDEX_func3_r),
        .ALUOp(IDEX_EXcontrol_r[2:1]),
        .Optype(EX_Optype)
    );

    ALU #(
        .DATA_W(DATA_W)
    ) ALU (
        .data_a(EX_ALUdata1),
        .data_b(EX_ALUdata2),
        .Optype(EX_Optype),
        .result(EX_ALUresult)
    );
    ForwardingUnit #(
        .ADDR_W(ADDR_W),
        .INST_W(INST_W),
        .DATA_W(DATA_W)
    ) Forward (
        .IDEX_rs1(IDEX_rs1_r),
        .IDEX_rs2(IDEX_rs2_r),
        .EXMEM_rd(EXMEM_rd_r),
        .EXMEM_RegWrite(EXMEM_WBcontrol_r[0]),
        .MEMWB_rd(MEMWB_rd_r),
        .MEMWB_RegWrite(MEMWB_WBcontrol_r[0]),
        .Forward1(EX_Forward1),
        .Forward2(EX_Forward2)
    );

    // MEM stage
    always @(*) begin
        if (EXMEM_MEMcontrol_r[1] == 1) begin
            // MemRead == 1
            o_d_w_data_r <= 0;
            o_d_w_addr_r <= 0;
            o_d_r_addr_r <= EXMEM_ALUresult_r;
            o_d_MemRead_r <= 1;
            o_d_MemWrite_r <= 0;
        end
        else if (EXMEM_MEMcontrol_r[0] == 1) begin
            // MemWrite == 1
            o_d_w_data_r <= EXMEM_writedata_r;
            o_d_w_addr_r <= EXMEM_ALUresult_r;
            o_d_r_addr_r <= 0;
            o_d_MemRead_r <= 0;
            o_d_MemWrite_r <= 1;
        end
        else begin
           // do nothing 
        end
    end

    always @(i_d_valid_data) begin
        if (i_d_valid_data) begin
            MEM_memorydata <= i_d_data;
        end
        else begin
            // do nothing
        end
    end

    // WB stage
    mux #(
        .DATA_W(DATA_W)
    ) WB_mux (
        .control({1'b0, MEMWB_WBcontrol_r[1]}),
        .data0(MEMWB_ALUresult_r),
        .data1(MEMWB_memorydata_r),
        .data2(64'b0),
        .data3(64'b0),
        .result(WB_WBdata)
    );


endmodule

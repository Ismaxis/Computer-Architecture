module mem (
    input clk;

    inout [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] address,
    inout [8-1:0] data [DATA_SIZE-1:0],
    inout [2-1:0] command,
);
    parameter MEM_ADDR_SIZE = 10 + 9;          // log2(MEM_SIZE)
    parameter MEM_SIZE = 1 << MEM_ADDR_SIZE;   // 512KB = 2^9 * 2^10 = 2^19

    localparam  C2_NOP      = 3'd0,
                C2_RESPONSE = 3'd1,
                C2_READ     = 3'd2,
                C2_WRITE    = 3'd3;


    reg [8-1:0] storage [MEM_SIZE-1:0];         // 2^19 8-bit word
    reg [8-1:0] data_buff [DATA_SIZE-1:0];      // single bus
    reg [2-1:0] command_buff;

    always @(posedge clk) begin
        command_buff = 0;
        if (command == C2_READ) begin
            // READ
            for (int i=0; i<CACHE_LINE_SIZE; i=i+1) begin
                data_buff[i] = storage[address + (i << CACHE_OFFSET_SIZE)];
            end
            command_buff=1;
        end else if (command == C2_WRITE) begin
            // WRITE
            for (int i=0; i<CACHE_LINE_SIZE; i=i+1) begin
                storage[address + (i << CACHE_OFFSET_SIZE)] = data[i];
            end
            command_buf = 1;
        end
    end
    assign command = command_buff;
    assign data = data_buff;
endmodule
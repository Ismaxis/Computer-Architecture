module mem (
    input clk;

    inout [MEM_ADDR_SIZE-1:0] address,
    inout [7:0] data [CACHE_LINE_SIZE-1:0],
    inout [1:0] command,
);
    parameter MEM_ADDR_SIZE = 10 + 9;          // log2(MEM_SIZE)
    parameter MEM_SIZE = 1 << MEM_ADDR_SIZE;   // 512KB = 2^9 * 2^10 = 2^19

    reg [7:0] storage [MEM_SIZE-1:0];          // 2^19 8-bit word
    reg [7:0] data_buff [CACHE_LINE_SIZE-1:0]; // single line
    reg [1:0] command_buff;

    always @(posedge clk) begin
        command_buff = 0;
        if (command == 2) begin
            // READ
            for (int i=0; i<CACHE_LINE_SIZE; i=i+1) begin
                data_buff[i] = storage[address + (i << CACHE_OFFSET_SIZE)];
            end
            command_buff=1;
        end else if (command == 3) begin
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
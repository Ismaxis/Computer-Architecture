module mem #(
    parameter MEM_ADDR_SIZE = 10 + 9, 
    parameter BUS_SIZE = 16 ,
    parameter CACHE_OFFSET_SIZE = 4,
    parameter CACHE_LINE_SIZE = 16
    ) (
    input clk,
    input reset,
    input [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] address,
    inout [BUS_SIZE-1:0] data,
    input [2-1:0] command
);

    // 512KB = 2^9 * 2^10 = 2^19 = 2^15 lines * 2^4 bits in each line (16 8-bit words)
    parameter MEM_SIZE = 1 << (MEM_ADDR_SIZE-CACHE_OFFSET_SIZE); // 2^15 cache lines

    localparam  C2_NOP      = 3'd0,
                C2_RESPONSE = 3'd1,
                C2_READ     = 3'd2,
                C2_WRITE    = 3'd3;

    reg [CACHE_LINE_SIZE*8-1:0] storage [MEM_SIZE-1:0]; // 2^15 16-byte lines
    reg [BUS_SIZE-1:0] data_buff; // single bus
    reg [2-1:0] command_buff;

    reg [CACHE_OFFSET_SIZE-1:0] rwPosition; // ptr to next 2 bytes to read/write

    integer SEED = 225526;



    always @(posedge clk or posedge reset) begin
        if (reset) begin
            for (int i = 0; i < 99; i=i+1) begin
                storage[i] = $random(SEED)>>16;
            end
            // $display("filled");

            data_buff = 'z;
            command_buff = 0;
            rwPosition = 0;
        end else begin
            command_buff = 0;
            if (command == C2_READ) begin
                // READ
                for (int i=0; i<CACHE_LINE_SIZE; i=i+1) begin
                    data_buff[i] = storage[address][rwPosition*8 + i];
                end
                rwPosition += 2;
            end else if (command == C2_WRITE) begin
                // WRITE
                // $display("before: %b", storage[address]);
                for (int i=0; i<CACHE_LINE_SIZE; i=i+1) begin
                    storage[address][rwPosition*8 + i] = data[i];
                end
                // $display("after : %b", storage[address]);
                rwPosition += 2;
            end else begin
                rwPosition = 0;
                data_buff = 'z;
            end
        end
    end

    assign data = data_buff;

endmodule
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

    // reg [CACHE_OFFSET_SIZE-1:0] rPosition; // ptr to next 2 bytes to read/write

    integer SEED = 225526;

    task delay;
        begin
            @(negedge clk);
        end
    endtask	

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            for (int i = 0; i < 99; i=i+1) begin
                storage[i] = $random(SEED)>>16;
            end
            data_buff = 'z;
        end else begin
            if (command == C2_READ) begin
                // READ
                for (int i=0; i<CACHE_LINE_SIZE/2; i=i+1) begin
                    data_buff = storage[address][BUS_SIZE*i +: BUS_SIZE];
                    delay;
                end
                delay;
            end else if (command == C2_WRITE) begin
                data_buff = 'z;
                delay;
                // WRITE
                for (int i=0; i<CACHE_LINE_SIZE/2; i=i+1) begin
                    storage[address][BUS_SIZE*i +: BUS_SIZE] = data;
                    delay;
                end
            end else begin
                data_buff = 'z;
            end
        end
    end

    assign data = data_buff;

endmodule
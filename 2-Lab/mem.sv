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
    parameter MEM_SIZE = 1 << (MEM_ADDR_SIZE-CACHE_OFFSET_SIZE);   // 512KB = 2^9 * 2^10 = 2^19

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
            for (int i = 0; i < 8; i=i+1) begin
                storage[i] = $random(SEED)>>16;
                $display("%d %b", i, storage[i]);
            end

            $display("filled");

            data_buff = 0;
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
                // WRIjTE
                for (int i=0; i<CACHE_LINE_SIZE; i=i+1) begin
                    storage[address + (i << CACHE_OFFSET_SIZE)] = data[i];
                end
                command_buff = 1;
            end else begin
                rwPosition = 0;
            end
        end
    end
    // assign command = command_buff;
    assign data = data_buff;
endmodule
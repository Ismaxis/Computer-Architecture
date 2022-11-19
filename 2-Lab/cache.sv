module cache (
    input clk;

    inout [MEM_ADDR_SIZE-1:0] cpu_address,
    inout [CACHE_LINE_SIZE*8-1:0] cpu_data,
    inout [2:0] cpu_command,
);
    parameter CACHE_WAY = 2;
    parameter CACHE_LINE_SIZE = 16;
    parameter CACHE_LINE_COUNT = 64;
    parameter CACHE_SIZE = CACHE_LINE_COUNT * CACHE_LINE_SIZE;

    parameter CACHE_SET_SIZE = 5;    // log2(CACHE_LINE_COUNT/CACHE_WAY)
    parameter CACHE_OFFSET_SIZE = 4; // log2(CACHE_LINE_SIZE)
    parameter CACHE_TAG_SIZE = 10;   // CACHE_LINE_SIZE - CACHE_SET_SIZE - CACHE_OFFSET_SIZE

    
    reg [MEM_ADDR_SIZE-1:0] mem_address,
    reg [CACHE_LINE_SIZE*8-1:0] mem_data,
    reg [1:0] mem_command,

    // STORAGE
    reg valid_array [CACHE_LINE_COUNT/CACHE_SET_SIZE-1:0][CACHE_SET_SIZE-1:0];
    reg dirty_array [CACHE_LINE_COUNT/CACHE_SET_SIZE-1:0][CACHE_SET_SIZE-1:0];
    reg [CACHE_TAG_SIZE-1:0] tag_array [CACHE_LINE_COUNT/CACHE_SET_SIZE-1:0][CACHE_SET_SIZE-1:0];
    reg [CACHE_LINE_SIZE*8-1:0] data_array [CACHE_LINE_COUNT/CACHE_SET_SIZE-1:0][CACHE_SET_SIZE-1:0]; // stores lines

    reg [CACHE_LINE_SIZE-1:0] cpu_data_buff; // single line
    reg [2:0] cpu_command_buff;
    reg [CACHE_LINE_SIZE-1:0] mem_data_buff; // single line
    reg [2:0] mem_command_buff;

    mem mem(
        .clk(clk),
        .address(mem_address),
        .data(mem_data),
        .command(mem_command),
    );

    // CPU
    always @(posedge clk) begin
        mem_command_buff = 0;
        // C1_READ8
        if (cpu_command == 3) begin
            reg [CACHE_SET_SIZE-1:0] set = cpu_address >> CACHE_OFFSET_SIZE;
            reg [CACHE_TAG_SIZE-1:0] tag = cpu_address >> (CACHE_SET_SIZE + CACHE_OFFSET_SIZE);
            reg found = 0;
            for (int i=0; i<CACHE_SET_SIZE; i=i+1) begin
                if (tag_array[set][i] == tag) begin
                    // Retrurn data
                    cpu_data_buff = data_array[set][i];
                    cpu_command_buff = 7;
                end
            end

            if (found == 0) begin 
                // Request from mem
                // Set valid 
                valid_array[set][i] = 1;
            end

        end
    end
    assign mem_command = mem_command_buff;
    assign mem_data = mem_data_buff;

    // MEM
    always @(posedge clk) begin
        cpu_command_buff = 0;
        // C2_READ
        if (mem_command == 1) begin
            reg [CACHE_SET_SIZE-1:0] set = mem_address >> CACHE_OFFSET_SIZE;
            reg [CACHE_TAG_SIZE-1:0] tag = mem_address >> (CACHE_SET_SIZE + CACHE_OFFSET_SIZE);
            reg found = 0;
            for (int i=0; i<CACHE_SET_SIZE; i=i+1) begin
                if (valid_array[set][i] == 0) begin
                    found = 1;
                    valid_array[set][i] = 1;
                    dirty_array[set][i] = 0;
                    tag_array[set][i] = tag;
                    data_array[set][i] = mem_data;
                end
            end
            if (found == 0) begin
                // search for not dirty
                // if all dirty write one and replace with new data
            end
            cpu_data_buff = mem_data;

            cpu_command_buff = 7;
        end
    end
    assign cpu_command = cpu_command_buff;
    assign cpu_data = cpu_data_buff;

    

endmodule
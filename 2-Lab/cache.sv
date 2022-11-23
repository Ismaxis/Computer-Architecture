module cache (
    input clk;

    inout [MEM_ADDR_SIZE-1:0] cpu_address,
    inout [CACHE_LINE_SIZE*8-1:0] cpu_data,
    inout [2:0] cpu_command,

    inout [MEM_ADDR_SIZE-1:0] mem_address,
    inout [CACHE_LINE_SIZE*8-1:0] mem_data,
    inout [1:0] mem_command,

);
    parameter CACHE_WAY         = 2;
    parameter CACHE_LINE_SIZE   = 16;
    parameter CACHE_LINE_COUNT  = 64;
    parameter CACHE_SIZE        = CACHE_LINE_COUNT * CACHE_LINE_SIZE;

    parameter CACHE_SET_SIZE    = 5;    // log2(CACHE_LINE_COUNT/CACHE_WAY)
    parameter CACHE_OFFSET_SIZE = 4;    // log2(CACHE_LINE_SIZE)
    parameter CACHE_TAG_SIZE    = 10;   // CACHE_LINE_SIZE - CACHE_SET_SIZE - CACHE_OFFSET_SIZE

    localparam  C1_NOP      = 3'd0;
                C1_READ8    = 3'd1;
                C1_READ16   = 3'd2;
                C1_READ32   = 3'd3;
                C1_INV_LINE = 3'd4;
                C1_WRITE8   = 3'd5;
                C1_WRITE16  = 3'd6;
                C1_WRITE32  = 3'd7;

    localparam	IDLE		= 3'd0,
                READ        = 3'd1,
                WRITE       = 3'd2,
                READMM      = 3'd3,
                WAITFORMM   = 3'd4,
                UPDATEMM    = 3'd5,
                UPDATECACHE = 3'd6;

    reg [2:0] state;

    // STORAGE
    reg valid_array [CACHE_LINE_COUNT/CACHE_SET_SIZE-1:0][CACHE_SET_SIZE-1:0];
    reg dirty_array [CACHE_LINE_COUNT/CACHE_SET_SIZE-1:0][CACHE_SET_SIZE-1:0];
    reg [CACHE_TAG_SIZE-1:0] tag_array [CACHE_LINE_COUNT/CACHE_SET_SIZE-1:0][CACHE_SET_SIZE-1:0];
    reg [CACHE_LINE_SIZE*8-1:0] data_array [CACHE_LINE_COUNT/CACHE_SET_SIZE-1:0][CACHE_SET_SIZE-1:0]; // stores lines

    reg [CACHE_LINE_SIZE-1:0] cpu_data_buff; // single line
    reg [2:0] cpu_command_buff;
    reg [CACHE_LINE_SIZE-1:0] mem_data_buff; // single line
    reg [2:0] mem_command_buff;


    always @(posedge clk) begin
        case(state)
            IDLE: begin
                case (cpu_command)
                    C1_NOP: begin
                        
                    end
                    C1_READ8: begin
                        
                    end
                    C1_READ16: begin
                        
                    end
                    C1_READ32: begin
                        
                    end
                    C1_INV_LINE: begin
                        
                    end
                    C1_WRITE8: begin
                        
                    end
                    C1_WRITE16: begin
                        
                    end
                    C1_WRITE32: begin
                        
                    end
                endcase
            end
            READ: begin
                
            end
            WRITE: begin
                
            end
            READMM: begin
                
            end
            WAITFORMM: begin
                
            end
            UPDATEMM: begin
                
            end
            UPDATECACHE: begin
                
            end

        endcase

    end

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
module cache#(
    parameter BUS_SIZE = 16 ,
    parameter MEM_ADDR_SIZE = 10 + 9,   // log2(MEM_SIZE)
    parameter CACHE_OFFSET_SIZE = 4,    // log2(CACHE_LINE_SIZE)
    parameter CACHE_LINE_SIZE   = 16
    ) (
    input clk,
    input reset,
    input [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] cpu_address,
    inout [BUS_SIZE-1:0] cpu_data,
    inout [3-1:0] cpu_command,

    output [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] mem_address,
    inout [BUS_SIZE-1:0] mem_data,
    inout [2-1:0] mem_command
);

    parameter CACHE_WAY         = 2;

    parameter CACHE_LINE_COUNT  = 64;
    parameter CACHE_SETS_COUNT  = CACHE_LINE_COUNT/CACHE_WAY; 
    parameter CACHE_SIZE        = CACHE_LINE_COUNT * CACHE_LINE_SIZE;
   
    parameter CACHE_SET_SIZE    = 5;    // log2(CACHE_SETS_COUNT)
    parameter CACHE_TAG_SIZE    = 10;   // CACHE_LINE_SIZE - CACHE_SET_SIZE - CACHE_OFFSET_SIZE
    localparam  C1_NOP          = 3'd0,
                C1_READ8        = 3'd1,
                C1_READ16       = 3'd2,
                C1_READ32       = 3'd3,
                C1_INV_LINE     = 3'd4,
                C1_WRITE8       = 3'd5,
                C1_WRITE16      = 3'd6,
                C1_WRITE32_RESP = 3'd7;

    localparam  C2_NOP          = 2'd0,
                C2_RESPONSE     = 2'd1,
                C2_READ         = 2'd2,
                C2_WRITE        = 2'd3;

    // localparam  IDLE        = 3'd0,
    //             READ        = 3'd1,
    //             WRITE       = 3'd2,
    //             READMM      = 3'd3,
    //             WAITFORMM   = 3'd4,
    //             UPDATEMM    = 3'd5,
    //             UPDATECACHE = 3'd6;

    // reg [3-1:0] state;

    // STORAGE
    reg valid_array [CACHE_SETS_COUNT-1:0][CACHE_WAY-1:0];
    reg dirty_array [CACHE_SETS_COUNT-1:0][CACHE_WAY-1:0];
    reg last_used_array [CACHE_SETS_COUNT-1:0]; // lRU
    reg [CACHE_TAG_SIZE-1:0] tag_array [CACHE_SETS_COUNT-1:0][CACHE_WAY-1:0];
    reg [CACHE_LINE_SIZE*8-1:0] data_array [CACHE_SETS_COUNT-1:0][CACHE_WAY-1:0]; // stores lines

    reg [MEM_ADDR_SIZE-1:0] cpu_address_buff;
    reg [CACHE_TAG_SIZE-1:0] cpu_tag_buff;
    reg [CACHE_SET_SIZE-1:0] cpu_set_buff;
    reg [CACHE_OFFSET_SIZE-1:0] cpu_offset_buff;
    reg [BUS_SIZE-1:0] cpu_data_bus_buff; // single bus
    reg [3-1:0] cpu_command_buff;
    
    reg [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] mem_address_buff;
    reg [CACHE_LINE_SIZE*8-1:0] mem_line_buff; // single line
    // reg [BUS_SIZE-1:0] mem_data_buff;
    reg [2-1:0] mem_command_buff;
    

    task delay;
        begin
            @(negedge clk);
        end
    endtask	

    initial begin
        mem_line_buff = 0;
        cpu_data_bus_buff = 0;
        cpu_command_buff = 'z;
    end

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            for (int i=0; i<CACHE_SETS_COUNT; i=i+1) begin
                valid_array[i][0] = 0;
                dirty_array[i][0] = 0;
                tag_array[i][0]   = 'z;
                data_array[i][0]  = 'z;
                valid_array[i][1] = 0;
                dirty_array[i][1] = 0;
                tag_array[i][1]   = 'z;
                data_array[i][1]  = 'z;
            end
        end 
        if (cpu_command == C1_READ8) begin
            $display("C1_READ8");

            mem_address_buff = cpu_address;
            cpu_tag_buff = cpu_address[CACHE_TAG_SIZE+CACHE_SET_SIZE-1:CACHE_SET_SIZE];
            cpu_set_buff = cpu_address[CACHE_SET_SIZE-1:0];
            while (mem_address_buff == cpu_address) begin 
                delay;
            end
            cpu_offset_buff = cpu_address[3:0];
            if (tag_array[cpu_set_buff][0] == cpu_tag_buff) begin
                $display("found0");
                if (valid_array[cpu_set_buff][0] == 1) begin
                    cpu_data_bus_buff = data_array[cpu_set_buff][0][cpu_offset_buff*8 +: 8];
                end
            end else if (tag_array[cpu_set_buff][1] == cpu_tag_buff) begin
                $display("found1");
                if (valid_array[cpu_set_buff][1] == 1) begin
                    cpu_data_bus_buff = data_array[cpu_set_buff][1][cpu_offset_buff*8 +: 8];
                end
            end else begin // GO TO MM
                $display("GO TO MM");
                // set
                mem_command_buff = C2_READ;
                delay;
                // read
                for (int i=0; i<CACHE_LINE_SIZE/2; i=i+1) begin
                    mem_line_buff[BUS_SIZE*i +: BUS_SIZE] = mem_data;
                    delay;
                end
                mem_command_buff = C2_NOP;
                // write in cache storage
                if (valid_array[cpu_set_buff][0] == 0) begin
                    $display("store to 0");
                    data_array[cpu_set_buff][0] = mem_line_buff;
                    tag_array[cpu_set_buff][0] = cpu_tag_buff;
                    valid_array[cpu_set_buff][0] = 1;
                    dirty_array[cpu_set_buff][0] = 0;
                    last_used_array[cpu_set_buff] = 1; // opposite 
                end else if (valid_array[cpu_set_buff][1] == 0) begin
                    $display("store to 1");
                    data_array[cpu_set_buff][1] = mem_line_buff;
                    tag_array[cpu_set_buff][1] = cpu_tag_buff;
                    valid_array[cpu_set_buff][1] = 1;
                    dirty_array[cpu_set_buff][1] = 0;
                    last_used_array[cpu_set_buff] = 0; // opposite 
                end else begin
                    if (dirty_array[cpu_set_buff][last_used_array[cpu_set_buff]] == 1) begin
                    $display("write to mm");
                        // write to MM
                    end
                end
                cpu_data_bus_buff = mem_line_buff[cpu_offset_buff*8 +: 8];
            end

            cpu_command_buff = C1_WRITE32_RESP;
            delay;
            cpu_command_buff = 'z;

        end else if (cpu_command == C1_WRITE8) begin
            
        end
    end
    assign mem_command = mem_command_buff;
    assign mem_address = mem_address_buff;
    assign cpu_data = cpu_data_bus_buff;
    assign cpu_command = cpu_command_buff;

    // always @(posedge clk) begin
    //     case(state)
    //         IDLE: begin
    //             case (cpu_command)
    //                 C1_NOP: begin
                        
    //                 end
    //                 C1_READ8: begin
    //                     state = READ;
    //                     mem_address_buff = cpu_address;
    //                     bitOfOp = 8;
    //                 end
    //                 C1_READ16: begin
                        
    //                     bitOfOp = 16;
    //                 end
    //                 C1_READ32: begin
                        
    //                     bitOfOp = 32;
    //                 end
    //                 C1_INV_LINE: begin
                        
    //                 end
    //                 C1_WRITE8: begin
                        
    //                     bitOfOp = 8;
    //                 end
    //                 C1_WRITE16: begin
                        
    //                     bitOfOp = 16;
    //                 end
    //                 C1_WRITE32: begin
                        
    //                     bitOfOp = 32;
    //                 end
    //             endcase
    //         end
    //         READ: begin
    //             reg [CACHE_SET_SIZE-1:0] set = cpu_address >> CACHE_OFFSET_SIZE;
    //             reg [CACHE_TAG_SIZE-1:0] tag = cpu_address >> (CACHE_SET_SIZE + CACHE_OFFSET_SIZE);

    //             if (valid_array[set][0] == 1 && tag_array[set][0] == tag) begin

    //             end else if (valid_array[set][1] == 1 && tag_array[set][1] == tag) begin
                    
    //             end else begin
    //                 // miss
    //                 if (dirty_array[set][last_used_array[set]] == 1) begin
    //                     // UPDATEMM
    //                     // dirty
    //                     // flag
    //                 end else begin
    //                     // READMM
    //                 end
    //             end

    //             assign hit_w0 = vw0 & (tagdata == rtag0[10:0]);
    //             assign hit_w1 = vw1 & (tagdata == rtag1[10:0]);
    //             assign hit = hit_w0 | hit_w1;
    //         end
    //         WRITE: begin
                
    //         end
    //         READMM: begin
                
    //         end
    //         WAITFORMM: begin
                
    //         end
    //         UPDATEMM: begin
                
    //         end
    //         UPDATECACHE: begin
                
    //         end
    //     endcase
    // end

    // assign mem_address = mem_address_buff;

    // // CPU
    // always @(posedge clk) begin
    //     mem_command_buff = 0;
    //     // C1_READ8
    //     if (cpu_command == 3) begin
    //         reg [CACHE_SET_SIZE-1:0] set = cpu_address >> CACHE_OFFSET_SIZE;
    //         reg [CACHE_TAG_SIZE-1:0] tag = cpu_address >> (CACHE_SET_SIZE + CACHE_OFFSET_SIZE);
    //         reg found = 0;
    //         for (int i=0; i<CACHE_SET_SIZE; i=i+1) begin
    //             if (tag_array[set][i] == tag) begin
    //                 // Retrurn data
    //                 cpu_data_buff = data_array[set][i];
    //                 cpu_command_buff = 7;
    //             end
    //         end

    //         if (found == 0) begin 
    //             // Request from mem
    //             // Set valid 
    //             valid_array[set][i] = 1;
    //         end

    //     end
    // end
    // assign mem_command = mem_command_buff;
    // assign mem_data = mem_line_buff;

    // // MEM
    // always @(posedge clk) begin
    //     cpu_command_buff = 0;
    //     // C2_READ
    //     if (mem_command == 1) begin
    //         reg [CACHE_SET_SIZE-1:0] set = mem_address >> CACHE_OFFSET_SIZE;
    //         reg [CACHE_TAG_SIZE-1:0] tag = mem_address >> (CACHE_SET_SIZE + CACHE_OFFSET_SIZE);
    //         reg found = 0;
    //         for (int i=0; i<CACHE_SET_SIZE; i=i+1) begin
    //             if (valid_array[set][i] == 0) begin
    //                 found = 1;
    //                 valid_array[set][i] = 1;
    //                 dirty_array[set][i] = 0;
    //                 tag_array[set][i] = tag;
    //                 data_array[set][i] = mem_data;
    //             end
    //         end
    //         if (found == 0) begin
    //             // search for not dirty
    //             // if all dirty write one and replace with new data
    //         end
    //         cpu_data_buff = mem_data;

    //         cpu_command_buff = 7;
    //     end
    // end
    // assign cpu_command = cpu_command_buff;
    // assign cpu_data = cpu_data_buff;

    

endmodule
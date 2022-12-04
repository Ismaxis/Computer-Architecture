module cache#(
    parameter BUS_SIZE = 16 ,
    parameter MEM_ADDR_SIZE = 10 + 9,
    parameter CACHE_OFFSET_SIZE = 4,
    parameter CACHE_LINE_SIZE   = 16
    ) (
    input clk,
    input reset,
    input dump,
    input [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] cpu_address,
    inout [BUS_SIZE-1:0] cpu_data,
    inout [3-1:0] cpu_command,

    output [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] mem_address,
    inout [BUS_SIZE-1:0] mem_data,
    inout [2-1:0] mem_command
);

    parameter CACHE_WAY         = 2;
    parameter CACHE_LINE_COUNT  = 64;

    parameter CACHE_SET_SIZE    = $clog2(CACHE_SETS_COUNT);
    parameter CACHE_TAG_SIZE    = 10; //CACHE_LINE_SIZE - CACHE_SET_SIZE - CACHE_OFFSET_SIZE;
    parameter CACHE_SIZE        = CACHE_LINE_COUNT * CACHE_LINE_SIZE;
    parameter CACHE_SETS_COUNT  = CACHE_LINE_COUNT/CACHE_WAY; 

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

    // STORAGE
    reg valid_array [CACHE_SETS_COUNT-1:0][CACHE_WAY-1:0];
    reg dirty_array [CACHE_SETS_COUNT-1:0][CACHE_WAY-1:0];
    reg last_used_array [CACHE_SETS_COUNT-1:0]; // lRU
    reg [CACHE_TAG_SIZE-1:0] tag_array [CACHE_SETS_COUNT-1:0][CACHE_WAY-1:0];
    reg [CACHE_LINE_SIZE*8-1:0] data_array [CACHE_SETS_COUNT-1:0][CACHE_WAY-1:0]; // stores lines

    reg [CACHE_TAG_SIZE-1:0] cpu_tag_buff;
    reg [CACHE_SET_SIZE-1:0] cpu_set_buff;
    reg [CACHE_OFFSET_SIZE-1:0] cpu_offset_buff;
    reg [BUS_SIZE-1:0] cpu_data_bus_buff; // single bus
    reg [BUS_SIZE*2-1:0] cpu_data_to_write;
    reg [3-1:0] cpu_command_buff;
    
    reg [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] mem_address_buff;
    reg [CACHE_LINE_SIZE*8-1:0] mem_line_buff; // single line
    reg [BUS_SIZE-1:0] mem_data_buff;
    reg [2-1:0] mem_command_buff;

    // Analytic
    real req;
    real hit;

    // Tasks
    task delay;
        begin
            @(negedge clk);
        end
    endtask	

    task wait_for_resp;
        while (mem_command !== C2_RESPONSE) begin 
            delay;
        end
    endtask

    task evict_if_dirty;
        if (dirty_array[cpu_set_buff][index_in_set] == 1) begin
            mem_address_buff = {tag_array[cpu_set_buff][index_in_set], cpu_set_buff};
            write_to_MM;
        end
    endtask

    task replace_from_MM;
        // command
        mem_command_buff = C2_READ;
        delay;
        mem_command_buff = 'z;
        wait_for_resp;

        // data
        for (int i=0; i<CACHE_LINE_SIZE/2; i=i+1) begin
            mem_line_buff[BUS_SIZE*i +: BUS_SIZE] = mem_data;
            delay;
        end

        // restore
        mem_command_buff = C2_NOP;
        
        if (valid_array[cpu_set_buff][0] == 0) begin
            index_in_set = 0;
            store;
        end else if (valid_array[cpu_set_buff][1] == 0) begin
            index_in_set = 1;
            store;
        end else begin
            // evict if no empty space 
            index_in_set = last_used_array[cpu_set_buff];
            evict_if_dirty;
        
            index_in_set = last_used_array[cpu_set_buff];
            store;
        end
    endtask 

    task write_to_MM;
        // command
        mem_command_buff = C2_WRITE;
        delay;
        mem_command_buff = 'z;
        mem_data_buff = data_array[cpu_set_buff][index_in_set][0 +: BUS_SIZE];
        wait_for_resp;

        // data
        for (int i=1; i<CACHE_LINE_SIZE/2; i=i+1) begin
            mem_data_buff = data_array[cpu_set_buff][index_in_set][BUS_SIZE*i +: BUS_SIZE];
            delay;
        end
        // restore
        mem_command_buff = C2_NOP;
        mem_data_buff = 'z;
    endtask

    reg index_in_set;
    task store;
        data_array[cpu_set_buff][index_in_set] = mem_line_buff;
        tag_array[cpu_set_buff][index_in_set] = cpu_tag_buff;
        valid_array[cpu_set_buff][index_in_set] = 1;
        dirty_array[cpu_set_buff][index_in_set] = 0;
        last_used_array[cpu_set_buff] = ~index_in_set; 
    endtask

    task read_cpu_address;
        mem_address_buff = cpu_address;
        cpu_tag_buff = cpu_address[CACHE_TAG_SIZE+CACHE_SET_SIZE-1:CACHE_SET_SIZE];
        cpu_set_buff = cpu_address[CACHE_SET_SIZE-1:0];

        delay;
        
        cpu_offset_buff = cpu_address[3:0];
    endtask

    reg [3-1:0] cur_cpu_command;
    task read_from_storage;
        if (cur_cpu_command == C1_READ8) begin
            cpu_data_bus_buff = data_array[cpu_set_buff][index_in_set][cpu_offset_buff*8 +: 8];
        end else if (cur_cpu_command == C1_READ16 || cur_cpu_command == C1_READ32) begin
            cpu_data_bus_buff = data_array[cpu_set_buff][index_in_set][cpu_offset_buff*8 +: 16];
        end
        last_used_array[cpu_set_buff] = ~index_in_set;
    endtask 

    task write_to_storage;
        if (cur_cpu_command == C1_WRITE8) begin
            data_array[cpu_set_buff][index_in_set][cpu_offset_buff*8 +: 8] = cpu_data_to_write;
        end else if (cur_cpu_command == C1_WRITE16) begin
            data_array[cpu_set_buff][index_in_set][cpu_offset_buff*8 +: 16] = cpu_data_to_write;
        end else if (cur_cpu_command == C1_WRITE32_RESP) begin
            data_array[cpu_set_buff][index_in_set][cpu_offset_buff*8 +: 32] = cpu_data_to_write;
        end
        dirty_array[cpu_set_buff][index_in_set] = 1;
        last_used_array[cpu_set_buff] = ~index_in_set;
    endtask

    int f;
    task dump_to_console;
        f = $fopen("cache.dump", "w");
        if (f) begin
            $display("req: %d\nhits: %d\nrate: %f", req, hit, hit/req);
            $fdisplay(f,"$$$$$$ CACHE DUMP $$$$$$");
            for (int i=0; i<CACHE_SETS_COUNT; i=i+1) begin
                $fdisplay(f,"== SET 0x%0h\t==", i);

                $fdisplay(f,"way %0d\nvalid: %b", 0, valid_array[i][0]);
                if (valid_array[i][0]) begin
                    $fdisplay(f,"dirty: %b", dirty_array[i][0]);
                    $fdisplay(f,"tag:   0x%h", tag_array[i][0]);
                    $fdisplay(f,"data:  0x%h", data_array[i][0]);
                end

                $fdisplay(f,"way %0d\nvalid: %b", 1, valid_array[i][1]);
                if (valid_array[i][1]) begin
                    $fdisplay(f,"dirty: %b", dirty_array[i][1]);
                    $fdisplay(f,"tag:   0x%h", tag_array[i][1]);
                    $fdisplay(f,"data:  0x%h", data_array[i][1]);
                end
                $fdisplay(f,"");
            end

            $display("Cache dumped successful. Check cache.dump");
        end else begin
            $display("Error while cache dump");
        end
        $fclose(f);
    endtask


    always @(posedge clk or posedge reset) begin
        if (reset) begin
            for (int i=0; i<CACHE_SETS_COUNT; i=i+1) begin
                valid_array[i][0] =  0;
                dirty_array[i][0] =  0;
                tag_array[i][0]   = 'z;
                data_array[i][0]  = 'z;
                valid_array[i][1] =  0;
                dirty_array[i][1] =  0;
                tag_array[i][1]   = 'z;
                data_array[i][1]  = 'z;
            end
            mem_line_buff     =  0;
            cpu_data_bus_buff = 'z;
            cpu_command_buff  = 'z;
            cur_cpu_command   =  0;
            mem_command_buff  = 'z;
            mem_data_buff     = 'z;
        end else if (dump) begin
            dump_to_console;
        end else if (cpu_command == C1_READ8 || cpu_command == C1_READ16 || cpu_command == C1_READ32) begin
            req = req + 1;

            cur_cpu_command = cpu_command;
            read_cpu_address;

            if (valid_array[cpu_set_buff][0] == 1 && tag_array[cpu_set_buff][0] == cpu_tag_buff) begin
                hit = hit + 1;
                index_in_set = 0;
                read_from_storage;
            end else if (valid_array[cpu_set_buff][1] == 1 && tag_array[cpu_set_buff][1] == cpu_tag_buff) begin
                hit = hit + 1;
                index_in_set = 1;
                read_from_storage;
            end else begin
                replace_from_MM;
                read_from_storage;
            end

            cpu_command_buff = C1_WRITE32_RESP;
            delay;

            if (cur_cpu_command == C1_READ32) begin
                delay;
                cpu_offset_buff += 2;
                read_from_storage;
                delay;
            end

            cpu_command_buff = 'z;
            cpu_data_bus_buff = 'z;
            cur_cpu_command = 'z;

        end else if (cpu_command == C1_WRITE8 || cpu_command == C1_WRITE16 || cpu_command == C1_WRITE32_RESP) begin
            req = req + 1;

            cur_cpu_command = cpu_command;
            cpu_data_to_write[0 +: BUS_SIZE] = cpu_data;

            read_cpu_address;

            if (cpu_command == C1_WRITE32_RESP) begin
                cpu_data_to_write[BUS_SIZE +: BUS_SIZE] = cpu_data;
            end

            if (valid_array[cpu_set_buff][0] == 1 && tag_array[cpu_set_buff][0] == cpu_tag_buff) begin
                hit = hit + 1;
                index_in_set = 0;
                write_to_storage;
            end else if (valid_array[cpu_set_buff][1] == 1 && tag_array[cpu_set_buff][1] == cpu_tag_buff) begin
                hit = hit + 1;
                index_in_set = 1;
                write_to_storage;
            end else begin
                replace_from_MM;
                write_to_storage;
            end

            cpu_command_buff = C1_WRITE32_RESP;
            delay;
            cpu_command_buff = 'z;
            cur_cpu_command  = 'z;

        end else if (cpu_command == C1_INV_LINE) begin
            read_cpu_address;
            if (tag_array[cpu_set_buff][0] == cpu_tag_buff) begin
                index_in_set = 0;
                evict_if_dirty;
                valid_array[cpu_set_buff][0] = 0;
            end else if (tag_array[cpu_set_buff][1] == cpu_tag_buff) begin
                index_in_set = 0;
                evict_if_dirty;
                valid_array[cpu_set_buff][1] = 0;
            end

            cpu_command_buff = C1_WRITE32_RESP;
            delay;
            cpu_command_buff = 'z;
            cur_cpu_command  = 'z;
        end
    end

    assign mem_address = mem_address_buff;
    assign mem_data = mem_data_buff;
    assign mem_command = mem_command_buff;
    assign cpu_data = cpu_data_bus_buff;
    assign cpu_command = cpu_command_buff;

endmodule
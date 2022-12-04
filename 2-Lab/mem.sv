module mem #(
    parameter MEM_ADDR_SIZE = 10 + 9, 
    parameter BUS_SIZE = 16 ,
    parameter CACHE_OFFSET_SIZE = 4,
    parameter CACHE_LINE_SIZE = 16
    ) (
    input clk,
    input reset,
    input dump,
    input [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] address,
    inout [BUS_SIZE-1:0] data,
    inout [2-1:0] command
);

    // 512KB = 2^9 * 2^10 = 2^19 = 2^15 lines * 2^4 bits in each line (16 8-bit words)
    parameter MEM_SIZE = 1 << (MEM_ADDR_SIZE-CACHE_OFFSET_SIZE); // 2^15 cache lines
    parameter RESPONSE_TIME = 100;

    localparam  C2_NOP      = 3'd0,
                C2_RESPONSE = 3'd1,
                C2_READ     = 3'd2,
                C2_WRITE    = 3'd3;

    reg [CACHE_LINE_SIZE*8-1:0] storage [MEM_SIZE-1:0]; // 2^15 16-byte lines
    reg [BUS_SIZE-1:0] data_buff; // single bus
    reg [2-1:0] command_buff;

    integer SEED = 225526;

    task delay;
        begin
            @(negedge clk);
        end
    endtask

    task read_bus_delay;
        begin
            @(posedge clk);
        end
    endtask	

    task wait_and_response;
        repeat(RESPONSE_TIME) begin
            delay;
        end
        command_buff = C2_RESPONSE;
    endtask

    int dump_f;
    task dump_to_console;
        dump_f = $fopen("mem.dump", "w");
        if (dump_f) begin
            
            $fdisplay(dump_f, "$$$$$$ MEM DUMP $$$$$$");
            for (int i=0; i<99; i=i+1) begin
                $fdisplay(dump_f, "0x%0H\t0x%0H\t0x%h\n", (i >> 5), i%32, storage[i]);
            end

            $display("Mem dumped successful. Check mem.dump");
        end else begin
            $display("Error while mem dump");
        end
        $fclose(dump_f);
    endtask

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            $display("Filling the MEM...");
            for (int i = 0; i < 99; i=i+1) begin
                storage[i] = $random(SEED)>>16;
            end
            $display("MEM filled");
            data_buff = 'z;
            command_buff = 'z;
        end else if (dump) begin
            dump_to_console;
        end else begin
            if (command == C2_READ) begin
                wait_and_response;

                // READ
                for (int i=0; i<CACHE_LINE_SIZE/2; i=i+1) begin
                    data_buff = storage[address][BUS_SIZE*i +: BUS_SIZE];
                    delay;
                end
                command_buff = 'z;
            end else if (command == C2_WRITE) begin
                data_buff = 'z;
                wait_and_response;

                // WRITE
                for (int i=0; i<CACHE_LINE_SIZE/2; i=i+1) begin
                    read_bus_delay;
                    storage[address][BUS_SIZE*i +: BUS_SIZE] = data;
                end
                command_buff = 'z;
            end else begin
                data_buff = 'z;
            end
        end
    end

    assign data = data_buff;
    assign command = command_buff;
endmodule
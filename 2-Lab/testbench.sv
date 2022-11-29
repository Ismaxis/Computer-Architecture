module testbench ();
    parameter BUS_SIZE = 16 ;
    parameter MEM_ADDR_SIZE = 10 + 9;   // log2(MEM_SIZE)
    parameter CACHE_OFFSET_SIZE = 4;    // log2(CACHE_LINE_SIZE)
    parameter CACHE_LINE_SIZE = 16;
    wire [MEM_ADDR_SIZE-1:0] cpu_address;
    wire [BUS_SIZE-1:0] cpu_data;
    wire [3-1:0] cpu_command;
    wire [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] mem_address;
    wire [BUS_SIZE-1:0] mem_data;
    wire [2-1:0] mem_command;

    reg clk;
    reg reset;


    cpu #(MEM_ADDR_SIZE, BUS_SIZE) 
    cpu (clk, cpu_address, cpu_data, cpu_command);

    cache #(BUS_SIZE, MEM_ADDR_SIZE, CACHE_OFFSET_SIZE, CACHE_LINE_SIZE)
    cache (clk, reset, 
    cpu_address, cpu_data, cpu_command,
    mem_address, mem_data, mem_command);

    mem #(MEM_ADDR_SIZE, BUS_SIZE, CACHE_OFFSET_SIZE, CACHE_LINE_SIZE) 
    mem(clk, reset, mem_address, mem_data, mem_command);

    initial begin
        reset = 0;
        #1 reset = 1;
        #1 reset = 0;
        clk = 1'd0;

        forever begin
            #1 clk = ~clk;
            // $display("addr: %b", mem_address);
            // $display("data: %b", mem_data);
            // $display("command: %b", mem_command);
        end
    end

    initial begin
        $dumpfile("dump_cache.vcd"); 
        $dumpvars;
    end

endmodule
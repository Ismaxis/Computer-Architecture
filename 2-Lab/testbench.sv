`include "cache.sv"
`include "cpu.sv"
`include "mem.sv"

module testbench ();
    parameter BUS_SIZE = 16;
    parameter MEM_ADDR_SIZE = 10 + 9;   // log2(MEM_SIZE)
    parameter CACHE_OFFSET_SIZE = $clog2(CACHE_LINE_SIZE);
    parameter CACHE_LINE_SIZE = 16;
    wire [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] cpu_address;
    wire [BUS_SIZE-1:0] cpu_data;
    wire [3-1:0] cpu_command;
    wire [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] mem_address;
    wire [BUS_SIZE-1:0] mem_data;
    wire [2-1:0] mem_command;
    wire cache_dump;
    wire mem_dump;

    reg clk;
    reg reset;


    cpu #(MEM_ADDR_SIZE, BUS_SIZE, CACHE_OFFSET_SIZE) 
    cpu (
        .clk(clk),
        .cache_dump(cache_dump),
        .mem_dump(mem_dump),
        .address(cpu_address), 
        .data(cpu_data), 
        .command(cpu_command)
        );

    cache #(BUS_SIZE, MEM_ADDR_SIZE, CACHE_OFFSET_SIZE, CACHE_LINE_SIZE)
    cache (
        .clk(clk), 
        .reset(reset), 
        .dump(cache_dump), 
        
        .cpu_address(cpu_address), 
        .cpu_data(cpu_data), 
        .cpu_command(cpu_command),
        
        .mem_address(mem_address), 
        .mem_data(mem_data), 
        .mem_command(mem_command)
        );

    mem #(MEM_ADDR_SIZE, BUS_SIZE, CACHE_OFFSET_SIZE, CACHE_LINE_SIZE) 
    mem(
        .clk(clk), 
        .reset(reset), 
        .dump(mem_dump),
        .address(mem_address), 
        .data(mem_data), 
        .command(mem_command)
        );

    int cyclesCount = 1;
    initial begin
        #2 forever begin
            #1 if (clk) begin
                ++cyclesCount;
            end
            if (mem_dump) begin
                $display("Total clock cycles: %0d", cyclesCount);
                repeat(2) begin
                    @(posedge clk);
                end
            end
        end
    end

    initial begin
        reset = 0;
        #1 reset = 1;
        #1 reset = 0;
        clk = 1'd0;

        forever begin
            #1 clk = ~clk;
        end
    end

endmodule
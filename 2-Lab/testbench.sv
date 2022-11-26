module testbench ();
    parameter DATA_SIZE = 16;
    
    reg clk;

    wire [MEM_ADDR_SIZE-1:0] cpu_address,
    wire [DATA_SIZE-1:0] cpu_data,
    wire [3-1:0] cpu_command,
    wire [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] mem_address,
    wire [DATA_SIZE-1:0] mem_data,
    wire [2-1:0] mem_command,

    cpu cpu(
        .clk(clk),
        .address(cpu_address),
        .data(cpu_data),
        .command(cpu_command)
    );
    cache cache(
        .clk(clk)
        
        .cpu_address(cpu_address),
        .cpu_data(cpu_data),
        .cpu_command(cpu_command)
        .mem_address(mem_address),
        .mem_data(mem_data),
        .mem_command(mem_command)
    );
    mem mem(
        .clk(clk)
        .address(mem_address),
        .data(mem_data),
        .command(mem_command)
    );

endmodule
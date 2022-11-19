module cpu();
    reg clk;
    reg [MEM_ADDR_SIZE-1:0] address;
    reg [CACHE_LINE_SIZE*8-1:0] data;
    reg [2:0] command;

    cache cache(
        .clk(clk),
        .cpu_address(address),
        .cpu_data(data),
        .cpu_command(command),
    );


endmodule
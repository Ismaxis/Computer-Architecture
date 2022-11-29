module cpu #(
    parameter MEM_ADDR_SIZE = 10 + 9, 
    parameter BUS_SIZE = 16,
    parameter CACHE_OFFSET_SIZE = 4
    ) (

    input clk,
    output [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] address,
    inout [BUS_SIZE-1:0] data,
    inout [3-1:0] command
);

    localparam  C1_NOP      = 3'd0,
                C1_READ8    = 3'd1,
                C1_READ16   = 3'd2,
                C1_READ32   = 3'd3,
                C1_INV_LINE = 3'd4,
                C1_WRITE8   = 3'd5,
                C1_WRITE16  = 3'd6,
                C1_WRITE32  = 3'd7;

    task delay;
        begin
            @(negedge clk);
        end
    endtask	
    reg [3-1:0] cpu_command_buff;
    reg [MEM_ADDR_SIZE-1:0] cpu_address_buff;
    reg [BUS_SIZE-1:0] recieved_data;
    
    // Place for test calls
    initial begin
        recieved_data = 0;
        delay;

        cpu_command_buff = C1_READ8;
        cpu_address_buff = 15'b000000000000011;
        delay;
        cpu_address_buff = 15'b0010;
        delay;
        cpu_command_buff = C1_NOP;
        delay;
        delay;
        delay;
        delay;
        delay;
        delay;
        delay;
        delay;
        delay;
        delay;
        recieved_data = data;
        $display("recieved: %b", recieved_data);
        $finish();
    end
    assign command = cpu_command_buff;
    assign address = cpu_address_buff;
endmodule
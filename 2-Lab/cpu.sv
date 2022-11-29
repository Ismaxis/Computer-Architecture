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

    localparam  C1_NOP          = 3'd0,
                C1_READ8        = 3'd1,
                C1_READ16       = 3'd2,
                C1_READ32       = 3'd3,
                C1_INV_LINE     = 3'd4,
                C1_WRITE8       = 3'd5,
                C1_WRITE16      = 3'd6,
                C1_WRITE32_RESP = 3'd7;
    
    reg [MEM_ADDR_SIZE-1:0] cpu_address_buff;
    reg [BUS_SIZE-1:0] data_to_write;
    reg [3-1:0] cpu_command_buff;
    reg [BUS_SIZE-1:0] recieved_data;

    task delay;
        begin
            @(negedge clk);
        end
    endtask	
    
    initial begin
        cpu_address_buff = 'x;
        cpu_command_buff = 'z;
        recieved_data = 0;
        data_to_write = 'z;
    end

    // Place for test calls
    initial begin
        delay;
        // READ1
        cpu_command_buff = C1_READ8;
        cpu_address_buff = 15'b0000000000_00010;
        delay;
        cpu_address_buff = 15'b0000;
        delay;
        cpu_command_buff = 'z;
        delay;
        while (command !== C1_WRITE32_RESP) begin 
            delay;
        end
        recieved_data = data;
        $display("recieved: %b", recieved_data);
        
        // WRITE1
        cpu_command_buff = C1_WRITE8;
        cpu_address_buff = 15'b0000000000_00010;
        data_to_write = 16'b0101010101010101;
        delay;
        cpu_address_buff = 15'b0000;
        delay;
        cpu_command_buff = 'z;
        delay;
        while (command !== C1_WRITE32_RESP) begin 
            delay;
        end
        data_to_write = 'z;
        
        // READ2
        cpu_command_buff = C1_READ8;
        cpu_address_buff = 15'b0000000000_00010;
        delay;
        cpu_address_buff = 15'b0000;
        delay;
        cpu_command_buff = 'z;
        delay;
        while (command !== C1_WRITE32_RESP) begin 
            delay;
        end
        recieved_data = data;
        $display("recieved: %b", recieved_data);
        
        // READ3
        cpu_command_buff = C1_READ8;
        cpu_address_buff = 15'b0000000001_00010;
        delay;
        cpu_address_buff = 15'b0000;
        delay;
        cpu_command_buff = 'z;
        delay;
        while (command !== C1_WRITE32_RESP) begin 
            delay;
        end
        recieved_data = data;
        $display("recieved: %b", recieved_data);

        // READ4
        cpu_command_buff = C1_READ8;
        cpu_address_buff = 15'b0000000011_00010;
        delay;
        cpu_address_buff = 15'b0000;
        delay;
        cpu_command_buff = 'z;
        delay;
        while (command !== C1_WRITE32_RESP) begin 
            delay;
        end
        recieved_data = data;
        $display("recieved: %b", recieved_data);

        $finish();
    end

    assign address = cpu_address_buff;
    assign data = data_to_write;
    assign command = cpu_command_buff;
    
endmodule
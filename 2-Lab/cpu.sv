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
    reg [MEM_ADDR_SIZE-CACHE_OFFSET_SIZE-1:0] address_bus_buff;
    reg [BUS_SIZE-1:0] data_to_write;
    reg [3-1:0] cpu_command_buff;
    reg [BUS_SIZE-1:0] recieved_data;
    reg [BUS_SIZE*2-1:0] local_storage;

    task delay;
        begin
            @(negedge clk);
        end
    endtask	
    
    task send_address;
        address_bus_buff = cpu_address_buff[MEM_ADDR_SIZE-1:CACHE_OFFSET_SIZE];
        delay;
        address_bus_buff = cpu_address_buff[CACHE_OFFSET_SIZE-1:0];
        delay;
    endtask

    task wait_for_resp;
        while (command !== C1_WRITE32_RESP) begin 
            delay;
        end
    endtask

    task READ8;
        cpu_command_buff = C1_READ8;
        READ;
        local_storage = recieved_data;
    endtask
    task READ16;
        cpu_command_buff = C1_READ16;
        READ;
        local_storage = recieved_data;
    endtask
    task READ32;
        cpu_command_buff = C1_READ32;
        READ;
        local_storage[BUS_SIZE-1:0] = recieved_data;
        READ;
        local_storage[BUS_SIZE*2-1:BUS_SIZE] = recieved_data;
        
    endtask

    task READ;
        send_address;
        cpu_command_buff = 'z;
        wait_for_resp;
        recieved_data = data;
    endtask

    task WRITE8;
        cpu_command_buff = C1_WRITE8;
        WRITE;
    endtask
    task WRITE16;
        cpu_command_buff = C1_WRITE16;
        WRITE;
    endtask

    task WRITE;
        send_address;
        cpu_command_buff = 'z;
        wait_for_resp;
        data_to_write = 'z;
    endtask

    initial begin
        address_bus_buff = 'z;
        cpu_command_buff = 'z;
        recieved_data = 0;
        data_to_write = 'z;
    end

    // Place for test calls
    initial begin
        delay;
        $display("----------------");

        cpu_address_buff = 19'b0000000000_00010_0010;
        data_to_write = 16'b0101010101010101;
        $display("write to  %b", cpu_address_buff);
        $display("data      %b", data_to_write);
        WRITE16;
        $display("----------------");
       
        cpu_address_buff = 19'b0000000000_00010_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------");

        cpu_address_buff = 19'b0000000000_00010_0000;
        data_to_write = 16'b0101010101010101;
        $display("write to  %b", cpu_address_buff);
        $display("data      %b", data_to_write);
        WRITE16;
        $display("----------------");
        
        cpu_address_buff = 19'b0000000000_00010_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------");

        cpu_address_buff = 19'b0000000000_00010_0010;
        $display("read from %b", cpu_address_buff);
        READ8;
        $display("data      %b", local_storage);
        $display("----------------");

        $finish();
    end

    assign address = address_bus_buff;
    assign data = data_to_write;
    assign command = cpu_command_buff;
    
endmodule
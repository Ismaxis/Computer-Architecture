module cpu #(
    parameter MEM_ADDR_SIZE = 10 + 9, 
    parameter BUS_SIZE = 16,
    parameter CACHE_OFFSET_SIZE = 4
    ) (

    input clk,
    output cache_dump,
    output mem_dump,
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
    reg [BUS_SIZE*2-1:0] data_to_write; 
    reg [BUS_SIZE-1:0] data_buff;
    reg [3-1:0] cpu_command_buff;
    reg [BUS_SIZE-1:0] recieved_data;
    reg [BUS_SIZE*2-1:0] local_storage;

    reg cache_dump_buff;
    reg mem_dump_buff;

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
    task WRITE32;
        cpu_command_buff = C1_WRITE32_RESP;

        data_buff = data_to_write[BUS_SIZE-1:0];
        address_bus_buff = cpu_address_buff[MEM_ADDR_SIZE-1:CACHE_OFFSET_SIZE];
        delay;

        data_buff = data_to_write[BUS_SIZE*2-1:BUS_SIZE];
        address_bus_buff = cpu_address_buff[CACHE_OFFSET_SIZE-1:0];
        delay;

        cpu_command_buff = 'z;
        wait_for_resp;
        data_buff = 'z;
    endtask
    task WRITE;
        data_buff = data_to_write;
        send_address;
        cpu_command_buff = 'z;
        wait_for_resp;
        data_buff = 'z;
    endtask

    task INV;
        cpu_command_buff = C1_INV_LINE;
        send_address;
        cpu_command_buff = 'z;
        wait_for_resp;
    endtask

    initial begin
        address_bus_buff = 'z;
        cpu_command_buff = 'z;
        recieved_data = 0;
        data_buff = 'z;
        cache_dump_buff = 0;
        mem_dump_buff = 0;
    end

    task read_write_test;
        $display("\n#############################################");
        $display("#############  READ/WRITE TEST  #############");
        $display("#############################################\n");

        $display("@@@@@@@@@@@@@@");
        $display("@@@  8 bit @@@");
        $display("@@@@@@@@@@@@@@");
        cpu_address_buff = 19'b0000000000_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ8;
        $display("data      %b", local_storage[8-1:0]);
        $display("----------------");

        cpu_address_buff = 19'b0000000000_01110_0000;
        data_to_write = 8'b1111_0000;
        $display("write to  %b", cpu_address_buff);
        $display("data      %b", data_to_write[8-1:0]);
        WRITE8;
        $display("----------------");
        
        cpu_address_buff = 19'b0000000000_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ8;
        $display("data      %b", local_storage[8-1:0]);
        $display("----------------");

        $display("@@@@@@@@@@@@@@");
        $display("@@@ 16 bit @@@");
        $display("@@@@@@@@@@@@@@");
        cpu_address_buff = 19'b0000000000_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ16;
        $display("data      %b", local_storage[16-1:0]);
        $display("----------------");

        cpu_address_buff = 19'b0000000000_01110_0000;
        data_to_write = 16'b1111_1111_0000_0000;
        $display("write to  %b", cpu_address_buff);
        $display("data      %b", data_to_write[16-1:0]);
        WRITE16;
        $display("----------------");
        
        cpu_address_buff = 19'b0000000000_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ16;
        $display("data      %b", local_storage[16-1:0]);
        $display("----------------");

        $display("@@@@@@@@@@@@@@");
        $display("@@@ 32 bit @@@");
        $display("@@@@@@@@@@@@@@");
        $display("----------------");
        cpu_address_buff = 19'b0000000000_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage[32-1:0]);
        $display("----------------");

        cpu_address_buff = 19'b0000000000_01110_0000;
        data_to_write = 32'b0101_0101_0101_0101_0101_0101_0101_0101;
        $display("write to  %b", cpu_address_buff);
        $display("data      %b", data_to_write[32-1:0]);
        WRITE32;
        $display("----------------");
       
        cpu_address_buff = 19'b0000000000_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage[32-1:0]);
        $display("----------------");
    endtask

    task eviction_test;
        $display("\n###########################################");
        $display("#############  EVICTION TEST  #############");
        $display("###########################################\n");

        $display("~~~~~~~~~~~~~");
        $display("~~~ FIRST ~~~");
        $display("~~~~~~~~~~~~~");
        $display("----------------");
        cpu_address_buff = 19'b0000000000_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------");

        cpu_address_buff = 19'b0000000000_01110_0000;
        data_to_write = 32'b1111_0000_1111_0000_0000_1111_0000_1111;
        $display("write to  %b", cpu_address_buff);
        $display("data      %b", data_to_write);
        WRITE32;
        $display("----------------");

        cpu_address_buff = 19'b0000000000_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------\n");

        $display("~~~~~~~~~~~~~~");
        $display("~~~ SECOND ~~~");
        $display("~~~~~~~~~~~~~~");
        $display("----------------");
        cpu_address_buff = 19'b0000000001_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------");

        cpu_address_buff = 19'b0000000001_01110_0000;
        data_to_write = 32'b1111_1111_1111_1111_0000_1111_1111_0000;
        $display("write to  %b", cpu_address_buff);
        $display("data      %b", data_to_write);
        WRITE32;
        $display("----------------");

        cpu_address_buff = 19'b0000000001_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------\n");

        $display("~~~~~~~~~~~~~");
        $display("~~~ THIRD ~~~");
        $display("~~~~~~~~~~~~~");
        $display("----------------");
        cpu_address_buff = 19'b0000000010_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------");

        cpu_address_buff = 19'b0000000010_01110_0000;
        data_to_write = 32'b1111_1111_0001_1001_1001_1000_1111_1111;
        $display("write to  %b", cpu_address_buff);
        $display("data      %b", data_to_write);
        WRITE32;
        $display("----------------");

        cpu_address_buff = 19'b0000000010_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------\n");

        $display("~~~~~~~~~~~~~~~~~~~");
        $display("~~~ FIRST AGAIN ~~~");
        $display("~~~~~~~~~~~~~~~~~~~");
        $display("----------------");
        cpu_address_buff = 19'b0000000000_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------\n");
        
        $display("~~~~~~~~~~~~~~~~~~~~");
        $display("~~~ SECOND AGAIN ~~~");
        $display("~~~~~~~~~~~~~~~~~~~~");
        $display("----------------");
        cpu_address_buff = 19'b0000000001_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------\n");

        $display("~~~~~~~~~~~~~~~~~~~");
        $display("~~~ THIRD AGAIN ~~~");
        $display("~~~~~~~~~~~~~~~~~~~");
        $display("----------------");
        cpu_address_buff = 19'b0000000010_01110_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------\n");
    endtask

    task invalidate_test; 
        $display("\n#############################################");
        $display("#############  INVALIDATE TEST  #############");
        $display("#############################################\n");

        $display("----------------");
        cpu_address_buff = 19'b0000000000_10001_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------");

        cpu_address_buff = 19'b0000000000_10001_0000;
        data_to_write = 32'b0101_0101_0101_0101_0101_0101_0101_0101;
        $display("write to  %b", cpu_address_buff);
        $display("data      %b", data_to_write);
        WRITE32;
        $display("----------------");

        cpu_address_buff = 19'b0000000000_10001_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------");
        
        cpu_address_buff = 19'b0000000000_10001_0000;
        $display("inv_line  %b", cpu_address_buff);
        INV;
        $display("----------------");

        cpu_address_buff = 19'b0000000000_10001_0000;
        $display("read from %b", cpu_address_buff);
        READ32;
        $display("data      %b", local_storage);
        $display("----------------");
    endtask

    // Place for test calls
    initial begin
        delay;
        
        read_write_test;

        invalidate_test;

        eviction_test;

        cache_dump_buff = 1;
        delay;
        cache_dump_buff = 0;

        mem_dump_buff = 1;
        delay;
        mem_dump_buff = 0;

        $finish();
    end

    assign address = address_bus_buff;
    assign data = data_buff;
    assign command = cpu_command_buff;
    assign cache_dump = cache_dump_buff;
    assign mem_dump = mem_dump_buff;
    
endmodule
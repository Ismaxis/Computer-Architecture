module ram (
    input [word_size - 1:0] data,
    input [word_size - 1:0] addr,	
    input wr,	
    input clk,	

    output response,
    output [word_size - 1:0] out
);

    parameter word_size = 32;

    parameter size = 1<<32;
    reg [word_size-1:0] ram [size-1:0];

    reg [word_size-1:0] data_reg;	
    reg [word_size-1:0] addr_reg;
    reg wr_reg;


    reg [word_size-1:0] out_reg;
    reg response_reg;

    initial
    begin
        response_reg = 1;
        data_reg = 0;
        addr_reg = 0;
        wr_reg = 0;
    end


    always @(negedge clk)
    begin
        if ((data != data_reg) || (addr%size != addr_reg)|| (wr != wr_reg))
        begin
            response_reg = 0;
            data_reg = data;
            addr_reg = addr%size;
            wr_reg = wr;
        end
        else
        begin
            if (response_reg == 0)
            begin
                if (wr)
                    ram[addr] = data;
                else
                    out_reg = ram[addr];
                    
                response_reg = 1;
            end
        end
    end

    assign out = out_reg;
    assign response = response_reg;

endmodule
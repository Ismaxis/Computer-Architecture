module direct_mapping_cache (
    input [word_size-1:0] data,
    input [word_size-1:0] addr,
    input wr,
    input clk,

    output response,
    output is_missrate,
    output [word_size-1:0] out
);
    parameter word_size = 32;

    parameter size = 64;
    parameter index_size = 6;

    reg [word_size-1:0] data_array [size-1:0];

    reg validity_array [size-1:0];
    reg [word_size-index_size-1:0] tag_array [size-1:0];
    reg [index_size-1:0] index_array [size-1:0];

    reg [word_size-index_size-1:0] tag;
    reg [index_size-1:0] index;

    reg [word_size-1:0] data_reg;
    reg [word_size-1:0] addr_reg;
    reg wr_reg;

    reg response_reg;
    reg is_missrate_reg;
    reg [word_size-1:0] out_reg;

    reg [word_size-1:0] ram_data;
    reg [word_size-1:0] ram_addr;
    reg ram_wr;

    wire ram_response;
    wire [word_size-1:0] ram_out;

    ram ram(
        .data(ram_data),
        .addr(ram_addr),
        .wr(ram_wr),
        .clk(clk),
        .response(ram_response),
        .out(ram_out)
    );

    integer i;
    initial begin
        data_reg = 0;
        addr_reg = 0;
        wr_reg = 0;
        for (i = 0; i < size; i = i + 1) begin
            data_array[i] = 0;
            tag_array[i] = 0;
            validity_array[i] = 0;
        end
    end

    always @(posedge clk)
    begin
        if (data_reg != data || addr_reg != addr || wr_reg != wr)
        begin
            response_reg = 0;

            data_reg = data;
            addr_reg = addr;
            wr_reg = wr;
      
            tag = addr >> index_size;
            index = addr;

            if (wr)
            begin
                // запись
                data_array[index] = data;
                tag_array[index] = tag;			
                validity_array[index] = 1;

                ram_data = data;
                ram_addr = addr;
                ram_wr = wr;
            end
            else
            begin
                // считывание
                if ((validity_array[index]) && (tag == tag_array[index]))
                begin
                    // найден в кэше
                    is_missrate_reg = 0;

                    out_reg = data_array[index];
                    response_reg = 1;
                end
                else
                begin
                    // не найден в кэше
                    is_missrate_reg = 1;

                    ram_data = data;
                    ram_addr = addr;
                    ram_wr = wr;			
                end
            end
        end
        else
        begin
            if ((ram_response) && (!response_reg))
            begin
                if (wr == 0)
                begin
                    validity_array [index] = 1;
                    data_array [index] = ram_out;
                    tag_array[index] = tag;
                    out_reg = ram_out;
                end
                response_reg = 1;
            end
        end
    end

    assign out = out_reg;
    assign is_missrate = is_missrate_reg;
    assign response = response_reg;

endmodule
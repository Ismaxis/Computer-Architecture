module tb_;

    reg[word_size-1:0] data;
    reg[word_size-1:0] addr;
    reg wr;
    reg clk;

    wire response;
    wire is_missrate;
    wire[word_size-1:0] out;

    parameter word_size = 32;

    direct_mapping_cache cache(
        .data(data),
        .addr(addr),
        .wr(wr),
        .clk(clk),
        .response(response),
        .is_missrate(is_missrate),
        .out(out)
    );

    initial begin
        clk = 0;
        $monitor("response: %b", response);
        $monitor("is_missrate: %b", is_missrate);
        $monitor("out: %b", out);

        data = 8'b01011110;
        addr = 8'b00000000;
        wr = 1;
        #1 clk = ~clk;
        #1 clk = ~clk;
        #1 wr = 0;
        #1 addr = 8'b00000000;
        #1 clk = ~clk;
        #1 clk = ~clk; 
    end

endmodule
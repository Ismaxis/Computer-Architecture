module tb_;

    reg[8:0] data;
    reg clk;
    wire out;
    my_xor xor1(
        .data(data),
        .clk(clk),
        .out(out)
    );

    initial begin
        clk = 0;
        $monitor("%b", out);
        data = 8'b01011110;
        clk = ~clk;
    end

endmodule
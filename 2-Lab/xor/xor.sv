module my_xor(
    input [8:0] data,
    input clk,
    output out 
); 
    reg result;
    integer i;

    always @(posedge clk)
    begin
        result = 0;
        $monitor("%b", result);

        for (i = 0; i < 8; i = i + 1)
        begin
            result = data[i]^result;
        end
    end

    assign out = result;

endmodule
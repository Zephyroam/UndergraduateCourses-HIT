`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/09 17:03:13
// Design Name: 
// Module Name: Clock_1ms
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Clock_1ms(
    CLK,
    CLK_1ms
    );
    input CLK;
    output CLK_1ms;
    reg CLK_1ms;
    reg [31:0]Counter;
    initial
    begin
        Counter = 0;
        CLK_1ms = 0;
    end
    always@(posedge CLK)
    begin
        if(Counter == 50000)
        begin
            Counter = 0;
            CLK_1ms = CLK_1ms ^ 1;
        end
        else
            Counter = Counter + 1;
    end
endmodule

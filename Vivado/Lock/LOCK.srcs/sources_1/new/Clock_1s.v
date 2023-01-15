`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/07 22:17:42
// Design Name: 
// Module Name: Clock_1s
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


module Clock_1s(
    CLK,
    CLK_1s
    );
    input CLK;
    output CLK_1s;
    reg CLK_1s;
    reg [31:0]Counter;
    initial
    begin
        Counter = 0;
        CLK_1s = 0;
    end
    always@(posedge CLK)
    begin
        if(Counter == 50000000)
        begin
            Counter = 0;
            CLK_1s = CLK_1s ^ 1;
        end
        else
            Counter = Counter + 1;
    end
endmodule

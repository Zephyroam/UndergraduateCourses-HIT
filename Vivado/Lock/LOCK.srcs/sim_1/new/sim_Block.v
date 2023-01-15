`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/13 11:47:14
// Design Name: 
// Module Name: sim_Block
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


module sim_Block(

    );
    reg CLK;
    reg CLK_1s_CLK;
    reg Block_Start;
    wire BlockTimeUp;
    wire [9:0]BlockTime;
    initial
    begin
        CLK = 0;
        CLK_1s_CLK = 0;
        Block_Start = 0;
    end
    Block B(CLK_1s_CLK, CLK, Block_Start, BlockTimeUp, BlockTime);
    always #1
        CLK = CLK + 1;
    always #100
        CLK_1s_CLK = CLK_1s_CLK + 1;
    always #1000
    begin
        Block_Start = 1;
        #1000;
        Block_Start = 0;
    end
endmodule

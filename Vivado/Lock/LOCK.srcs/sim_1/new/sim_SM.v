`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/15 20:27:58
// Design Name: 
// Module Name: sim_SM
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


module sim_SM(

    );
    reg CLK;
    reg CLK_1s_CLK;
    reg SecurityMode_Start;
    wire SecurityModeTimeUp;
    wire [9:0]SecurityModeTime1;
    wire [9:0]SecurityModeTime2;
    initial
    begin
        CLK = 0;
        CLK_1s_CLK = 0;
        SecurityMode_Start = 0;
    end
    SecurityMode SeM(CLK_1s_CLK, CLK, SecurityMode_Start, SecurityModeTimeUp, SecurityModeTime1, SecurityModeTime2);
    always #1
        CLK = CLK + 1;
    always #10
        CLK_1s_CLK = CLK_1s_CLK + 1;
    always #600
    begin
        SecurityMode_Start = 1;
        #600;
        SecurityMode_Start = 0;
    end
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/11 13:54:00
// Design Name: 
// Module Name: SecurityMode
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


module SecurityMode(
    CLK_1s_CLK, CLK_1ms_CLK, SecurityMode_Start, SecurityModeTimeUp, SecurityModeTime1, SecurityModeTime2
    );
    input CLK_1s_CLK, CLK_1ms_CLK, SecurityMode_Start;
    output SecurityModeTimeUp;
    output [9:0]SecurityModeTime1;
    output [9:0]SecurityModeTime2;
    reg SecurityModeTimeUp;
    reg [7:0]SecurityModeTimer;
    reg [9:0]SecurityModeTime1;
    reg [9:0]SecurityModeTime2;
    reg [7:0]SecurityModeTime_Ori;
    initial
    begin
        SecurityModeTimer = 0;
        SecurityModeTimeUp = 0;
        SecurityModeTime1 = 0;
        SecurityModeTime2 = 0;
        SecurityModeTime_Ori = 0;
    end
    always@(posedge CLK_1ms_CLK)
        SecurityModeTime_Ori = 30 - SecurityModeTimer;
    always@(negedge CLK_1ms_CLK)
    begin
        if(SecurityModeTime_Ori / 10 == 0)
            SecurityModeTime1 = 10'b0_000_000_001;
        else if(SecurityModeTime_Ori / 10 == 1)
            SecurityModeTime1 = 10'b0_000_000_010;
        else if(SecurityModeTime_Ori / 10 == 2)
            SecurityModeTime1 = 10'b0_000_000_100;
        else if(SecurityModeTime_Ori / 10 == 3)
            SecurityModeTime1 = 10'b0_000_001_000;
        else if(SecurityModeTime_Ori / 10 == 4)
            SecurityModeTime1 = 10'b0_000_010_000;
        else if(SecurityModeTime_Ori / 10 == 5)
            SecurityModeTime1 = 10'b0_000_100_000;
        else if(SecurityModeTime_Ori / 10 == 6)
            SecurityModeTime1 = 10'b0_001_000_000;
        else if(SecurityModeTime_Ori / 10 == 7)
            SecurityModeTime1 = 10'b0_010_000_000;
        else if(SecurityModeTime_Ori / 10 == 8)
            SecurityModeTime1 = 10'b0_100_000_000;
        else if(SecurityModeTime_Ori / 10 == 9)
            SecurityModeTime1 = 10'b1_000_000_000;
        
        if(SecurityModeTime_Ori % 10 == 0)
            SecurityModeTime2 = 10'b0_000_000_001;
        else if(SecurityModeTime_Ori % 10 == 1)
            SecurityModeTime2 = 10'b0_000_000_010;
        else if(SecurityModeTime_Ori % 10 == 2)
            SecurityModeTime2 = 10'b0_000_000_100;
        else if(SecurityModeTime_Ori % 10 == 3)
            SecurityModeTime2 = 10'b0_000_001_000;
        else if(SecurityModeTime_Ori % 10 == 4)
            SecurityModeTime2 = 10'b0_000_010_000;
        else if(SecurityModeTime_Ori % 10 == 5)
            SecurityModeTime2 = 10'b0_000_100_000;
        else if(SecurityModeTime_Ori % 10 == 6)
            SecurityModeTime2 = 10'b0_001_000_000;
        else if(SecurityModeTime_Ori % 10 == 7)
            SecurityModeTime2 = 10'b0_010_000_000;
        else if(SecurityModeTime_Ori % 10 == 8)
            SecurityModeTime2 = 10'b0_100_000_000;
        else if(SecurityModeTime_Ori % 10 == 9)
            SecurityModeTime2 = 10'b1_000_000_000;
    end
    always@(posedge CLK_1s_CLK)
    begin
        if(SecurityModeTimer == 30)
        begin
            SecurityModeTimeUp = 1;
        end
        if(SecurityMode_Start)
        begin
            SecurityModeTimer = SecurityModeTimer + 1;
        end
        else
        begin
            SecurityModeTimeUp = 0;
            SecurityModeTimer = 0;
        end
    end
endmodule


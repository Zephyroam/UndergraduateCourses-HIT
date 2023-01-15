`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/13 11:48:36
// Design Name: 
// Module Name: sim_DS
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


module sim_DS(

    );
    reg CLK;
    reg ChangePWDMode, SecurityMode_Start, Open;
    wire Status1, Status2;
    initial
    begin 
        CLK = 0;
        ChangePWDMode = 0;
        SecurityMode_Start = 0;
        Open = 0;
    end
    always #1
        CLK = CLK + 1;
    DisplayStatus DS(CLK, ChangePWDMode, SecurityMode_Start, Open, Status1, Status2);
    always #100
    begin
        ChangePWDMode = 1;
        #100 ChangePWDMode = 0;
        #100 SecurityMode_Start = 1;
        #100 SecurityMode_Start = 0;
        #100 Open = 1;
        #100 Open = 0;
    end
endmodule

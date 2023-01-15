`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/13 10:45:48
// Design Name: 
// Module Name: sim_Alarm
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


module sim_Alarm(

    );
    reg CLK;
    reg Alarm_Start;
    initial
    begin 
        CLK = 0;
        Alarm_Start = 0;
    end
    Alarm A(CLK, Alarm_Start, Alarm_Out);
    always #1
        CLK = CLK + 1;
    always #10000
    begin
        Alarm_Start = 1;
        #10;
        Alarm_Start = 0;
    end
endmodule

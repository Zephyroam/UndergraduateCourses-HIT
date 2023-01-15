`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/13 10:47:51
// Design Name: 
// Module Name: sim_Clock_1ms
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


module sim_Clock_1ms(

    );
    reg CLK;
    wire CLK_1ms;
    initial
    begin 
        CLK = 0;
    end
    Clock_1ms C1ms(CLK,CLK_1ms);
    always #1
        CLK = CLK + 1;
endmodule

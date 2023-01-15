`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/09 18:29:35
// Design Name: 
// Module Name: CheckPWD
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


module CheckPWD(
    Num1, Num2, Num3, Num4, PWD1, PWD2, PWD3, PWD4, CheckRes
    );
    input [7:0]Num1;
    input [7:0]Num2;
    input [7:0]Num3;
    input [7:0]Num4;
    input [7:0]PWD1;
    input [7:0]PWD2;
    input [7:0]PWD3;
    input [7:0]PWD4;
    output CheckRes;
    wire CheckRes;
    assign CheckRes = (Num1 == PWD1) && (Num2 == PWD2) && (Num3 == PWD3) && (Num4 == PWD4);
endmodule

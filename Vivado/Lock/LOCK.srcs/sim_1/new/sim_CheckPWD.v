`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/13 11:41:08
// Design Name: 
// Module Name: sim_CheckPWD
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


module sim_CheckPWD(

    );
    reg [7:0]Num1;
    reg [7:0]Num2;
    reg [7:0]Num3;
    reg [7:0]Num4;
    reg [7:0]PWD1;
    reg [7:0]PWD2;
    reg [7:0]PWD3;
    reg [7:0]PWD4;
    wire CheckRes;
    initial
    begin
        Num1 = 0;
        Num2 = 0;
        Num3 = 0;
        Num4 = 0;
        PWD1 = 1;
        PWD2 = 1;
        PWD3 = 1;
        PWD4 = 1;
    end
    CheckPWD CPWD(Num1, Num2, Num3, Num4, PWD1, PWD2, PWD3, PWD4, CheckRes);
    always #100
    begin
        Num1 <= 1;
        Num2 <= 1;
        Num3 <= 1;
        Num4 <= 1;
        #100
        Num1 <= 5;
        Num2 <= 5;
        Num3 <= 5;
        Num4 <= 5;
    end
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/13 11:31:06
// Design Name: 
// Module Name: sim_DisplayNum
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


module sim_DisplayNum(

    );
    reg CLK;
    wire [7:0]Code1;
    wire [7:0]Code2;
    wire [7:0]Code3;
    wire [7:0]Code4;
    wire [9:0]Code5;
    wire [9:0]Code6;
    wire [6:0]Figure1;
    wire [6:0]Figure2;
    wire [3:0]DN0;
    wire [3:0]DN1;
    initial
    begin 
        CLK = 0;
    end
    DisplayNum DN(CLK, Code1, Code2, Code3, Code4, Code5, Code6, Figure1, Figure2, DN0, DN1);
    always #1
        CLK = CLK + 1;

    assign Code1 = 1;
    assign Code2 = 1;
    assign Code3 = 1;
    assign Code4 = 1;
    assign Code5 = 1;
    assign Code6 = 1;
endmodule

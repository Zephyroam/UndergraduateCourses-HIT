`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/08 00:10:46
// Design Name: 
// Module Name: DisplayNum
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


module DisplayNum(
    CLK, Code1, Code2, Code3, Code4, Code5, Code6, Figure1, Figure2, DN0, DN1
    );
    input CLK;
    input [7:0]Code1;
    input [7:0]Code2;
    input [7:0]Code3;
    input [7:0]Code4;
    input [9:0]Code5;
    input [9:0]Code6;
    output [6:0]Figure1;
    output [6:0]Figure2;
    output [3:0]DN0;
    output [3:0]DN1;
    wire [6:0]Num1;
    wire [6:0]Num2;
    wire [6:0]Num3;
    wire [6:0]Num4;
    wire [6:0]Num5;
    wire [6:0]Num6;
    reg [6:0]Figure1;
    reg [6:0]Figure2;
    reg [3:0]DN0;
    reg [3:0]DN1;
    reg [31:0]Counter;
    initial
    begin
        Figure1 = 0;
        Figure2 = 0;
        DN0 = 4'b0001;
        DN1 = 4'b0100;
        Counter = 0;
    end
    TranslateCode2Num_8 T1(CLK, Code1, Num1);
    TranslateCode2Num_8 T2(CLK, Code2, Num2);
    TranslateCode2Num_8 T3(CLK, Code3, Num3);
    TranslateCode2Num_8 T4(CLK, Code4, Num4);
    TranslateCode2Num_10 T5(CLK, Code5, Num5);
    TranslateCode2Num_10 T6(CLK, Code6, Num6);

    always@(posedge CLK)
        Counter = Counter + 1;
    
    always@(posedge Counter[16])
    begin
        DN0 = DN0 << 1;
        if(DN0 == 0)
            DN0 = 4'b0001;
        if(DN0 == 4'b0001)
            Figure1 = Num1;
        if(DN0 == 4'b0010)
            Figure1 = Num2;
        if(DN0 == 4'b0100)
            Figure1 = Num3;
        if(DN0 == 4'b1000)
            Figure1 = Num4;
    end
    
    always@(posedge Counter[16])
    begin
        if(DN1 == 4'b0100)
        begin
            Figure2 = Num5;
            DN1 = 4'b1000;
        end
        else if(DN1 == 4'b1000)
        begin
            Figure2 = Num6;
            DN1 = 4'b0100;
        end
    end
endmodule

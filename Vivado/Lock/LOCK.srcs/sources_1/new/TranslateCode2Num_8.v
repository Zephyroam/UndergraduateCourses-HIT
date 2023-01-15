`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/09 18:08:27
// Design Name: 
// Module Name: TranslateCode2Num_8
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


module TranslateCode2Num_8(
    CLK,
    Code,
    Num
    );
    input CLK;
    input [7:0]Code;
    output [6:0]Num;
    reg [6:0]Num;
    initial
    begin
        Num = 0;
    end
    always@(posedge CLK)
    begin
        if(Code == 8'b00_000_001)
            Num = 7'b1111110;
        else if(Code == 8'b00_000_010)
            Num = 7'b0110000;
        else if(Code == 8'b00_000_100)
            Num = 7'b1101101;
        else if(Code == 8'b00_001_000)
            Num = 7'b1111001;
        else if(Code == 8'b00_010_000)
            Num = 7'b0110011;
        else if(Code == 8'b00_100_000)
            Num = 7'b1011011;
        else if(Code == 8'b01_000_000)
            Num = 7'b1011111;
        else if(Code == 8'b10_000_000)
            Num = 7'b1110000;
        else if(Code == 8'b00_000_000)
            Num = 7'b0000001;
    end
endmodule
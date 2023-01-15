`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/15 20:17:41
// Design Name: 
// Module Name: sim_Trans
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


module sim_Trans(

    );
    reg CLK;
    reg [9:0]Code;
    wire [6:0]Num;
    TranslateCode2Num_10 T10(CLK,Code,Num);
    initial
    begin
        CLK = 0;
        Code = 0;
    end
    always #1
        CLK = CLK + 1;
    always #5
    begin
        Code = 10'b0_000_000_001;
        #5 Code = 10'b0_000_000_010;
        #5 Code = 10'b0_000_000_100;
        #5 Code = 10'b0_000_001_000;
        #5 Code = 10'b0_000_010_000;
        #5 Code = 10'b0_000_100_000;
        #5 Code = 10'b0_001_000_000;
        #5 Code = 10'b0_010_000_000;
        #5 Code = 10'b0_100_000_000;
        #5 Code = 10'b1_000_000_000;
        #5 Code = 10'b0_000_000_000;
    end
endmodule

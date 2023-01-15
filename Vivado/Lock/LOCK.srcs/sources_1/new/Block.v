`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/10 20:43:30
// Design Name: 
// Module Name: Block
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


module Block(
    CLK_1s_CLK, CLK_1ms_CLK, Block_Start, BlockTimeUp, BlockTime
    );
    input CLK_1s_CLK, CLK_1ms_CLK, Block_Start;//
    output BlockTimeUp;
    output BlockTime;
    reg BlockTimeUp;
    reg [4:0]BlockTimer;
    reg [9:0]BlockTime;
    reg [4:0]BlockTime_Ori;
    initial
    begin
        BlockTimer = 0;
        BlockTimeUp = 0;
        BlockTime = 0;
        BlockTime_Ori = 0;
    end
    always@(posedge CLK_1ms_CLK)
        BlockTime_Ori = 5 - BlockTimer;
    always@(negedge CLK_1ms_CLK)
    begin
        if(BlockTime_Ori == 0)
            BlockTime = 10'b0_000_000_001;
        else if(BlockTime_Ori == 1)
            BlockTime = 10'b0_000_000_010;
        else if(BlockTime_Ori == 2)
            BlockTime = 10'b0_000_000_100;
        else if(BlockTime_Ori == 3)
            BlockTime = 10'b0_000_001_000;
        else if(BlockTime_Ori == 4)
            BlockTime = 10'b0_000_010_000;
        else if(BlockTime_Ori == 5)
            BlockTime = 10'b0_000_100_000;
    end
    always@(posedge CLK_1s_CLK)
    begin
        if(BlockTimer == 5)
        begin
            BlockTimeUp = 1;
        end
        if(Block_Start)
        begin
            BlockTimer = BlockTimer + 1;
        end
        else
        begin
            BlockTimeUp = 0;
            BlockTimer = 0;
        end
    end
endmodule

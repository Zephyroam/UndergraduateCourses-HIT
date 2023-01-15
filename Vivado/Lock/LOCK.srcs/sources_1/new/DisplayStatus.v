`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/12 17:21:47
// Design Name: 
// Module Name: DisplayStatus
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


module DisplayStatus(
    CLK_1ms_CLK, ChangePWDMode, SecurityMode_Start, Open, Status1, Status2
    );
    input CLK_1ms_CLK, ChangePWDMode, SecurityMode_Start, Open;
    output Status1, Status2;
    reg Status1, Status2;
    initial
    begin
        Status1 = 0;
        Status2 = 0;
    end
    always@(posedge CLK_1ms_CLK)
    begin
        if(ChangePWDMode)
        begin
            Status1 <= 1;
            Status2 <= 1;
        end
        else if(SecurityMode_Start)
        begin
            Status1 <= 0;
            Status2 <= 0;
        end
        else if(Open)
        begin
            Status1 <= 1;
            Status2 <= 0;
        end
        else if(!Open)
        begin
            Status1 <= 0;
            Status2 <= 1;
        end
    end
endmodule

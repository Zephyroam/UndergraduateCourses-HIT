`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/08 18:32:00
// Design Name: 
// Module Name: sim_lock
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


module sim_lock(
    );
    reg [7:0]Input;
    wire [6:0]Figure1;
    wire [6:0]Figure2;
    wire [3:0]DN0;
    wire [3:0]DN1;
    reg CLK, ChangeButton, LockButton;
    wire Status1, Status2, Alarm_Out;
    Lock L(CLK, Input, ChangeButton, LockButton, Figure1, Figure2, DN0, DN1, Status1, Status2, Alarm_Out);
    initial
        begin
            Input = 0;
            CLK = 0;
        end
    always #1
        CLK = CLK + 1;
    always #200000000
    begin
        LockButton = 1;
        #200000000 LockButton = 0;
        #200000000 Input = 8'b100_000_00;
        #200000000 Input = 8'b000_000_00;
        #200000000 Input = 8'b000_000_00;
    end
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/12/08 15:40:20
// Design Name: 
// Module Name: Lock
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


module Lock(
    CLK, Input, ChangeButton, LockButton, Figure1, Figure2, DN0, DN1, Status1, Status2, Alarm_Out
    );
    input CLK;
    input [7:0]Input;
    input LockButton;
    input ChangeButton;
    output [6:0]Figure1;
    output [6:0]Figure2;
    output [3:0]DN0;
    output [3:0]DN1;
    output Status1, Status2, Alarm_Out;
    reg [7:0]Num1;
    reg [7:0]Num2;
    reg [7:0]Num3;
    reg [7:0]Num4;
    reg [9:0]Num5;
    reg [9:0]Num6;
    reg [3:0]Epoch;
    reg [7:0]Base;
    reg [7:0]PWD1;
    reg [7:0]PWD2;
    reg [7:0]PWD3;
    reg [7:0]PWD4;
    reg [3:0]SecurityCounter;
    reg Open, ToLow, ChangePWDMode, Determine2Open, Changed, SecurityMode_Start, Block_Start, Alarm_Start;
    wire CLK_1ms_CLK, CheckRes, CLK_1s_CLK, BlockTimeUp, SecurityModeTimeUp;
    wire [9:0]BlockTime;
    wire [9:0]SecurityModeTime1;
    wire [9:0]SecurityModeTime2; 
    Clock_1ms CLK1ms(CLK, CLK_1ms_CLK);
    Clock_1s CLK1s(CLK, CLK_1s_CLK);
    DisplayNum DpNm(CLK, Num1, Num2, Num3, Num4, Num5, Num6, Figure1, Figure2, DN0, DN1);
    CheckPWD CPWD(Num1, Num2, Num3, Num4, PWD1, PWD2, PWD3, PWD4, CheckRes);
    Block Bl(CLK_1s_CLK, CLK_1ms_CLK, Block_Start, BlockTimeUp, BlockTime);
    Alarm Al(CLK, Alarm_Start, Alarm_Out);
    SecurityMode SeM(CLK_1s_CLK, CLK_1ms_CLK, SecurityMode_Start, SecurityModeTimeUp, SecurityModeTime1, SecurityModeTime2);
    DisplayStatus DS(CLK_1ms_CLK, ChangePWDMode, SecurityMode_Start, Open, Status1, Status2);
    initial
    begin
        Num1 = 0;
        Num2 = 0;
        Num3 = 0;
        Num4 = 0;
        Num5 = 0;
        Num6 = 0;
        Epoch = 0;
        Changed = 0;
        Base = 0;
        Open = 1;
        PWD1 = 1;
        PWD2 = 1;
        PWD3 = 1;
        PWD4 = 1;
        SecurityCounter = 0;
        ChangePWDMode = 0;
        SecurityMode_Start = 0;
        ToLow = 0;
        Determine2Open = 0;
        Block_Start = 0;
        Alarm_Start = 0;
    end
    always@(negedge CLK_1ms_CLK)
    begin
        if(Input - Base > 0)
            Changed = 1;
        else 
        begin
           Changed = 0;
        end
    end
    always@(posedge CLK_1ms_CLK)
    begin
        if(Alarm_Start)
            Alarm_Start = 0;
        
        if(SecurityCounter == 3)
        begin
            SecurityCounter = 0;
            SecurityMode_Start = 1;
            Alarm_Start = 1;
        end
        if(SecurityMode_Start && SecurityModeTimeUp)
        begin
            SecurityMode_Start = 0;
        end
        if(Determine2Open && !SecurityMode_Start)
        begin
            if(CheckRes)
            begin
                Open = 1;
                SecurityCounter = 0;
            end
            else 
            begin
                Alarm_Start = 1;
                SecurityCounter = SecurityCounter + 1;
            end
            Determine2Open = 0;
        end
        if(Block_Start && BlockTimeUp && !SecurityMode_Start)
        begin
            Block_Start = 0;
            SecurityCounter = SecurityCounter + 1;
            Epoch = 0;
            Alarm_Start = 1;
        end
        if(!Changed)
            ToLow = 0; 
        if(Changed && !ToLow && !ChangePWDMode && !SecurityMode_Start)
        begin
            if(!Open)
            begin
                if(Epoch == 0)
                begin
                    Num1 <= Input;
                    Block_Start = 1;
                end
                else if(Epoch == 1)
                    Num2 <= Input;
                else if(Epoch == 2)
                    Num3 <= Input;
                else if(Epoch == 3)
                    Num4 <= Input;
                Epoch = Epoch + 1;
                ToLow = 1;
                if(Epoch == 4)
                begin
                    Epoch = 0;
                    Determine2Open = 1;
                    Block_Start = 0;
                end
            end
        end
        else if(Changed && !ToLow && ChangePWDMode && !SecurityMode_Start)
        begin
            if(Epoch == 0)
            begin
                Num1 <= Input;
                PWD1 <= Input;
            end
            else if(Epoch == 1)
            begin
                Num2 <= Input;
                PWD2 <= Input;
            end
            else if(Epoch == 2)
            begin
                Num3 <= Input;
                PWD3 <= Input;
            end
            else if(Epoch == 3)
            begin
                Num4 <= Input;
                PWD4 <= Input;
            end
            Epoch = Epoch + 1;
            ToLow = 1;
            if(Epoch == 4)
            begin
                Epoch = 0;
                ChangePWDMode = 0;
            end
        end
        else if(LockButton && !SecurityMode_Start)
            Open = 0;
        else if(CheckRes && Open && ChangeButton && !SecurityMode_Start)
            ChangePWDMode = 1;
        
    end
    always@(posedge CLK_1ms_CLK)
    begin
        if(!SecurityMode_Start)
        begin
            Num6 <= 1 << SecurityCounter;
            Num5 <= BlockTime;
        end
        else if(SecurityMode_Start)
        begin
            Num5 = SecurityModeTime2;
            Num6 = SecurityModeTime1;
        end
    end
endmodule

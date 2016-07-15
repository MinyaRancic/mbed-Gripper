classdef mbedGripper < matlab.mixin.SetGet
    %MBEDGRIPPER Summary of this class goes here
    %   Detailed explanation goes here
    
    properties(SetAccess = 'public', GetAccess = 'public')
        Angle
        mbedSerial
        kP
        kI
        kD
    end
    
    methods(Access = 'public')
        function obj = mbedGripper(ComPort)
            obj.mbedSerial = serial(ComPort);
            set(obj.mbedSerial, 'BaudRate', 9600);
            set(obj.mbedSerial, 'DataBits', 8);
            set(obj.mbedSerial, 'Parity', 'even');
            set(obj.mbedSerial, 'StopBits', 1);
            set(obj.mbedSerial, 'Terminator', 'LF');
            fopen(obj.mbedSerial);
        end
    end
    
    methods
        function Angle = get.Angle(obj)
            flushinput(obj.mbedSerial);
            fprintf(obj.mbedSerial, 'angl', 'async');
            Angle = fscanf(obj.mbedSerial);
        end
        
        function set.Angle(obj, value)
            flushoutput(obj.mbedSerial);
            fprintf(obj.mbedSerial, '%f', value);s
        end
    end
    
end


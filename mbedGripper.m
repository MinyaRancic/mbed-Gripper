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
            flushoutput(obj.mbedSerial);
            fprintf(obj.mbedSerial, '1,0,0\n', 'async');
            Angle = str2double(fscanf(obj.mbedSerial));
        end
        
        function set.Angle(obj, value)
            flushoutput(obj.mbedSerial);
            flushinput(obj.mbedSerial);
            fprintf(obj.mbedSerial, '0,0,%f\n', value, 'async');
        end
        
        function kP = get.kP(obj)
            flushoutput(obj.mbedSerial);
            flushinput(obj.mbedSerial);
            fprintf(obj.mbedSerial, '1,1,0\n', 'async');
        end
        
        function set.kP(obj, value)
            flushoutput(obj.mebedSerial);
            flushinput(obj.mbedSerial);
            fprintf(obj.mbedSerial, '0,1,%f\n', value, 'async');
        end
        
        function kI = get.kI(obj)
            flushoutput(obj.mbedSerial);
            flushinput(obj.mbedSerial);
            fprintf(obj.mbedSerial, '1,2,0\n', 'async');
        end
        
        function set.kI(obj, value)
            flushoutput(obj.mebedSerial);
            flushinput(obj.mbedSerial);
            fprintf(obj.mbedSerial, '0,2,%f\n', value, 'async');
        end
        
        function kD = get.kD(obj)
            flushoutput(obj.mbedSerial);
            flushinput(obj.mbedSerial);
            fprintf(obj.mbedSerial, '1,3,0\n', 'async');
        end
        
        function set.kD(obj, value)
            flushoutput(obj.mebedSerial);
            flushinput(obj.mbedSerial);
            fprintf(obj.mbedSerial, '0,3,%f\n', value, 'async');
        end
    end
    
end


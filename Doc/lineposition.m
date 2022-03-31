clc
clear
%% パラメータ
l1 = 76.19;         % タイロッドの軸間距離 L1[mm]
l2 = 86;            % サーボ出力軸からキングピンまでの距離L2[mm]
l3 = 17.69;         % キングピンからタイロッド軸までの距離 L3[mm]
l4 = 28.6;          % キングピンからホイールの中心までの距離L4[mm]
l5 = 17;             % サーボ出力軸からタイロッドまでの距離r[mm]
alfa = 106.1;       % 車軸とナクルアームのなす角[°]
W = 149;            % ホイールベースW[mm]
T = 143;            % トレッドT[mm]
sensorbar = 307;
maxDuty = 100;      % Duty比の最大値
maxAD = 1700;       % サーボ最大切れ角でのAD値[]
maxDeg = 42;        % サーボ最大切れ角[°]
ad2deg = maxDeg/maxAD;   % 1ad値あたりの角度

%% 旋回半径の算出
ad = 1500;
turningAngle = 20;

% servoDeg = ad * ad2deg * pi/180;        % AD値を[rad]に変換
servoDeg = 23 * pi/180;
turningAngle = turningAngle * pi/180; % 機体角度を[rad]に変換
alfa = alfa * pi/180;                   % [rad]に変換

xIn = ( (l2-l1)/2 ) + ( l5*sin(servoDeg) );
thetaIn1 = acos(xIn/l3);
thetaIn2 = pi-(thetaIn1 + alfa);
WIn = W - (l4 * sin(thetaIn2) );

r3 = WIn/sin(thetaIn2);
r1 = r3 * cos(thetaIn2);

%% 旋回後センサ位置の算出
theta1 = (pi/2) - servoDeg - turningAngle;
x1 = (r1+T/2)*cos(turningAngle) - ( W*sin(turningAngle) + sensorbar*cos(theta1) );
y1 = (r1+T/2)*sin(turningAngle) + W*cos(turningAngle) + sensorbar*sin(theta1);
theta2 = atan(abs(x1/y1));
r = y1/cos(theta2);
if x1 > 0
    theta3 = theta1 - theta2;
else
    theta3 = theta1 + theta2;
end
y2 = r * cos(theta3);
clc
clear
%% パラメータ
l1 = 76.19;         % タイロッドの軸間距離 L1[mm]
l2 = 86;            % サーボ出力軸からキングピンまでの距離L2[mm]
l3 = 17.69;         % キングピンからタイロッド軸までの距離 L3[mm]
l4 = 28.6;          % キングピンからホイールの中心までの距離L4[mm]
r = 17;             % サーボ出力軸からタイロッドまでの距離r[mm]
alfa = 106.1;       % 車軸とナクルアームのなす角[°]
W = 149;            % ホイールベースW[mm]
T = 143;            % トレッドT[mm]
maxduty = 100;      % Duty比の最大値
maxad = 1750;       % サーボ最大切れ角でのAD値[]
maxdeg = 42;        % サーボ最大切れ角[°]
ad2deg = maxdeg/maxad;   % 1ad値あたりの角度

%% 旋回半径の算出
cnt = 0;
fprintf("	const char rev_difference_D[] = {       // 角度から内輪、外輪回転差計算\n");
fprintf("		");
for ad = 0:maxad*2
    if mod(ad,maxdeg) == 0
        servodeg = ad * ad2deg;     % AD値を°に変換
        xin = ( (l2-l1)/2 ) + ( r*sind(servodeg) );
        xout = ( (l2-l1)/2 ) - ( r*sind(servodeg) );

        thetain1 = acosd(xin/l3);
        thetaout1 = acosd(xin/l3);

        thetain2 = 180-(thetain1 + alfa);
        thetaout2 = 180-(thetaout1+alfa);

        Win = W - (l4 * sind(thetain2) );
        Wout = W + (l4 * sind(thetaout2) );

        r3 = Win/sind(thetain2);
        r1 = r3 * cosd(thetain2);
        r4 = Wout/sind(thetaout2);
        r2 = r4 * cosd(thetaout2);

        r1 = round(maxduty * (r1/r4));
        r2 = round(maxduty * (r2/r4));
        r3 = round(maxduty * (r3/r4));

        fprintf("%d,%d,%d,",r1,r2,r3);
        cnt = cnt+1;
        if cnt == 5
          cnt = 0;
          fprintf("\n");
          fprintf("		");
        end
        if cnt > maxdeg
            break;
        end
        
    end
end
fprintf("\n		};");
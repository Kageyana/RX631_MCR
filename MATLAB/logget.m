clear
clc
close all
%% 定数設定
TIMEOUT = 5;        % タイムアウトまでの時間[s]
num_data = 7;       % データ数
XPOINTS = 300;      % x軸の最大値
%% データ保存用行列生成
values = zeros(num_data,'double');
x = zeros(XPOINTS,num_data);
for i = 1:num_data
    x(:,i) = (1:XPOINTS)';
end
xlabels = (1:XPOINTS);
y = zeros(XPOINTS,num_data);
data = zeros(10000,num_data);

%% 保存ファイル名生成
for i=1:100
    list = dir(num2str(i)+".mat");
    name = i;
    if size(list,1) == 0
        break
    end
end
%% 
try 
    %% COM設定
    %serialオブジェクト生成
    mbed = serial('COM4', ...
                  'BaudRate', 230400, ...
                  'Parity', 'none', ...
                  'DataBits', 8, ...
                  'StopBits', 1);       %通信フォーマットを設定
    
    set(mbed,'Timeout',TIMEOUT);        %タイムアウト時間を設定
    fopen(mbed);        % 指定したポートを開く
    
    %% メイン処理
    while(1)
        str = input('Please press s\n','s');
        if str == 's'
            fprintf(mbed,'%c',str);       % EPOS4と接続
            break;
        end
    end
    
    str = input('Please speed\n','s');
    if str=="0"
        fprintf(mbed,'%c',str);                  % 低速信号
        str = input('Please low speed\n','s');
        fprintf(mbed,'%c',str);                  % 回転数信号
    else
        fprintf(mbed,'%c',str);                  % 回転数信号
    end
    
    
    hf=figure('position',[60 30 50 50],'menubar','none');   % 停止用画面生成
    fprintf("start\n")
    position = 1;       % グラフ初期値を設定
    time = 1;           % プロットカウント初期値
    %% データ取得
    while (1)       %ループ開始
        values = fscanf(mbed, '%f,%f,%f,%f,%f,%f,%d');    %値を取得(カンマ区切り)
        y(position,:) = values';
        data(time,:) = values';         % データ保存
        
        if (position < XPOINTS)         %  x軸位置更新
            position = position + 1;
        else
            position = 1;
        end
 
%         if rem(time, 40) == 0       % 40サンプルごとにプロットを表示
%             plot(x,y)
%         end
        % 'q'押下で通信終了(Figure維持)
        if strcmp(get(hf,'currentcharacter'),'q')
            str = 'q';
            for j=1:10  
                fprintf(mbed,'%c','q'); % 停止信号
            end
            close(hf)
            fclose(mbed);
            break
        end
        
%         disp(time)
        
        time = time + 1;
        drawnow;    %figureを更新
        
    end
    fclose(mbed);   %COMポートを閉じる
    fprintf('end\n')
    save(num2str(name)) % ワークスペースを保存
catch
    % エラーが発生したらCOMポートを閉じる
    fprintf('Failed!\n')
    close(hf)
    fclose(mbed);   %COMポートを閉じる
%     save(num2str(name)+"error") % ワークスペースを保存
    save(num2str(name)) % ワークスペースを保存
end 
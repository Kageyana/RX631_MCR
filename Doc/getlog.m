clear
clc
close all
%% 定数設定
TIMEOUT = 5;        % タイムアウトまでの時間[s]
num_data = 7;       % データ数
XPOINTS = 300;      % x軸の最大値
%% データ保存用行列生成
values = zeros(num_data,'double');
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
    port = serial('COM4', ...
                  'BaudRate', 230400, ...
                  'Parity', 'none', ...
                  'DataBits', 8, ...
                  'StopBits', 1);       %通信フォーマットを設定
    
    set(port,'Timeout',TIMEOUT);        %タイムアウト時間を設定
    fopen(port);        % 指定したポートを開く
    
    %% メイン処理
    fprintf(port,'start');       % ログ送信開始
    
    hf=figure('position',[60 30 50 50],'menubar','none');   % 停止用画面生成
    fprintf("start\n")
    time = 1;           % プロットカウント初期値
    %% データ取得
    while (1)       %ループ開始
        logstr = [  "%5d,";
                    "%5d,";
                    "%5d,";
                    "%5d,";
                    "%5d,";
                    "%5d,";
                    "%5d,";
                    ];
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");

        fscanf(port, "%4.2f,");
        fscanf(port, "%4.4f,");
        fscanf(port, "%4.4f,");
        fscanf(port, "%4.4f,");
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");
        fscanf(port, "%5d,");
        fscanf(port, "%2.2f,");
        fscanf(port, "%4d,");

        printf(port, "%6d,");
        printf(port, "%6d,");
        printf(port, "%6d");
        values = fscanf(port, '%f,%f,%f,%f,%f,%f,%d');    %値を取得(カンマ区切り)
        data(time,:) = values';         % データ保存
        
        % 'q'押下で通信終了(Figure維持)
        if strcmp(get(hf,'currentcharacter'),'q')
            str = 'q';
            for j=1:10  
                fprintf(port,'%c','q'); % 停止信号
            end
            close(hf)
            fclose(port);
            break
        end
       
        time = time + 1;
        
    end
    fclose(port);   %COMポートを閉じる
    fprintf('end\n')
    save(num2str(name)) % ワークスペースを保存
catch
    % エラーが発生したらCOMポートを閉じる
    fprintf('Failed!\n')
    close(hf)
    fclose(port);   %COMポートを閉じる
%     save(num2str(name)+"error") % ワークスペースを保存
    save(num2str(name)) % ワークスペースを保存
end 
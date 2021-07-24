delete(instrfindall); %limpa todos os instrumentos anteriormente abertos na serial
clear all;
close all;
clc;

s = serial('COM3');             %porta serial

set(s, 'InputBufferSize', 16);  %numeros de bits no buffer
set(s, 'FlowControl', 'hardware');
set(s, 'BaudRate', 9600);
set(s, 'Parity', 'none');
s.ReadAsyncMode = 'continuous';

set(s, 'DataBits', 8);
set(s, 'StopBit', 1);
set(s, 'Timeout',0.005);       %tempo em segundos que o matlab aguarda por
%novos dados. Deve "casar" com o do Arduino

disp(get(s,'Name'));           %mostra o nome "Serial -COM3"
prop(1)=(get(s,'BaudRate'));   %prop é apenas um vetor de strings que nesse
%caso tem 4 posições
prop(2)=(get(s,'DataBits'));
prop(3)=(get(s, 'StopBit'));
prop(4)=(get(s, 'InputBufferSize'));

disp(['Port Setup Done!!',num2str(prop)]); %help num2str: converte numeros
%para uma string

fopen(s);                   %abre a porta serial
t=1;
disp('Running');
valor = zeros(1,4);
x = zeros(1,2000);
y = zeros(4,2000);

fprintf('\nAguardando 1s para setar o zero.')
pause(1.0);                %gera uma pausa de 1 segundos
fprintf('\nFim da espera, iniciando coleta.')
fprintf('\n gravando arquivo.')
time = clock;
adress='C:\Users\rosan\Desktop\';  %endereço para salvar o arquivo
dia = num2str(time(3));
mes = num2str(time(2));
ano = num2str(time(1));
horas = num2str(time(4));
minutos = num2str(time(5));
nome_do_arquivo = strcat(adress,dia,'-',mes,'-',ano,'_',horas,'-',minutos,'.txt.');
id_do_arquivo = fopen(nome_do_arquivo,'wt'); % wt = write, refere-se à ação que se deseja fazer com o arquivo

%%
while(t <= 2000)             %Runs for 200 cycles - if you cant see the
    %symbol, it is "less than" sign. so while
    %(t less than 200)
    
    x(t)=t*0.005;            % multiplicando pela taxa de amostragem % x(t)
    
    a = fgetl(s);
    if(a~=-1)
        tf = strcmp(a,'0');
        if(~tf)
            fprintf('%s',a)
            fprintf(id_do_arquivo,'\t%5.3f\t%s\n',x(t), a);
        end
    end
    t=t+1;
    a=0;                     %limpa o buffer
    pause(0.005);            %pausa de 5 ms
    
end
fprintf('\nFim da coleta')
fclose(id_do_arquivo);
fprintf('\nArquivo gravado.\n\n')
fclose(s); %fecha a porta serial
%% Tratamento do arquivo

fid = fopen(nome_do_arquivo);
t=1;
novo_arquivo = fopen('C:\Users\rosan\Desktop\dados_tratados.txt', 'wt');
tline = fgets(fid);
while ischar(tline)
   % disp(tline);
    a = str2num(tline);
    if(length(a)==5)
        fprintf(novo_arquivo,'%s\n',tline);
        t=t+1;
    end
    tline = fgets(fid);
end
fclose(novo_arquivo);
fclose(fid);


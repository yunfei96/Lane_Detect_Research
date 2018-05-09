%% Machine Learning 
clear ; close all; clc

%% ======================= Plotting =======================
fprintf('Plotting Data ...\n')

data1 = load('data.txt');
x = data1(:, 1);
y = data1(:, 2);

plot(x,y,'o')
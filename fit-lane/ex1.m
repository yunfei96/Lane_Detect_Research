%% Machine Learning 
clear ; close all; clc

%% ======================= Plotting =======================
fprintf('Plotting Data ...\n')

data1 = load('data1.txt');
y1 = data1(:, 1); x1 = data1(:, 2);

data2 = load('data2.txt');
y2 = data2(:, 1); x2 = data2(:, 2);

data3 = load('data3.txt');
y3 = data3(:, 1); x3 = data3(:, 2);

data4 = load('data4.txt');
y4 = data4(:, 1); x4 = data4(:, 2);

% Plot Data
plot(x1, y1, 'r.', 'MarkerSize', 2); % Plot the data
ylabel('y coord'); % Set the y?axis label
xlabel('x coord'); % Set the x?axis label
hold on
plot(x2, y2, 'g.', 'MarkerSize', 2); % Plot the data
hold on
plot(x3, y3, 'b.', 'MarkerSize', 2); % Plot the data
hold on
plot(x4, y4, 'y.', 'MarkerSize', 2); % Plot the data
hold on
%
bigx = vertcat(x1, x2, x3, x4);
bigy = vertcat(y1, y2, y3, y4);
r1 = polyfit(bigx,bigy,2);
a1 = r1(1);
b1 = r1(2);
c1 = r1(3);
fx1 = min(x4):1:max(x1);
fy1 = a1.*fx1.^2+b1.*fx1+c1;
plot(fx1,fy1);
%
%r2 = polyfit(x2,y2,2);
%a2 = r2(1);
%b2 = r2(2);
%c2 = r2(3);
%fx2 = min(x2):1:max(x2);
%fy2 = a2.*fx2.^2+b2.*fx2+c2;
%plot(fx2,fy2);
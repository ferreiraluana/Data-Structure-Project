data1 = csvread('priority_queue.csv');
x1 = data(:,1);
y1 = data(:,2);
figure(1);
plot(x1, y1, 'b');

data2 = csvread('heap_queue.csv');
x2 = data(:,1);
y2 = data(:,2);
figure(2);
plot(x2, y2, 'r');
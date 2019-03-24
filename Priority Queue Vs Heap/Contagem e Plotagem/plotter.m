data1 = csvread('priority_queue.csv');
x1 = data1(:,1);
y1 = data1(:,2);
figure(1);
subplot(1,2,1);
plot(x1, y1, 'b');
title('Regular Priority Queue');

data2 = csvread('heap_queue.csv');
x2 = data2(:,1);
y2 = data2(:,2);
subplot(1,2,2);
plot(x2, y2, 'r');
title('Priority Queue with Heap');

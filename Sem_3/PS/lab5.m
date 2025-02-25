clc
clear all
x=[20*ones(1,2),21*ones(1,1),22*ones(1,3),23*ones(1,6), 24*ones(1,5), 25*ones(1,9), 26*ones(1,2), 27*ones(1,2)];
mean(x);
y=[75*ones(1,3),76*ones(1,2), 77*ones(1,2),78*ones(1,5),79*ones(1,8),80*ones(1,8),81,82];
mean(y);
a=var(x,1);
b=var(y,1);
cov(x,y,1);
sqrt(a);

% a)
X=[7,7,4,5,9,9,4,12,8,1,8,7,3,13,2,1,17,7,12,5,6,2,1,13,14,10,2,4,9,11,3,5,12,6,10,7];
n=length(X);
X_bar=mean(X);
sigma=5;
alpha=input("give significance lvl: ");

z1=norminv(1-alpha/2,0,1);
z2=norminv(alpha/2,0,1);
T_lower=X_bar-sigma/sqrt(length(X))*z1;
T_upper=X_bar-sigma/sqrt(length(X))*z2;
confidance_lvl=100*(1-alpha);

fprintf("the %f %% for the average number of files is (%f,%f)", confidance_lvl, T_lower,T_upper)

% b)
s=std(X);
t1=tinv(1-alpha/2,n-1);
t2=tinv(alpha/2,n-1);
T_lower1=X_bar-s/sqrt(n)*t1;
T_upper1=X_bar-s/sqrt(n)*t2;

% c)
% punctul 2 din fisier
% 



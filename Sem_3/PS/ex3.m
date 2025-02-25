% Nickel powders are used in coattings used to shield electronic equipment.
% A random sample is selected, and the sizes of nickel particles in each coating
% are recorded (they are assumed to be approximately normally distributed):

% 3.26, 1.89, 2.42, 2.03, 3.07, 2.95, 1.39, 3.06, 2.46, 3.35, 1.56, 1.79, 1.76, 3.82, 2.42, 2.96

% a. Find a 95% confidence interval for the average size of nicked particles

% significance lvl = 1-confidence lvl, so alpha is 0.05
alpha = 0.05;
x=[3.26, 1.89, 2.42, 2.03, 3.07, 2.95, 1.39, 3.06, 2.46, 3.35, 1.56, 1.79, 1.76, 3.82, 2.42, 2.96];

% sigma unknown
n=length(x);
xbar=mean(x);
sigma=std(x);
% average size means miu
% sigma is not known and the population is approximately normally distributed
% => second case 

% and now compute the quantiles referring to the T(n-1) distribution
t1 = tinv(1-alpha/2,n-1);
t2 = tinv(alpha/2,n-1);

% now compute the confidence limits
limit1 = xbar - sigma/sqrt(n)*t1;
limit2 = xbar - sigma/sqrt(n)*t2;

fprintf('The confidence interval for the average size of nickel particles is: (%6.3f,%6.3f)\n',limit1,limit2);

% At the 1% significance level, on average, do these nickel particles seem to 
% be smaller than 3?

% alpha = significance level
alpha = 0.01;
% average means miu

% H0 - null hypothesis
% H0: The nickle particles are equal to 3
% H0: miu = 3
% H1 - the research hypothesis
% H1: The nickle particles are smaller than 3
% H1: miu < 3
% => we perform a left-tailed test 
fprintf("We perform a left-tailed test for the population mean\n");

% we want to perform a test for the population mean, the second case, 
% with a large sample but sigma unknown => ttest aas TT in T(n-1)
miu = 3;

% ttest -> test for the mean of a population(theta=miu), the case where sigma is unknown
% INPUT : x = the data sample 
%         miu = the population mean
%         alpha = the significance level
%         tail = left
% OUTPUT: H = indicator which tells us if we reject or do not reject H_0
%         P = critical value of the test (p-value)
%         CI = confidence interval
%         statistics = TS_0 TS when theta = theta0. If TS_0 is in RR => reject H0.
[H,P,CI,statistics] = ttest(x, miu, 'alpha', alpha, 'tail', 'left');

% result of the test, h = 0, if H0 is NOT rejected,
% h = 1, if H0 IS rejected
fprintf('\n H is %d', H)
if H == 1
  fprintf('\n So the null hypothesis is rejected, \n')
  fprintf('i.e the data suggests that the nickle particles are smaller than 3\n')
else
  fprintf('\nSo the null hypothesis is not rejected,\n')
  fprintf('i.e. the data suggests that the nickle particles are not smaller than 3\n')
end  


% building the rejection region
% RR = (-inf, tt_{alpha}) since it's a left-tailed test
% tt_alpha is the quantile for the T(n-1) distribution

tt_alpha = tinv(alpha, n-1);
RR = [-inf, tt_alpha]; % vector with 2 positions

fprintf('\nThe rejection region is (%4.4f, %4.4f)\n', RR);
fprintf('The value of the test statistic z is %4.4f\n', statistics.tstat);
fprintf('The P-value of the test is %4.4f\n\n', P);
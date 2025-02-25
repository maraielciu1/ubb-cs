% practical test 1
% data set
x=[1001.7,975.0,978.3,988.3,978.7,988.9,1000.3,979.2,968.9,983.5,999.2,985.6];
n=length(x);

% a find a 95% confidence interval for the average velocity of shells of
% this type
fprintf("a)");

% confidence lvl is 0.95
% significance lvl = 1-confidence lvl, so alpha is 0.05
alpha = 0.05;
xbar=mean(x);
sigma=std(x);
% average velocity means miu
% sigma is not known and the population is approximately normally distributed
% => second case 

% and now compute the quantiles referring to the T(n-1) distribution
t1 = tinv(1-alpha/2,n-1);
t2 = tinv(alpha/2,n-1);

% now compute the confidence limits
limit1 = xbar - sigma/sqrt(n)*t1;
limit2 = xbar - sigma/sqrt(n)*t2;

fprintf('The confidence interval for the average velocity of shells of this type is: (%6.3f,%6.3f)\n',limit1,limit2);

% b at 1% significance level, does data suggest that, on average, the
% muzzles are faster than 995 m/s?
fprintf("b)");

% significance level 1% => alpha=0.01
alpha=0.01;
% average means miu

% H0 - null hypothesis (we have the equality hypothesis)
% H0: The mussles have the speed 995 m/s
% H0: miu = 995
% H1 - the research hypothesis
% H1: The mussles have the speed greater 995 m/s
% H1: miu > 995
% => we perform a right-tailed test 
fprintf("We perform a right-tailed test for the population mean\n");

% we want to perform a test for the population mean, the second case, 
% with a large sample but sigma unknown => ttest aas TT in T(n-1)
miu = 995;

% ttest -> test for the mean of a population(theta=miu), the case where sigma is unknown
% INPUT : x = the data sample 
%         miu = the population mean
%         alpha = the significance level
%         tail = right
% OUTPUT: H = indicator which tells us if we reject or do not reject H0
%         P = critical value of the test (p-value)
%         CI = confidence interval
%         statistics = TS_0 TS when theta = theta0. If TS_0 is in RR => reject H0.
[H,P,CI,STATS] = ttest(x, miu, 'alpha', alpha, 'tail', 'right');

% result of the test, h = 0, if H0 is NOT rejected,
% h = 1, if H0 IS rejected
fprintf('\n H is %d', H)
if H == 1
  fprintf('\n The null hypothesis is rejected, \n')
  fprintf('=>the data suggests that mussles move with a speed greater than 995 m/s\n')
else
  fprintf('\nThe null hypothesis is not rejected,\n')
  fprintf('=>the data suggests that mussles move with a speed not greater than 995 m/s\n')
end  


% building the rejection region
% RR = (tt_{alpha-1}, inf) since it's a right-tailed test
% for tt_alpha we use the student distribution T(n-1) tinv

tt_alpha = tinv(1-alpha, n-1);
RR = [tt_alpha, inf];

fprintf('\nThe rejection region is (%4.4f, %4.4f)\n', RR);
fprintf('The value of the test statistic z is %4.4f\n', STATS.tstat);
fprintf('The P-value of the test is %4.4f\n\n', P);

% A study is conducted to compare heat loss in glass pipes, versus steel
% pipes of the same size. Various liquids at identical starting temperature 
% are run through segments of each type and the heat loss (in C) is 
% measured. These data result (normality of the two populations is assumed):

%       !             Heat loss               !
% Steel ! 4.6 0.7 4.2 1.9 4.8 6.1 4.7 5.5 5.4 !
% Glass ! 2.5 1.3 2.0 1.8 2.7 3.2 3.0 3.5 3.4 !
% ---------------------------------------------

%a. At the 5% significance level, do the population variances seem to differ?

% data sample
x_steel=[4.6,0.7,4.2,1.9,4.8,6.1,4.7,5.5,5.4];
x_glass=[2.5,1.3,2.0,1.8,2.7,3.2,3.0,3.5,3.4];
n1=length(x_steel);
n2=length(x_glass);

%significance level alpha=5%=0.05
alpha=0.05;

fprintf('a)');

% a)
% consider H0 - null hypothesis
% H0: sigma_steel^2=sigma_glass^2 (equal variances)
% H1 - research hypothesis
% H1: sigma_steel^2!=sigma_glass^2 (the population variances seem to
% differ)
% => a two-tailed test
fprintf("We perform a two-tailed test for the ratio of two population variances\n");

% we want to perform a test for the ratio of two population variances
% => vartest2 as TT in F(n_1-1, n_2-1)

% vartest2 -> test for the ratio of two population variances
% INPUT : x1 = the first data sample 
%         x2 = the second data sample 
%         alpha = the significance level
% OUTPUT: H = indicator which tells us if we reject or do not reject H_0
%         P = critical value of the test (p-value)
%         CI = confidence interval
%         statistics = TS_0 TS when theta = theta0. If TS_0 is in RR => reject H0.

[H,P,CI,STATS]=vartest2(x_steel,x_glass,"alpha",alpha);

% result of the test, h = 0, if H0 is NOT rejected,
% h = 1, if H0 IS rejected
fprintf('\n H is %d', H)
if H == 1
  fprintf('\n So the null hypothesis is rejected, \n')
  fprintf('i.e the variances of the two populations seem to differ.\n')
else
  fprintf('\nSo the null hypothesis is not rejected,\n')
  fprintf('i.e. the variances of the two populations are equal.\n')
end  

% building the rejection region
% RR = (-inf, tt_{alpha/2}) U (tt_{1-alpha/2},inf) since it's a two-tailed test
% tt_alpha is the quantile for the F(n_1-1, n2_-1) distribution

tt_alpha1 = finv(alpha/2, n1-1, n2-1);
tt_alpha2 = finv(1-alpha/2, n1-1, n2-1);
RR1 = [-inf, tt_alpha1];
RR2 = [tt_alpha2, inf]; % vector with 2 positions

% print ZVAL, P and RR on the screen
fprintf('\nThe rejection region is (%4.4f, %4.4f) U (%4.4f, %4.4f)\n', RR1, RR2);
fprintf('The value of the test statistic z is %4.4f\n', STATS.fstat);
fprintf('The P-value of the test is %4.4f\n\n', P);

%b. At the same significance level does it seem that on average, steel pipes
% lose more heat than glass pipes?
fprintf('b)');

% b)
% H0 - null hypothesis
% H0: steel pipes lose the same amount of heats than glass pipes
% H0: miu_steel = miu_glass
% H1 - the research hypothesis
% H1: steel pipes lose more heat than glass pipes
% H1: miu_steel > miu_glass
% => we perform a right-tailed test 

fprintf("We perform a right-tailed test for the difference of two population means\n");

% We want to perform a test for the difference of two population means:
% since from a) we got that the devations differ, so enter the third case:
% ttest2 with TT in T(n)
[H1,P1,CI1,STATS1]=ttest2(x_steel,x_glass,'alpha',alpha,'tail','right');

fprintf('\n H is %d', H1)
if H1 == 1
  fprintf('\nSo the null hypothesis is rejected,\n');
  fprintf('i.e. the data suggests that steel pipes lose more heat than glass pipes.\n');
else
  fprintf('\nSo the null hypothesis is not rejected,\n');
  fprintf('i.e. the data suggests that steel pipes do not lose more heat than glass pipes.\n');
end

% building the rejection region
% RR = (tt_{1-alpha},inf) since it's a right-tailed test
% tt_alpha is the quantile for the T(n) distribution

% compute the sample variances
var1 = var(x_steel);
var2 = var(x_glass);

% compute c and n
c = (var1/n1)/(var1/n1+var2/n2);
n = 1/((c^2/(n1-1) + (1-c)^2/(n2-1)));

tt_alpha = tinv(1-alpha, n);
RR = [tt_alpha, inf];% vector with 2 positions

% print ZVAL, P and RR on the screen
fprintf('\nThe rejection region is (%4.4f, %4.4f)\n', RR);
fprintf('The value of the test statistic z is %4.4f\n', STATS1.tstat);
fprintf('The P-value of the test is %4.4f\n', P1);
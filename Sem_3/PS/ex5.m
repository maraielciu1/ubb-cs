% To reach the maximum efficiency in performing an assembling operation
% in a manufacturing plant, new employees are required to take a 1-month training
% corse. A new method was suggested, and the maager wants to compare the new
% method with the standard one, by looking at the lengths of time required for 
% employees to assemble a certain device. They are given below (and assumed
% normally distributed):

%          !       Assembling times        !
% Standard ! 46 37 39 48 47 44 35 31 44 37 !
% New      ! 35 33 31 35 34 30 27 32 31 31 !
% -------------------------------------------

% a. At the 5% significance level, do the population variances seem to differ?
x_standard=[46,37,39,48,47,44,35,31,44,37];
x_new=[35,33,31,35,34,30,30,27,32,31,31];

n1=length(x_standard);
n2=length(x_new);

%5% significance level = alpha
alpha=0.05;

% H0 - the null hypothesis
% H0 sigma_std^2=sigma_new^2
% H1 - the research hypothesis
% H1 sigma_std^2!=sigma_new^2
% => two-tailed test
[H,P,CI,STATS]=vartest2(x_standard,x_new,'Alpha',alpha);

fprintf('\n H is %d', H)
if H == 1
  fprintf('\n So the null hypothesis is rejected, \n')
  fprintf('i.e the variances of the two populations seem to differ.\n')
else
  fprintf('\nSo the null hypothesis is not rejected,\n')
  fprintf('i.e. the variances of the two populations are equal.\n')
end 

% RR - rejection region = (-inf, tt_{alpha/2})U(tt_{1-alpha/2},inf)
tt_alpha1=finv(alpha/2,n1-1,n2-1);
tt_alpha2=finv(1-alpha/2,n1-1,n2-1);
RR1 = [-inf, tt_alpha1];
RR2 = [tt_alpha2, inf]; % vector with 2 positions

% print ZVAL, P and RR on the screen
fprintf('\nThe rejection region is (%4.4f, %4.4f) U (%4.4f, %4.4f)\n', RR1, RR2);
fprintf('The value of the test statistic z is %4.4f\n', STATS.fstat);
fprintf('The P-value of the test is %4.4f\n\n', P);

% b. Find a 95% confidence interval for the difference of the average 
% assembling times.
% sigma in unknown and we know they are not equal
m1=mean(x_standard);
m2=mean(x_new);
var1=var(x_standard);
var2=var(x_new);

% find c and n
c=(var1/n1)/(var1/n1+var2/n2)
n=1/(c^2/(n1-1)+(1-c)^2/(n2-1))

% and now the quantiles referring to the T(n) distribution
q=tinv(1-alpha/2,n);

limit1 = m1 - m2 - q*sqrt((var1/n1)+(var2/n2));
limit2 = m1 - m2 + q*sqrt((var1/n1)+(var2/n2));

fprintf('The confidence interval for the difference of true means is: (%6.3f,%6.3f)\n',limit1, limit2);
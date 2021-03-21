/* 代码编写于2021年3月21日
 * 作者：张昊
 *
 * 本程序是使用串行的方式利用梯形法则对二次函数进行求积分
 * 
 */

#include <stdio.h>

double f(double x);  // 声明函数f
double Trap(double a, double b, int n, double h);  // 声明梯形法则函数Trap

int main() {
	double result;
	double a, b, h;
	int n;

	printf("请输入a, b, n:\n");
	scanf("%lf",&a);
	scanf("%lf",&b);
	scanf("%d",&n);

	h = (b-a)/n;
	result = Trap(a, b, n, h);

	printf("当n=%d时，从区间%f到%f估计值为%.15f。", n, a, b, result);
	return 0;
}

double Trap(double a, double b, int n, double h) {
	double result;
	int k;

	result = (f(a) + f(b))/2.0;
	for(k = 1; k<=n-1; k++) {
		result += f(a + k*h);
	}
	result = result * h;
	return result;
}

double f(double x) {
	return x*x;
}

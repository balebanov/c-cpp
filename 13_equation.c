#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[])
{
	double a, b, c, discr, root1, root2, realPart, imPart;

	printf("Введите коэффициенты a, b, c: ");
	scanf("%lf %lf %lf", &a, &b, &c);

	discr = b*b-4*a*c;
	if(discr > 0){
		root1 = (-b+sqrt(discr))/(2*a);
		root2 = (-b-sqrt(discr))/(2*a);
		printf("Корень 1 = %.2lf и корень 2 = %.2lf\n", root1, root2);
	} else if(discr == 0){
		root1 = root2 = -b/(2*a);
		printf("Единственный корень = %.2lf\n", root1);
	} else{
		realPart = -b/(2*a);
		imPart = sqrt(-discr)/(2*a);
		printf("Корень 1 = %.2lf+%.2lfi и корень 2 = %.2lf-%.2lfi\n", realPart, imPart, realPart, imPart);
	}

	return 0;
}
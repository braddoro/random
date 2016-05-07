/* Written by Mike Harvey, March 1997 */
/* Placed in the public domain */

#include <stdio.h>

#define LO -99
#define HI  99

int count[200];
int *cnt = count+100;
int i,d1,d2,d3,d4,d5,d6,d7,d8;

void clear()
{
	for (i=LO; i<=HI; i++) cnt[i]=0;
}
int dF(int x)
{
	switch (x) {
		case 1: return -1;
		case 2: return -1;
		case 3: return  0;
		case 4: return  0;
		case 5: return  1;
		case 6: return  1;
	}
	return 99;
}
int dX(int x)
{
	switch (x) {
		case 1: return -2;
		case 2: return -1;
		case 3: return  0;
		case 4: return  0;
		case 5: return  1;
		case 6: return  2;
	}
	return 99;
}

int dC(int x, int n, int width, int width2)
{
	if (x <= width2) return -2;
	if (x <= width) return -1;
	if (x > (n-width2)) return +2;
	if (x > (n-width)) return +1;
	return 0;
}

void do2d(int n, int width, int width2)
{
	int k;

	printf("2d%d = (",n);
	for (k=1; k<=n; k++) {
		if (k > 1) printf(",");
		if (k <= width2) printf("-2");
		else if (k <= width) printf("-1");
		else if (k > (n-width2)) printf("+2");
		else if (k > (n-width)) printf("+1");
		else printf("0");
	}
	printf("):\n\n");

	for (d1=1; d1<=n; d1++) {
	for (d2=1; d2<=n; d2++) {
		k = dC(d1,n,width,width2)
		  + dC(d2,n,width,width2);

		cnt[k]++;
	}
	}
}
void do3d(int n, int width, int width2)
{
	int k;

	printf("3d%d = (",n);
	for (k=1; k<=n; k++) {
		if (k > 1) printf(",");
		if (k <= width2) printf("-2");
		else if (k <= width) printf("-1");
		else if (k > (n-width2)) printf("+2");
		else if (k > (n-width)) printf("+1");
		else printf("0");
	}
	printf("):\n\n");

	for (d1=1; d1<=n; d1++) {
	for (d2=1; d2<=n; d2++) {
	for (d3=1; d3<=n; d3++) {
		k = dC(d1,n,width,width2)
		  + dC(d2,n,width,width2)
		  + dC(d3,n,width,width2);

		cnt[k]++;
	}
	}
	}
}
void do4d(int n, int width, int width2)
{
	int k;

	printf("4d%d = (",n);
	for (k=1; k<=n; k++) {
		if (k > 1) printf(",");
		if (k <= width2) printf("-2");
		else if (k <= width) printf("-1");
		else if (k > (n-width2)) printf("+2");
		else if (k > (n-width)) printf("+1");
		else printf("0");
	}
	printf("):\n\n");

	for (d1=1; d1<=n; d1++) {
	for (d2=1; d2<=n; d2++) {
	for (d3=1; d3<=n; d3++) {
	for (d4=1; d4<=n; d4++) {
		k = dC(d1,n,width,width2)
		  + dC(d2,n,width,width2)
		  + dC(d3,n,width,width2)
		  + dC(d4,n,width,width2);

		cnt[k]++;
	}
	}
	}
	}
}
void do5d(int n, int width, int width2)
{
	int k;

	printf("5d%d = (",n);
	for (k=1; k<=n; k++) {
		if (k > 1) printf(",");
		if (k <= width2) printf("-2");
		else if (k <= width) printf("-1");
		else if (k > (n-width2)) printf("+2");
		else if (k > (n-width)) printf("+1");
		else printf("0");
	}
	printf("):\n\n");

	for (d1=1; d1<=n; d1++) {
	for (d2=1; d2<=n; d2++) {
	for (d3=1; d3<=n; d3++) {
	for (d4=1; d4<=n; d4++) {
	for (d5=1; d5<=n; d5++) {
		k = dC(d1,n,width,width2)
		  + dC(d2,n,width,width2)
		  + dC(d3,n,width,width2)
		  + dC(d4,n,width,width2)
		  + dC(d5,n,width,width2);

		cnt[k]++;
	}
	}
	}
	}
	}
}
void do6d(int n, int width, int width2)
{
	int k;

	printf("6d%d = (",n);
	for (k=1; k<=n; k++) {
		if (k > 1) printf(",");
		if (k <= width2) printf("-2");
		else if (k <= width) printf("-1");
		else if (k > (n-width2)) printf("+2");
		else if (k > (n-width)) printf("+1");
		else printf("0");
	}
	printf("):\n\n");

	for (d1=1; d1<=n; d1++) {
	for (d2=1; d2<=n; d2++) {
	for (d3=1; d3<=n; d3++) {
	for (d4=1; d4<=n; d4++) {
	for (d5=1; d5<=n; d5++) {
	for (d6=1; d6<=n; d6++) {
		k = dC(d1,n,width,width2)
		  + dC(d2,n,width,width2)
		  + dC(d3,n,width,width2)
		  + dC(d4,n,width,width2)
		  + dC(d5,n,width,width2)
		  + dC(d6,n,width,width2);

		cnt[k]++;
	}
	}
	}	
	}
	}
	}
}
void do5dP()
{
	int n=3;
	int width=1;
	int k;

	printf("5dPadded\n\n");

	for (d1=-1; d1<=1; d1++) {
	for (d2=-1; d2<=1; d2++) {
	for (d3=-1; d3<=1; d3++) {
	for (d4=-1; d4<=1; d4++) {
	for (d5=-1; d5<=1; d5++) {
		k = d1 + d2 + d3 + d4 + d5;

		if (k < 0) k++;
		else if (k > 0) k--;
		cnt[k]++;
	}
	}
	}
	}
	}
}
void stat()
{
	int tot=0,cum=0;
	double chance,below,above;
	double low=0;
	double acc=0;
	double high=0;
	double exact=0;
	double not=0;

	for (i=LO; i<=HI; i++) if (cnt[i]) tot += cnt[i];

	printf("   n                   P(n)    P(n-)   P(n+)\n");
	printf("  ---                ------- ------- -------\n");
	for (i=LO; i<=HI; i++) {
		if (cnt[i]) {
			above = ((float)(tot-cum)) / ((float)tot);
			cum += cnt[i];
			chance = ((float)cnt[i]) / ((float)tot);
			below = ((float)cum) / ((float)tot);
			printf("  %2d  %6d/%-6d  %7.3f %7.3f %7.3f\n",
				i, cnt[i], tot,
				chance*100.0,
				below*100.0,
				above*100.0);

			if (i < -1) low += chance;
			else if (i > +1) high += chance;
			else acc += chance;
			if (i == 0) exact = chance;
			if (i != 0) not += chance;
		}
	}
	printf("\n");
	printf("  Chance of very low result ............... %7.3f\n",low*100);
	printf("  Chance of acceptable result (+/-1) ...... %7.3f\n",acc*100);
	printf("  Chance of very high result .............. %7.3f\n",high*100);
	printf("\n");
	printf("  Chance of hitting trait level exactly ... %7.3f\n",exact*100);
	printf("  Chance of missing trait level ........... %7.3f\n",not*100);
	printf("\n");
}

int main()
{
	// chance is 2/5  (40%)
	clear();
	do2d(20,3,1);
	stat();

	// chance is 2/5  (40%)
	clear();
	do2d(20,2,1);
	stat();

	// chance is 2/3  (67%)
	clear();
	do4d(3,1,0);
	stat();

	// chance is 2/5  (40%)
	clear();
	do2d(10,3,1);
	stat();

	// chance is 2/5  (40%)
	clear();
	do2d(10,2,1);
	stat();

	// chance is 2/3  (67%)
	clear();
	do3d(3,1,0);
	stat();

	// chance is 1/2  (50%)
	clear();
	do4d(8,2,0);
	stat();

	// chance is 2/5  (40%)
	clear();
	do4d(10,2,0);
	stat();

	// chance is 2/3  (67%)
	clear();
	do2d(3,1,0);
	stat();

	// chance is 1/3  (33%)
	clear();
	do4d(6,1,0);
	stat();

	clear();
	do5d(6,1,0);
	stat();

	clear();
	do6d(6,1,0);
	stat();

	// chance is 2/3  (67%)
	clear();
	do5dP();
	stat();

	// chance is 3/10 (30%)
	clear();
	do4d(20,3,0);
	stat();

	// chance is 1/4  (25%)
	clear();
	do4d(8,1,0);
	stat();

	// chance is 1/5  (20%)
	clear();
	do4d(10,1,0);
	stat();

	// chance is 1/6  (16%)
	clear();
	do4d(12,1,0);
	stat();

	// chance is 1/10 (10%)
	clear();
	do4d(20,1,0);
	stat();

	return 0;
}
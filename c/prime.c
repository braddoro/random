#include <stdlib.h>
#include <stdio.h>
int check (int);
main() {
    float pct;
    int  i = 0, c, n, val, cnt = 0;
    printf("Enter max\n");
    scanf("%d",&n);
    for(c=1;c<=n;c++){
        val = check(i);
        if(val == 0){
            cnt++;
            pct = (i/(float)n*100);
            printf("%i : %i : %.1f%%\n",i,cnt,pct);
        }
        i++;
    }
    printf("Max: %i\n", n);
    return 0;
}
int check(int v) {
   int r=0, c;
   for (c=2;c<=(v-1);c++){
       if (v%c==0){
          break;
       }
   }
   r=1;
   if (c==v){
      r=0;
   }
   return r;
}

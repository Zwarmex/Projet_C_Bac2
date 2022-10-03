#include <stdio.h>
#include <stdlib.h>

int main()
{
    int milli = 67890 , h, m, s;

	h = ((milli / 1000)/3600); 
	
	m = ((milli / 1000) -(3600*h))/60;
	
	s = ((milli / 1000) -(3600*h)-(m*60));

    if (h)
    {
        printf("%d hours %d minutes %d seconds.\n", h, m, s);
        
    }

    else if(m)
    {
        printf("%d minutes %d seconds.\n", m, s);
    }

    else if(s)
    {
        printf("%d seconds.\n", s);  
    }

    else
    {
        printf("%d milliseconds.\n", milli);
    }
    

    return 0;
}


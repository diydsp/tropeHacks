#include <stdio.h>


int main( int argc, char *argv [] )
{
    int8_t phase      [ 2 ] = {  0,  0 };
    int8_t phase_delta[ 4 ] = {  10,  10, 10, 20 };

    // R0 = init

    while( 1 )
    {    
        
        // R4 = update
        while( phase[ 0 ] >= 0 )     // phase wrap will muck it up!
        {
            printf("0, ");
            phase[ 0 ] += phase_delta[ 0 ];	
            printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          printf("\n");
        }
            
        while( phase[ 1 ] >= 0 )    {
          printf("1, ");
          //phase[ 1 ] -= 4;
          printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          printf("\n");
          phase[ 1 ] += phase_delta[ 1 ];
        }  
        while( phase[ 0 ] <= 0 )    
        {
          printf("2, ");
          phase[ 0 ] += phase_delta[ 2 ];
          //phase[ 0 ] -= 16;
          //printf(" [ %d ] ", phase_delta[ 2 ] );
          printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          printf("\n");
        }  
        while( phase[ 1 ] < 0 )    {
          printf("3, ");
          printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          phase[ 1 ] += phase_delta[ 3 ];
          printf("\n");
        }  
        //printf("ph0,1=( %d, %d )",phase[ 0] , phase[ 1] );
        //printf("\n");
    }



    return 0;
}
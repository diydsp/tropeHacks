#include <stdio.h>
#include <stdlib.h>
 

// R4 rotate
int8_t phase      [ 2 ] = {  0,  0 };
int8_t phase_delta[ 4 ] = {  10,  10, 10, 20 };
int8_t rot_count;

// R1 Note Select
int8_t note = 0;

// R7 Noise Embedder

// output

FILE *fi_out;
void txtout( char *str )
{
  //return;
  //fprintf( fi_out, str );
  fprintf( fi_out, "xxx\n" );
}

void init( void )
{
  FILE *fi_out =fopen("out.csv","w");
  if( fi_out== NULL )
  {
    printf("fopen failed.\n"); exit(-1);
  }
  else 
  {
  printf("%ld",(long)fi_out );
  }
}

void R1_Note_Select( void )
{
  int8_t scale[] = { 16, 17, 18, 19, 20, 22, 25,   // C - F#
                     24, 25, 27, 28, 30 };         // G - B
  static int8_t scale_idx = 0 ;
  scale_idx++;
  if(scale_idx>=12){ scale_idx=0; }
  int8_t val;
  val = scale[ scale_idx ];
  phase_delta[ 0 ] = val;
  phase_delta[ 1 ] = val;
  phase_delta[ 2 ] = val;
  phase_delta[ 3 ] = val;

}

void R4_Rotate( void )
{        
        // R4 = update
        while( phase[ 0 ] >= 0 )     // phase wrap will muck it up!
        {
          printf("0, ");
          phase[ 0 ] += phase_delta[ 0 ];	
          printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          printf("\n");
          txtout( "1" );
        }
            
        while( phase[ 1 ] >= 0 )    
        {
          printf("1, ");
          printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          printf("\n");
          phase[ 1 ] += phase_delta[ 1 ];
          txtout( "0" );
          
        }  
        while( phase[ 0 ] <= 0 )    
        {
          printf("2, ");
          phase[ 0 ] += phase_delta[ 2 ];
          printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          printf("\n");
          txtout( "1" );
        }  
        while( phase[ 1 ] < 0 )    {
          printf("3, ");
          printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          phase[ 1 ] += phase_delta[ 3 ];
          printf("\n");
          txtout( "0" );
        }  
        //printf("ph0,1=( %d, %d )",phase[ 0] , phase[ 1] );
        //printf("\n");

}


int main( int argc, char *argv [] )
{

  init();
  
    // R0 = init

    while( 1 )
    {    
      R1_Note_Select();

      rot_count = 2;
      while( rot_count > 0 )
      {
        R4_Rotate();
        rot_count--;
      }
      
    }


    return 0;
}
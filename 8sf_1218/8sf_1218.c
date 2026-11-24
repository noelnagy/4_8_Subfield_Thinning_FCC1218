/*******************************************************************************
*
*       Name:                8sf_1218.c
*       Author:              K. Palagyi
*       Date:                13 Dec, 2024
* 
*******************************************************************************/

/* include files */
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <math.h>
  #include <time.h>
  #include <unistd.h>
  
  #include "cputime.h"
  #include "8sf_1218.h"
  

/* global variables */
  int              mask[18];
  unsigned int     neighbours;
  unsigned int     countnei;
  unsigned int     deletednei_limit;
  unsigned int     twig_size;  
  unsigned char   *lut_simple;
  unsigned int     size_x;
  unsigned int     size_y;
  unsigned int     size_z;
  unsigned int     size_xy;
  unsigned int     size_xyz;
  analyze_hdr      hdr;
  unsigned char   *image;
  unsigned char   *pimage;  
  unsigned int     act_addr;  
  struct listcell *BorderList;
  struct listcell *SF_List[8];
      
  #include "readwrite_functions.c"
  #include "thinning_functions.c"
  #include "cputime.c"

/*===========================================================================
    function    m a i n
  ===========================================================================*/
int main(int argc, char *argv[])
  {
  char   out_file_name[300];
  double runTime, t1, t2;

  /**********************/
  /* PARAMETER CHECKING */
  /**********************/
    if ( argc != 4 )
      {
        printf("\n USAGE:                                                          ");
        printf("\n   %s inp_name deletednei_limit twig_size", argv[0]               );
        printf("\n   where: - inp_name          : name of the input image          ");
        printf("\n                                containing the image data        ");
	printf("\n                                (without extension)              ");
	printf("\n           - deletednei_limit : min. number of deleted endpoints ");
	printf("\n           - twig_size        : iterations of endpoint deletion  ");	
        printf("\n\n");
        exit(0);
      } /* endif */
    printf("\n%s %s %s %s\n", argv[0], argv[1], argv[2], argv[3] );
    printf("\n *** 8-subfield curve-thinning on the (12,18)_FCC pictures with endpoint re-checking ***\n");
    
  /************************/
  /* SET deletednei_limit */
  /************************/
    deletednei_limit = atoi(argv[2]);
    
  /************************/
  /* SET deletednei_limit */
  /************************/
    twig_size = atoi(argv[3]);
 
  /********************/
  /* READ INPUT IMAGE */
  /********************/
    read_image( argv[1] );
    
  /******************/
  /* SET 8 SUBFIELDS */
  /*******************/   
    set_8_subfields();
    
  /***************/
  /* READING LUT */
  /***************/
    init_lut();

  /************/  
  /* THINNING */
  /************/
    t1 = report_cpu( NULL, NULL );
    subfield8_curve_thinning_FCC();
    t2 = report_cpu( NULL, NULL );
    runTime += t2 -t1;
    printf("\n Running time: %f ms", runTime * 1000.0 );
    
  /********************/
  /* WRITE OUPUT IMAGE */
  /********************/
    strcpy(out_file_name, argv[1]);
    strcat(out_file_name,"_8sf_1218_");
    strcat(out_file_name,argv[2]);
    strcat(out_file_name,"_");
    strcat(out_file_name,argv[3]);        
    write_image( out_file_name );
  
  /********/  
  /* FREE */
  /********/
    free(lut_simple);
    free(image);
    free(pimage);    

    exit(0);
  };
/*===========================================================================
    end of function    m a i n
  ===========================================================================*/

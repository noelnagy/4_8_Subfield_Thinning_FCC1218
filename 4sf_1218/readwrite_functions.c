/*******************************************************************************
*
*       Name:                readwrite_functions.cpp
*       Author:              K. Palagyi
*       Date:                1 Sep, 2026
* 
*******************************************************************************/

/*========= function read_image =========*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      hdr
      size_x
      size_y
      size_z
      size_xy
      size_xyz
      image
----------------*/  
void read_image( char *inp_name )
{
  char file_name[300];
  FILE         *fp_inp_img;
  FILE         *fp_inp_hdr;
  short int     hdrshortint;
  unsigned int  i, j, p, num;
  short int     perm;
  
  /* open IMG */
    strcpy(file_name, inp_name);
    strcat(file_name, ".img");
    if ( (fp_inp_img = fopen(file_name, "rb")) == NULL)
      {
        printf("ERROR: Can't open the input image data: %s!\n", file_name);
        exit(1);
      }
      
   /* open HDR */
    strcpy(file_name, inp_name);
    strcat(file_name, ".hdr" );
    if ( (fp_inp_hdr = fopen(file_name, "rb")) == NULL)
      {
        printf("ERROR: Can't open the input header %s!\n", file_name);
        exit(1);
      }
    
  /* read HDR */    
    if ( fread(&hdr, sizeof(analyze_hdr), 1, fp_inp_hdr) != 1 )
      {
        printf("ERROR: Couldn't read input header\n");
        exit(1);
      }
    fclose(fp_inp_hdr);
    
  /* set perm */  
    perm = 0;
    if ( hdr.sizeof_hdr != sizeof(analyze_hdr) )
      perm = 1;
      
  /* binary ? */    
    hdrshortint = hdr.bits;
    if ( perm ) 
      hdrshortint = ( hdrshortint<<8 ) | ( (hdrshortint>>8) & 0x00FF );
    if ( hdrshortint !=8 )
      {
        printf("ERROR: Cannot handle %d depth in image data \n", hdrshortint);
        exit(-1);
      }
      
  /* set dimensions */
    hdrshortint = hdr.x_dim;
    if ( perm ) 
      hdrshortint = ( hdrshortint<<8 ) | ( (hdrshortint>>8) & 0x00FF );
    size_x = (unsigned int)hdrshortint;
    hdrshortint = hdr.y_dim;
    if ( perm ) 
      hdrshortint = ( hdrshortint<<8 ) | ( (hdrshortint>>8) & 0x00FF );
    size_y = (unsigned int)hdrshortint;
    hdrshortint = hdr.z_dim;
    if ( perm ) 
      hdrshortint = ( hdrshortint<<8 ) | ( (hdrshortint>>8) & 0x00FF );
    size_z = (unsigned int)hdrshortint;
    if ( size_z <= 1 )
      {
        printf("ERROR: The image data is not a real 3D one. Number of slices is %d \n", size_z);
        exit(-1);
      }  
     printf("\n Size of the input image: size_x x size_y x size_z = %d x %d x %d\n", size_x, size_y, size_z);
     
  /* add frame */
    size_x += 4;
    size_y += 4;
    size_z += 4;
      
  /* set slice- and image size */   
    size_xy  = size_x * size_y;
    size_xyz = size_xy * size_z;
    
  /* alloc image */
    image = (unsigned char *)malloc(size_xyz);
    if ( image == NULL )
      {
         printf("\n Alloc. error (image)");
         exit(0);
      }
      
  /* init image */
    for (i=0; i<size_xyz; i++)
      *(image +i) = 0;

  /* read image */ 
    p = 2*(size_xy + size_x + 1);
    for (i=2; i<size_z-2; i++)
      {
        for (j=2; j<size_y-2; j++)
          {
            if ( fread(image+p, 1, size_x-4, fp_inp_img) != (size_x-4) )
              {
                printf("ERROR: Couldn't read image data\n");
                exit(2);
              }
            p+=size_x;
          }
        p+=(4*size_x);
      }
    fclose(fp_inp_img);
    
  /* count object points */
    num=0;
    for (i=0; i<size_xyz; i++)
      if ( *(image +i) )
        num++;
    printf("\n Number of object points in the input picture: %d\n", num);
    
}  
/*========= end of function read_image =========*/



/*========= function write_image =========*/
/* -------------
  considered global variables:
      hdr
      size_x
      size_y
      size_z
      size_xy
      size_xyz
  changed global variables:
      image 
----------------*/  
void write_image( char *out_name )
{
  char          file_name[300];
  FILE         *fp_out_img;
  FILE         *fp_out_hdr;
  unsigned int  i, j, p, num;
 
    
  /* open IMG */
    strcpy(file_name, out_name);
    strcat(file_name, ".img");
    if ( (fp_out_img = fopen(file_name, "wb")) == NULL)
      {
        printf("ERROR: Can't open the output image data: %s!\n", file_name);
        exit(1);
      }
      
  /* open HDR */
    strcpy(file_name, out_name);
    strcat(file_name, ".hdr" );
    if ( (fp_out_hdr = fopen(file_name, "wb")) == NULL)
      {
        printf("ERROR: Can't open the input header %s!\n", file_name);
        exit(1);
      }
    
  /* write HDR */    
    if ( fwrite(&hdr, sizeof(analyze_hdr), 1, fp_out_hdr) != 1 )
      {
        printf("ERROR: Couldn't read input header\n");
        exit(1);
      }
    fclose(fp_out_hdr);
    
  /* set non-zero to 255 */
    num=0;
    for (i=0; i<size_xyz; i++)
      if ( *(image +i) )
        {
          *(image +i) = 0xFF;
          num++;
        }
    printf("\n Number of object points in the skeleton: %d\n", num);
    
  /* write image */
    p = 2*(size_xy + size_x + 1);
    for (i=2; i<size_z-2; i++)
      {
        for (j=2; j<size_y-2; j++)
          {
            if ( fwrite(image+p, 1, size_x-4, fp_out_img) != (size_x-4) )
              {
                printf("ERROR: Couldn't write image data\n");
                exit(2);
              }
            p+=size_x;
          }
        p+=(4*size_x);
      }
    fclose(fp_out_img);
    
}  
/*========= end of function write_image =========*/


/*========= function set_mask =========*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      mask 
----------------*/  
void set_mask( void )
  {
   mask[ 0] = 0x00000001;
   mask[ 1] = 0x00000002;
   mask[ 2] = 0x00000004;
   mask[ 3] = 0x00000008;
   mask[ 4] = 0x00000010;
   mask[ 5] = 0x00000020;
   mask[ 6] = 0x00000040;
   mask[ 7] = 0x00000080;
   mask[ 8] = 0x00000100;
   mask[ 9] = 0x00000200;
   mask[10] = 0x00000400;
   mask[11] = 0x00000800;
   mask[12] = 0x00001000;
   mask[13] = 0x00002000;
   mask[14] = 0x00004000;
   mask[15] = 0x00008000;
   mask[16] = 0x00010000;
   mask[17] = 0x00020000;
  }
/*========= end of function set_mask =========*/


/*============= function init_lut =============*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      lut_simple
      mask
----------------*/  
void init_lut( void )
{
  char  lutname [100];
  FILE  *lutfile;

  /* alloc lut_simple */
    lut_simple = (unsigned char *)malloc(262144);
    if ( lut_simple == NULL)
      {
        printf("\n Alloc error!!!\n");
        exit(1);
      }  /* end if */ 

  /* open lutfile */
    strcpy( lutname, "lut_fcc_18_12.dat");
    lutfile = fopen( lutname, "rb");
    if ( lutfile == NULL)
      {
        printf("\n\n file lut_fcc_18_12.dat is not found!!!\n");
        exit(1);
      }  /* end if */

  /* reading lutfile */
    if ( fread( lut_simple, 1, 262144, lutfile) != 262144 )
      {
         printf("ERROR: Couldn't read LUT\n");
         exit(1);
      }
      
    fclose(lutfile);

  /* setting masks */
    set_mask();

}
/*=========== end of function init_lut ===========*/


/*========= function set_4_subfields =========*/
void set_4_subfields( void )
{
  unsigned int  x, y, z;
  
 /* alloc pimage */
 pimage = (unsigned char *)malloc(size_xyz);
 if ( pimage == NULL )
   {
     printf("\n Alloc. error (pimage)");
     exit(0);
   }

/* ##### */   
 for (x=0; x<size_xyz; x++)
   *(pimage + x) = 9;
/* ##### */

  for (z=0; z<size_z; z++)
    {
      for (y=0; y<size_y; y++)
          {
	    for (x=0; x<size_x; x++)
              {
	        act_addr = x + y*size_x + z*size_xy;
	        if ( (x%2==0) && (y%2==0) && (z%2==0) )
	          *(pimage + act_addr) = 0;
	        if ( (x%2) && (y%2) && (z%2==0) )
	          *(pimage + act_addr) = 1;
	        if ( (x%2==0) && (y%2) && (z%2) )
	          *(pimage + act_addr) = 2;
	        if ( (x%2) && (y%2==0) && (z%2) )
	          *(pimage + act_addr) = 3;  
              }
          }
      }

/* ##### 
  for (z=0; z<size_z; z++)
    {
      for (y=0; y<size_y; y++)
          {
	    for (x=0; x<size_x; x++)
              {
	        act_addr = x + y*size_x + z*size_xy;
                printf(" %d - %3d", *(pimage + act_addr), *(image + act_addr));
              }
            printf("\n");
          }
       printf("\n\n");
     } 
##### */
     
}
/*=========== end of function set_4_subfields ===========*/


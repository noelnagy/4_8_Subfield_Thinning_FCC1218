/*******************************************************************************
*
*       Name:                4sf_1218.h
*       Author:              K. Palagyi
*       Date:                1 Sep, 2026
* 
*******************************************************************************/

/* types */
  typedef struct{
        int             sizeof_hdr;
        char            pad1[28];
        int             extents;
        char            pad2[2];
        char            regular;
        char            pad3;
        short int       dims;
        short int       x_dim;
        short int       y_dim;
        short int       z_dim;
        short int       t_dim;
        char            pad4[20];
        short int       datatype;
        short int       bits;
        char            pad5[6];
        float           x_size;
        float           y_size;
        float           z_size;
        char            pad6[48];
        int             glmax;
        int             glmin;
        char            descrip[80];
        char            pad7[120];
  } analyze_hdr;


struct listcell {
  unsigned int     address;
  unsigned int     oldneinum;
  struct listcell *next;
};

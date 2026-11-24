/*******************************************************************************
*
*       Name:                thinning_functions.cpp
*       Author:              K. Palagyi
*       Date:                01 Sep, 2026
*
*******************************************************************************/

/*========= function Init_BorderList =========*/
void Init_BorderList (void) {
unsigned int x,y,z;
unsigned int z_size_xy, y_size_x;
struct listcell *p;

  BorderList = NULL;
  
  for( z=1, z_size_xy=size_xy;
       z<size_z-1;
       z++, z_size_xy+=size_xy )
    {
      for( y=1, y_size_x=size_x;
           y<size_y-1;
           y++, y_size_x+=size_x )
        {
          for(x=1; x<size_x-1; x++)
	    {
	      act_addr = x + y_size_x + z_size_xy;	    
              if ( *(image + act_addr ) )
                {
                  if (  ( *(image + act_addr + 1 +   size_x             ) ==0 ) ||
                        ( *(image + act_addr + 1 -   size_x             ) ==0 ) ||
			( *(image + act_addr - 1 +   size_x             ) ==0 ) ||
                        ( *(image + act_addr - 1 -   size_x             ) ==0 ) ||
                        ( *(image + act_addr + 1            +   size_xy ) ==0 ) ||
                        ( *(image + act_addr + 1            -   size_xy ) ==0 ) ||
                        ( *(image + act_addr - 1            +   size_xy ) ==0 ) ||
                        ( *(image + act_addr - 1            -   size_xy ) ==0 ) ||
                        ( *(image + act_addr     +   size_x +   size_xy ) ==0 ) ||
                        ( *(image + act_addr     +   size_x -   size_xy ) ==0 ) ||
                        ( *(image + act_addr     -   size_x +   size_xy ) ==0 ) ||
                        ( *(image + act_addr     -   size_x -   size_xy ) ==0 ) ||
                        ( *(image + act_addr + 2            -   size_xy ) ==0 ) ||
                        ( *(image + act_addr - 2            -   size_xy ) ==0 ) ||
                        ( *(image + act_addr     + 2*size_x             ) ==0 ) ||
                        ( *(image + act_addr     - 2*size_x             ) ==0 ) ||
                        ( *(image + act_addr                + 2*size_xy ) ==0 ) ||
                        ( *(image + act_addr                - 2*size_xy ) ==0 )    )
                     {
                        *(image + act_addr) = 2;
			/* insert */
                        p = (struct listcell*)malloc(sizeof(struct listcell));
		        p->address = act_addr;
		        p->next    = BorderList;
		        BorderList     = p;
                     } /* endif */
                } /* endif */
            } /* endfor x */		
        } /* endfor y */
    } /* endfor z */

}
/*========= end of function Init_BorderList =========*/


/*========= function collect_18_neighbours ==========*/
/* *** (12,18)-simple <=> negated (18,12)-simple *** */
void collect_18_neighbours( void )			   
  {

    neighbours = 0x00000000;
    countnei   = 0;

    /*  0: -0- */
    if ( *(image + act_addr - 1            -   size_xy ) ==0 )
      neighbours |= mask[ 0];
    else  
      countnei++;
    /*  1: 0-- */
    if ( *(image + act_addr     -   size_x -   size_xy ) ==0 )
      neighbours |= mask[ 1];
    else  
      countnei++;
    /*  2: +0- */
    if ( *(image + act_addr + 1            -   size_xy ) ==0 )
      neighbours |= mask[ 2];
    else  
      countnei++;
    /*  3: 0+- */
    if ( *(image + act_addr     +   size_x -   size_xy ) ==0 )
      neighbours |= mask[ 3];
    else  
      countnei++;
    /*  4: -0+ */
    if ( *(image + act_addr - 1            +   size_xy ) ==0 )
      neighbours |= mask[ 4];
    else  
      countnei++;
    /*  5: 0-+ */
    if ( *(image + act_addr     -   size_x +   size_xy ) ==0 )
      neighbours |= mask[ 5];
    else  
      countnei++;
    /*  6: +0+ */
    if ( *(image + act_addr + 1            +   size_xy ) ==0 )
      neighbours |= mask[ 6];
    else  
      countnei++;
    /*  7: 0++ */
    if ( *(image + act_addr     +   size_x +   size_xy ) ==0 )
      neighbours |= mask[ 7];
    else  
      countnei++; 
    /*  8: -+0 */
    if ( *(image + act_addr - 1 +   size_x             ) ==0 )
      neighbours |= mask[ 8];
    else  
      countnei++;
    /*  9: --0 */ 
    if ( *(image + act_addr - 1 -   size_x             ) ==0 )
      neighbours |= mask[ 9];
    else  
      countnei++;
    /* 10: +-0 */
    if ( *(image + act_addr + 1 -   size_x             ) ==0 )
      neighbours |= mask[10];
    else  
      countnei++;
    /* 11: ++0 */
    if ( *(image + act_addr + 1 +   size_x             ) ==0 )
      neighbours |= mask[11];
    else  
      countnei++;

    /* *** endpoint detection - additional 6-neighbors omitted *** */
    /* 12: 00-2 */
    if ( *(image + act_addr                - 2*size_xy ) ==0 )
      neighbours |= mask[12];
    /* 13: -200 */
    if ( *(image + act_addr - 2                        ) ==0 )
      neighbours |= mask[13];
    /* 14: 0-20 */
    if ( *(image + act_addr      - 2*size_x            ) ==0 )
      neighbours |= mask[14];
    /* 15: +200 */
    if ( *(image + act_addr + 2                        ) ==0 )
      neighbours |= mask[15];
    /* 16: 0+20 */
    if ( *(image + act_addr      + 2*size_x            ) ==0 )
      neighbours |= mask[16];
    /* 17: 00+2 */
    if ( *(image + act_addr                + 2*size_xy ) ==0 )
      neighbours |= mask[17];

  }
/*========= end of function collect_18_neighbours =========*/


/*=========== function SpreadOutBorderPoints ===========*/
void SpreadOutBorderPoints( void ) {
struct listcell *p;
struct listcell *q;

SF_List[0] = NULL;
SF_List[1] = NULL;
SF_List[2] = NULL;
SF_List[3] = NULL;

/* scan BorderList */
for ( p=BorderList; p!=NULL; p=p->next )
  {
    act_addr     = p->address;
    collect_18_neighbours();
    p->oldneinum = countnei;
    q            = (struct listcell*)malloc(sizeof(struct listcell));
    q->address   = act_addr;    
    q->next      = SF_List[*(pimage+act_addr)];
    SF_List[*(pimage+act_addr)] = q;  
  }
			
}
/*=========== end of function SpreadOutBorderPoints ===========*/


/*========= function is_simple_and_nonend =========*/
int is_simple_and_nonend( void )	      
{
  collect_18_neighbours();
  return ( ( *(lut_simple + neighbours ) ) && (countnei != 1) );
}
/*========= end of function is_simple_and_nonend =========*/


/*========= function remains_simple_after_deletion =========*/
int remains_simple_after_deletion( void )	      
{
  unsigned int     newneighbours;
  /* act_addr and neighbours are known */

  if ( ( iter % 2 ) == 0 )
  {
  /* 12: 00-2 */
  if ( *(image + act_addr                - 2*size_xy ) )
    {
      newneighbours = neighbours | mask[12];
      if ( *(lut_simple + newneighbours ) == 0 )
        return(0);
    }
  /* 13: -200 */
  if ( *(image + act_addr - 2                        ) )
    {
      newneighbours = neighbours | mask[13];
      if ( *(lut_simple + newneighbours ) == 0 )
        return(0);
    }      
  /* 14: 0-20 */
  if ( *(image + act_addr      - 2*size_x            ) )
    {
      newneighbours = neighbours | mask[14];
      if ( *(lut_simple + newneighbours ) == 0 )
        return(0);
    }
  }
  
  if ( iter % 2 )
  {  
  /* 15: +200 */
  if ( *(image + act_addr + 2                        ) )
    {
      newneighbours = neighbours | mask[15];
      if ( *(lut_simple + newneighbours ) == 0 )
        return(0);
    }
  /* 16: 0+20 */
  if ( *(image + act_addr      + 2*size_x            ) )
    {
      newneighbours = neighbours | mask[16];
      if ( *(lut_simple + newneighbours ) == 0 )
        return(0);
    }
  /* 17: 00+2 */
  if ( *(image + act_addr                + 2*size_xy ) )
    {
      newneighbours = neighbours | mask[17];
      if ( *(lut_simple + newneighbours ) == 0 )
        return(0);
    }
  }

  return(1);
}
/*========= end of function remains_simple_after_deletion =========*/


/*=========== function SF_Deletion  ===========*/
unsigned int SF_Deletion ( int sf ) {
unsigned int     deleted_points;
int              deletable;
struct listcell *DeletableList;
struct listcell *p;
struct listcell *q;

/* scan SF_List[sf] and fill DeletableList */
DeletableList = NULL;
for ( p=SF_List[sf]; p!=NULL; p=p->next )
  {
    act_addr     = p->address;
    if ( is_simple_and_nonend() )
      {
        if ( remains_simple_after_deletion() )
          {
            /* insert DeletableList */
              q = (struct listcell*)malloc(sizeof(struct listcell));
	      q->address    = act_addr;
	      q->next       = DeletableList;
              DeletableList = q;
          }
      }
  }
  
/* deletion */
deleted_points = 0;
for ( p=DeletableList; p!=NULL; p=p->next )
  {
    act_addr = p->address;
    *(image + act_addr ) = 0;
    deleted_points ++;
  }  

/* empty SF_List[sf] */
  p = SF_List[sf];
  while( p )
    {
       p = p->next;
       free(SF_List[sf]);
       SF_List[sf] = p;
    }
  
/* empty DeletableList */
  p = DeletableList;
  while( p )
    {
       p = p->next;
       free(DeletableList);
       DeletableList = p;
    }

return(deleted_points);
}
/*=========== end of function SF_Deletion  ===========*/


/*=========== function UpdateBorderPoints ===========*/
int UpdateBorderPoints( void ) {
struct listcell *AddedList;
struct listcell *DelEndList;
struct listcell *p;
struct listcell *q;
struct listcell *prev;
struct listcell *act;
struct listcell *tmp;
unsigned int     curr_addr;
unsigned int     tmp_addr;
unsigned int     added_points;
unsigned int     deletion_needed;
unsigned int     i;
int              deleted_endpoints;

/***************************************/
/******* 1st endpoint re-checking ******/
/* scan BorderList - create DelEndList */
/***************************************/
deleted_endpoints = 0;
for ( i=0; i<twig_size; i++ )
  {
    DelEndList = NULL;
    for ( act=BorderList; act!=NULL; act=act->next )
      {
           act_addr = act->address;
           collect_18_neighbours();
           if ( ( *(image + act_addr)!=0) && (countnei == 1) ) /* endpoint found */
             {
                if ( (act->oldneinum - 1) >= deletednei_limit ) /* potentially deletable endpoint found */
                  {         
                     /* is the only 12-neighbor an endpoint ? */
                     tmp_addr = act_addr;
                     deletion_needed = 1;
                     if ( *(image + tmp_addr - 1            -   size_xy ) ) /* 0 */
                       {
                         act_addr = tmp_addr - 1            -   size_xy;
                         collect_18_neighbours();
	                 if ( countnei == 1 )
	                   deletion_needed = 0;
                       } /* endif - 0 */
                     else
                       {  
                          if ( *(image + tmp_addr     -   size_x -   size_xy ) ) /* 1 */
                            {
                              act_addr = tmp_addr     -   size_x -   size_xy;
                              collect_18_neighbours();
	                      if ( countnei == 1 )
	                        deletion_needed = 0;
                            } /* endif - 1 */
                          else
                            {
                              if ( *(image + tmp_addr + 1            -   size_xy ) ) /* 2 */
                                {
                                  act_addr = tmp_addr + 1            -   size_xy;
                                  collect_18_neighbours();
	                          if ( countnei == 1 )
	                            deletion_needed = 0;
                                } /* endif - 2 */
                              else
                                {
                                  if ( *(image + tmp_addr     +   size_x -   size_xy ) ) /* 3 */
                                    {
                                      act_addr = tmp_addr     +   size_x -   size_xy;
                                      collect_18_neighbours();
	                              if ( countnei == 1 )
	                                 deletion_needed = 0;
                                    } /* endif - 3 */
                                  else
                                    {
                                      if ( *(image + tmp_addr - 1            +   size_xy ) ) /* 4 */
                                        {
                                          act_addr = tmp_addr - 1            +   size_xy;
                                          collect_18_neighbours();
	                                  if ( countnei == 1 )
	                                    deletion_needed = 0;
                                        } /* endif - 4 */
                                      else
                                        {
                                          if ( *(image + tmp_addr     -   size_x +   size_xy ) ) /* 5 */
                                            {
                                              act_addr = tmp_addr     -   size_x +   size_xy;
                                              collect_18_neighbours();
	                                      if ( countnei == 1 )
	                                        deletion_needed = 0;
                                            } /* endif - 5 */
                                          else
                                            {
                                              if ( *(image + tmp_addr + 1            +   size_xy ) ) /* 6 */
                                                {
                                                  act_addr = tmp_addr + 1            +   size_xy;
                                                  collect_18_neighbours();
	                                          if ( countnei == 1 )
	                                            deletion_needed = 0;
                                                } /* endif - 6 */
                                              else
                                                {
                                                  if ( *(image + tmp_addr     +   size_x +   size_xy ) ) /* 7 */
                                                    {
                                                      act_addr = tmp_addr     +   size_x +   size_xy;
                                                      collect_18_neighbours();
	                                              if ( countnei == 1 )
         	                                        deletion_needed = 0;
                                                    } /* endif - 7 */
                                                  else
                                                    {
                                                      if ( *(image + tmp_addr - 1 +   size_x             ) ) /* 8 */
                                                        {
                                                          act_addr = tmp_addr - 1 +   size_x            ;
                                                          collect_18_neighbours();
	                                                  if ( countnei == 1 )
	                                                    deletion_needed = 0;
                                                        } /* endif - 8 */
                                                      else
                                                        {
                                                          if ( *(image + tmp_addr - 1 -   size_x             ) ) /* 9 */
                                                            {
                                                              act_addr = tmp_addr - 1 -   size_x            ;
                                                              collect_18_neighbours();
	                                                      if ( countnei == 1 )
	                                                        deletion_needed = 0;
                                                            } /* endif - 9 */
                                                          else
                                                            {
                                                              if ( *(image + tmp_addr + 1 -   size_x             ) ) /* 10 */
                                                                {
                                                                  act_addr = tmp_addr + 1 -   size_x            ;
                                                                  collect_18_neighbours();
	                                                          if ( countnei == 1 )
	                                                            deletion_needed = 0;
                                                                } /* endif - 10 */
                                                              else
                                                                {
                                                                  if ( *(image + tmp_addr + 1 +   size_x             ) ) /* 11 */
                                                                    {
                                                                      act_addr = tmp_addr + 1 +   size_x            ;
                                                                      collect_18_neighbours();
	                                                              if ( countnei == 1 )
	                                                                deletion_needed = 0;
                                                                    } /* endif - 11 */                            
                                                                } /* endelse - 10 */                           
                                                            } /* endelse - 9 */                          
                                                        } /* endelse - 8 */                          
                                                    } /* endelse - 7 */                          
                                                } /* endelse - 6 */                           
                                            } /* endelse - 5 */                        
                                        } /* endelse - 4 */                       
                                    } /* endelse - 3 */                        
                                } /* endelse - 2 */                        
                            } /* endelse - 1 */
                       } /* endelse - 0 */ 
                    if ( deletion_needed )    
                      {
        	        q          = (struct listcell*)malloc(sizeof(struct listcell));
  	                q->address = tmp_addr;   /* original act_addr */
	                q->next    = DelEndList;
                        DelEndList = q;
                      } /* endif - deletion_needed */
                  } /* endif - potentially deletable endpoint found */
             } /* endif - endpoint found */
      } /* endfor */ 

    /* set deletable endpoints to 0 */
      for ( act=DelEndList; act!=NULL; act=act->next )
        {
           act_addr = act->address;
           *(image + act_addr ) = 0;       
           deleted_endpoints ++;
        }
    
    /* empty DelEndList */
      act = DelEndList;
      while( act )
        {
           act = act->next;
           free(DelEndList);
           DelEndList = act;
        }
    
} /* i - endfor */
/******* end of endpoint re-checking ******/
            
/* scan BorderList - create AddedList */
AddedList = NULL;
added_points = 0;
for ( p=BorderList; p!=NULL; p=p->next )
  {
    curr_addr = p->address;
    if ( *(image + curr_addr ) == 0 )
      {
        /* investigating 18-neighbours */
        act_addr = curr_addr + 1 + size_x           ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr + 1 - size_x           ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr - 1 + size_x           ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr - 1 - size_x           ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr + 1          + size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr + 1          - size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr - 1          + size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr - 1          - size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr     + size_x + size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr     + size_x - size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr     - size_x + size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
        act_addr = curr_addr     - size_x - size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }

        act_addr = curr_addr + 2                        ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
          
        act_addr = curr_addr - 2                        ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
          
        act_addr = curr_addr     + 2*size_x             ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
          
        act_addr = curr_addr     - 2*size_x             ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
          
        act_addr = curr_addr                 + 2*size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }

        act_addr = curr_addr                 - 2*size_xy ;
        if ( *(image + act_addr ) & 0x01 )
          {
	    q = (struct listcell*)malloc(sizeof(struct listcell));
	    q->address         = act_addr;
	    q->next            = AddedList;
            AddedList = q;
            *(image + act_addr ) = 2;
	    added_points++;
          }
      } /* endif */
  } /* endfor */
  
/* remove deleted points from BorderList */
  prev = (struct listcell*)malloc(sizeof(struct listcell));
  act = BorderList;
  while( act )
    {   
       act_addr = act->address;
       if ( *(image + act_addr ) == 0 )
         {
	   if ( act == BorderList )
	     {
	       tmp = BorderList;
	       act = act->next;
	       BorderList = act;
	       free(tmp);
	     }
	   else
	     {
	       tmp        = act;
	       act        = act->next;
	       prev->next = act;
	       free(tmp);
	       /* prev remains the same */
	     }
	 }
       else
         {
	   prev = act;
	   act  = act->next;
	 }
    }

/* scan AddedList - update BorderList */
p = AddedList;
while( p )
  {
    q = p;
    p = p->next;
    q->next = BorderList;
    BorderList = q;
    AddedList = p;
  }   

return(deleted_endpoints);
}
/*=========== end of function UpdateBorderPoints ===========*/


/*========= subfield4_curve_thinning_FCC =========*/
void subfield4_curve_thinning_FCC(void)
{
  unsigned int changed;
  unsigned int deleted0; 
  unsigned int deleted1;
  unsigned int deleted2;
  unsigned int deleted3; 
  unsigned int deletedend;
  
  Init_BorderList();
  
  iter = 0;
  changed = 1;
  while ( changed )
    {
      SpreadOutBorderPoints();
      deleted0   = SF_Deletion(0);
      deleted1   = SF_Deletion(1);
      deleted2   = SF_Deletion(2);
      deleted3   = SF_Deletion(3);   
      deletedend = UpdateBorderPoints();
      changed = deleted0 + deleted1 + deleted2 + deleted3;
      iter++;
      printf("\n  - iteration step: %3d - deleted point(s): %d+%d+%d+%d = %d (+%d)",
             iter, 
             deleted0, deleted1, deleted2, deleted3, 
             changed, deletedend);       
    }

}
/*========= end of function subfield4_curve_thinning_FCC =========*/


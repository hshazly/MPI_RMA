#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mpi.h"


void main(){
     MPI_Init(NULL, NULL);

     int size, rank, target;

     MPI_Win put_win, get_win;

     MPI_Comm_size(MPI_COMM_WORLD, &size);
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

     if(rank == size-1){
         target = 0;
     }else{
         target = rank+1;
     }

     /////////////////////////// MPI_Put Test
     /////////// Each process puts its rank to neighbour
    
     int *num_exposed, expected = -1;
 
     MPI_Win_allocate(sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &num_exposed, &put_win);

     *num_exposed = -1;    
 
     if(rank == 0){
        expected=size-1;
     }else{
        expected=rank-1;
     }
      
     MPI_Win_fence(0, put_win); 
     MPI_Put(&rank, 1, MPI_INT, target, 0, 1, MPI_INT, put_win); 
     MPI_Win_fence(0, put_win);
     
     if(*num_exposed != expected){
        printf("Put Error -- rank %d: %d should be %d\n", rank, *num_exposed, expected);
     }
    
     MPI_Win_free(&put_win);

     /////////////////////////// MPI_Get Test
     /////////// Each rank gets neighbour's rank

     int *share_num, neighbour;
 
     MPI_Win_allocate(sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &share_num, &get_win);    

     *share_num = rank;

     MPI_Win_fence(0, get_win);
     MPI_Get(&neighbour, 1, MPI_INT, target, 0, 1, MPI_INT, get_win);
     MPI_Win_fence(0, get_win);

     if(neighbour != target){
        printf("Get Error -- rank %d: %d should be %d\n", rank, neighbour, target);
     }

     //printf("rank %d = %d\n", rank, n);
 
     MPI_Win_free(&get_win);

     MPI_Finalize();   
}

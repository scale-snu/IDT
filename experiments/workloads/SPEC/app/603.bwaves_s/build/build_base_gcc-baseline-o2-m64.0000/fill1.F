       subroutine fill1(t,nb,nx,ny,nzl)

      implicit none

#if defined(SPEC_MPI)
      include "mpif.h"
#endif
      integer nb,nx,ny,nzl
      real(kind=8)  t(nb,nx,ny,nzl+2)

      integer l,i,j

#if defined(SPEC_MPI)
C     Global domain data and common block /dom/
      integer nprocs
      common /dom/ nprocs
C     Domain indexes and local common block /proc/
      integer npl,np,npr
      common /proc/ npl,np,npr
      
      integer m,count,stat(mpi_status_size),ierr
      real(kind=8) rbuf(nb*nx*ny),sbuf(nb*nx*ny)
#endif

#if !defined(SPEC_MPI)

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(j,i,l)
#endif 
            do j=1,ny
               do i=1,nx
                  do l=1,nb
                     t(l,i,j,nzl+2)=t(l,i,j,2)
                  enddo
               enddo
            enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(j,i,l)
#endif 
            do j=1,ny
               do i=1,nx
                  do l=1,5
                     t(l,i,j,1)=t(l,i,j,nzl+1)
                  enddo
               enddo
            enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

#else

      count=nb*nx*ny
C     Send --> Left    Recieve <-- Right
         call mpi_barrier( MPI_COMM_WORLD, ierr)
            do j=1,ny
               do i=1,nx
                  do l=1,nb
                     m=l+nb*(i-1) +nb*nx*(j-1)
                     sbuf(m)=t(l,i,j,2)
                  enddo
               enddo
            enddo

            call mpi_sendrecv(sbuf,count,mpi_double_precision,npl,1,
     1                        rbuf,count,mpi_double_precision,npr,1,
     2                         MPI_COMM_WORLD,stat, ierr)

            do j=1,ny
               do i=1,nx
                  do l=1,nb
                     m=l+nb*(i-1) +nb*nx*(j-1)
                     t(l,i,j,nzl+2)=rbuf(m)
                  enddo
               enddo
            enddo
C     Send --> Right    Recieve <-- Left
         call mpi_barrier(MPI_COMM_WORLD, ierr)
            do j=1,ny
               do i=1,nx
                  do l=1,nb
                     m=l+nb*(i-1) +nb*nx*(j-1)
                     sbuf(m)=t(l,i,j,nzl+1)
                  enddo
               enddo
            enddo

            call mpi_sendrecv(sbuf,count,mpi_double_precision,npr,1,
     1                        rbuf,count,mpi_double_precision,npl,1,
     2                         MPI_COMM_WORLD,stat, ierr)

            do j=1,ny
               do i=1,nx
                  do l=1,5
                     m=l+nb*(i-1) +nb*nx*(j-1)
                     t(l,i,j,1)=rbuf(m)
                  enddo
               enddo
            enddo
#endif

      return
      end

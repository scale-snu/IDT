      subroutine fill2(q,nx,ny,nzl)

      implicit none

#if defined(SPEC_MPI)
      include "mpif.h"
#endif
      integer nx,ny,nzl
      real(kind=8)  q(5,nx,ny,nzl+4)

      integer l,i,j

#if defined(SPEC_MPI)
C     Global domain data and common block /dom/
      integer nprocs
      common /dom/ nprocs
C     Domain indexes and local common block /proc/
      integer npl,np,npr
      common /proc/ npl,np,npr

      integer m,count,stat(mpi_status_size),ierr
      real(kind=8) rbuf1(5*nx*ny),rbuf2(5*nx*ny),sbuf1(5*nx*ny),sbuf2(5*nx*ny)
#endif

#if !defined(SPEC_MPI)

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(j,i,l)
#endif 
            do j=1,ny
               do i=1,nx
                  do l=1,5
                     q(l,i,j,nzl+3)=q(l,i,j,3)
                     q(l,i,j,nzl+4)=q(l,i,j,4)
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
                     q(l,i,j,1)=q(l,i,j,nzl+1)
                     q(l,i,j,2)=q(l,i,j,nzl+2)
                  enddo
               enddo
            enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

#else

      count=5*nx*ny

C     Send --> Left    Recieve <-- Right
         call mpi_barrier( MPI_COMM_WORLD, ierr)

            do j=1,ny
               do i=1,nx
                  do l=1,5
                     m=l+5*(i-1) +5*nx*(j-1)
                     sbuf1(m)=q(l,i,j,3)
                     sbuf2(m)=q(l,i,j,4)
                  enddo
               enddo
            enddo




            call mpi_sendrecv(sbuf1,count,mpi_double_precision,npl,1,
     1                        rbuf1,count,mpi_double_precision,npr,1,
     2                         MPI_COMM_WORLD,stat, ierr)
            call mpi_sendrecv(sbuf2,count,mpi_double_precision,npl,2,
     1                        rbuf2,count,mpi_double_precision,npr,2,
     2                         MPI_COMM_WORLD,stat, ierr)

            do j=1,ny
               do i=1,nx
                  do l=1,5
                     m=l+5*(i-1) +5*nx*(j-1)
                     q(l,i,j,nzl+3)=rbuf1(m)
                     q(l,i,j,nzl+4)=rbuf2(m)
                  enddo
               enddo
            enddo

C     Send --> Right    Recieve <-- Left
         call mpi_barrier(MPI_COMM_WORLD, ierr)

            do j=1,ny
               do i=1,nx
                  do l=1,5
                     m=l+5*(i-1) +5*nx*(j-1)
                     sbuf1(m)=q(l,i,j,nzl+1)
                     sbuf2(m)=q(l,i,j,nzl+2)
                  enddo
               enddo
            enddo

            call mpi_sendrecv(sbuf1,count,mpi_double_precision,npr,1,
     1                        rbuf1,count,mpi_double_precision,npl,1,
     2                         MPI_COMM_WORLD,stat, ierr)
            call mpi_sendrecv(sbuf2,count,mpi_double_precision,npr,2,
     1                        rbuf2,count,mpi_double_precision,npl,2,
     2                         MPI_COMM_WORLD,stat, ierr)

            do j=1,ny
               do i=1,nx
                  do l=1,5
                     m=l+5*(i-1) +5*nx*(j-1)
                     q(l,i,j,1)=rbuf1(m)
                     q(l,i,j,2)=rbuf2(m)
                  enddo
               enddo
            enddo

         call mpi_barrier(MPI_COMM_WORLD, ierr)

#endif

      return
      end

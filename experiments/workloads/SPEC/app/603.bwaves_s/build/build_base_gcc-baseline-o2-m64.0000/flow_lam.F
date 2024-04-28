
      program driver
      implicit none

#if defined(SPEC_MPI)
      include "mpif.h"
#endif

      character(len=80) title
      integer nx,ny,nz,scheme,conf,time_steps
      integer ks,ke,nzl,id
      real(kind=8) nuim,nuex2,nuex4,cfl, Re, Pr

#if !defined(SPEC)
#if defined(SPEC_MPI)
      real(kind=8) tstart, tend
#else
      real(kind=8) tm, dtime, tarray(2)
      real(kind=8) gtm
#endif
#endif
C     real(kind=8) tmp

#if defined(SPEC_MPI)
      integer chunk,nextra
      integer ierr, ip

C  Common variable to communicate #processors to other routines
      integer nprocs
      common /dom/ nprocs
#endif

      integer   nbuff(8)
      real(kind=8)    rbuff(6)

#if defined(SPEC_MPI)
      call mpi_init(ierr)
      call mpi_comm_size(mpi_comm_world,nprocs,ierr)
      call mpi_comm_rank(mpi_comm_world,ip,ierr)
#endif

#if defined(SPEC_MPI)
      if( ip.eq.0) then
#endif
C        open (5,file='bwaves.in')
C        open (10,file='bwaves.out')

         write (6,'(1X,A)')  'BI-CGSTAB & symmetric difference scheme '
         write (6,'(1X,A)')   '3D  Laminar shock wave propagation'

#if !defined(SPEC)
         write (6,*)   'Re, Pr'
#endif
       read(5,*) title
       read (5,*) rbuff(1),rbuff(2)
         write (6,'(1X,2(A,F15.6))')   'Re: ',rbuff(1),
     *             '    Pr: ',rbuff(2)


#if !defined(SPEC)
         write (6,*)  '(nx,ny,nz) ?'
#endif
       read(5,*) title
       read(5,*) nbuff(1),nbuff(2),nbuff(3)
         write (6,'(1X,A,3(1X,I5))')  'grid size is: ', nbuff(1),
     *              nbuff(2), nbuff(3)

#if !defined(SPEC)
         write (6,*)  '(CFL, nuim, nuex2, nuex4) ?'
#endif
       read(5,*) title
       read(5,*) rbuff(3),rbuff(4),rbuff(5),rbuff(6)

c        Adjustment of artificial viscosity 
c         tmp =1.0 * nprocs /32
c         if ( tmp .gt. 1.0) then
c           rbuff(4) =rbuff(4) + 0.05 * tmp
c           rbuff(5) =rbuff(5) + 0.05 * tmp
c         endif           

         write (6,'(1X,A,F7.3,A,A,F7.3,A,A,F7.3,A,F7.3)')
     1       'CFL:',rbuff(3),'   ', 'nuim:',rbuff(4),'  ',
     1           'nuex2:',rbuff(5),' nuex4:', rbuff(6)

#if !defined(SPEC)
         write (6,*)  'What scheme you will use -explicit(0) or implicit(1)'
#endif
       read(5,*) title
       read (5,*) nbuff(4)
       if (nbuff(4).EQ.0) then
          write (6,'(1X,A)')   'Explicit scheme is working'
       else
          write (6,'(1X,A)')   'Implicit scheme is working'      
       endif

#if !defined(SPEC)
         write (6,*)  'What initial configuration do you want-'
#endif
       read(5,*) title
#if !defined(SPEC)
         write (6,*)   'cubic(0) or spheric(1) ?'
#endif
       read (5,*) nbuff(5)
       if (nbuff(5).EQ.0) then
          write (6,'(1X,A)')   'Cubic initial configuration'
       else
          write (6,'(1X,A)')   'Spheric initial configuration'      
       endif

#if !defined(SPEC)
         write (6,*)   'Number of Time Steps ?'
#endif
       read(5,*) title
       read (5,*) nbuff(6)
         write (6,'(1X,A,1X,I6/)')   'Number of Time Steps:', nbuff(6)
       read(5,*) title
       title=trim(title)
       open(unit=20,file=title,form='formatted')
       read(5,*) title
       title=trim(title)
       open(unit=30,file=title,form='formatted')
#if defined(SPEC_MPI)
      endif
#endif
C

#if defined(SPEC_MPI)
c      Broadcast input data
      call mpi_bcast(rbuff,6,mpi_double_precision,0,mpi_comm_world,
     1                 ierr)
      call mpi_bcast(nbuff,6,mpi_integer,0,mpi_comm_world,ierr)
#endif


c       Unpack input buffers
      Re = rbuff(1)
      Pr = rbuff(2)
      CFL = rbuff(3)
      nuim = rbuff(4)
      nuex2 = rbuff(5)
      nuex4 = rbuff(6)
      nx = nbuff(1)
      ny = nbuff(2)
      nz = nbuff(3)
      scheme = nbuff(4)
      conf = nbuff(5)
      time_steps = nbuff(6)
      id = nbuff(7)


      

c       **********************************************************

#if !defined(SPEC_MPI)
C   Not with MPI: simply call shell
#if !defined(SPEC)
      gtm=0.0
      tm=dtime(tarray)
#endif
      ks = 1
      ke = nz
      nzl = nz
      call shell(Re,Pr,nx,ny,nz,ks,ke,nzl,
     1           nuim,nuex2,nuex4,cfl,scheme,conf,time_steps)
#if !defined(SPEC)
      write (6,*)'Global Time:',gtm,' sec'
#endif
      close(20)
      close(30)
#else
C   MPI: Make domains and call shell for current domain

#if !defined(SPEC)
      tstart = MPI_Wtime()
#endif
      chunk=nz/nprocs
        nextra=mod(nz,nprocs)
      ks=ip*chunk+1
      ke=ks+chunk-1
      if(nextra.ne.0) then
         ks=ks+min(nextra,ip)
         ke=ke+min(nextra,ip+1)
      endif
      nzl=ke-ks+1
      call make_domains(Re,Pr,nx,ny,nz,ks,ke,nzl,
     1  nuim,nuex2,nuex4,cfl,scheme,conf,time_steps,ip)

#if !defined(SPEC)
      tend = MPI_Wtime()
      if(ip.eq.0) write (6,*)   'Global Time:',tend - tstart,' sec'
#endif
      call mpi_finalize(ierr)
      end 

      subroutine make_domains(Re,Pr,nx,ny,nz,ks,ke,nzl,
     1                    nuim,nuex2,nuex4,cfl,scheme,conf,ni,ip)
      implicit none
      integer nx,ny,nz,ks,ke,scheme,conf,ni,nzl,ip
      real(kind=8) nuim,nuex2,nuex4,cfl, Re, Pr

C     common block /dom/
      integer nprocs
      common /dom/ nprocs

C     Domain indexes and local common block /proc/
      integer npl,np,npr
      common /proc/ npl,np,npr


C     Define the stencil in procs space
      np=ip
      npl=np-1
      if(npl.lt.0) npl=nprocs-1
      npr= np + 1
      if (npr.gt.nprocs-1) npr = 0

      write (11+np,*)  "*** Region ",np," allocated."

      call shell(Re,Pr,nx,ny,nz,ks,ke,nzl,
     1           nuim,nuex2,nuex4,cfl,scheme,conf,ni)

      return
#endif

      end

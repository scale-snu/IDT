
      program driver
      implicit none


      character(len=80) title
      integer nx,ny,nz,scheme,conf,time_steps
      integer ks,ke,nzl,id
      real(kind=8) nuim,nuex2,nuex4,cfl, Re, Pr

C     real(kind=8) tmp


      integer   nbuff(8)
      real(kind=8)    rbuff(6)


C        open (5,file='bwaves.in')
C        open (10,file='bwaves.out')

         write (6,'(1X,A)')  'BI-CGSTAB & symmetric difference scheme '
         write (6,'(1X,A)')   '3D  Laminar shock wave propagation'

       read(5,*) title
       read (5,*) rbuff(1),rbuff(2)
         write (6,'(1X,2(A,F15.6))')   'Re: ',rbuff(1),
     *             '    Pr: ',rbuff(2)


       read(5,*) title
       read(5,*) nbuff(1),nbuff(2),nbuff(3)
         write (6,'(1X,A,3(1X,I5))')  'grid size is: ', nbuff(1),
     *              nbuff(2), nbuff(3)

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

       read(5,*) title
       read (5,*) nbuff(4)
       if (nbuff(4).EQ.0) then
          write (6,'(1X,A)')   'Explicit scheme is working'
       else
          write (6,'(1X,A)')   'Implicit scheme is working'      
       endif

       read(5,*) title
       read (5,*) nbuff(5)
       if (nbuff(5).EQ.0) then
          write (6,'(1X,A)')   'Cubic initial configuration'
       else
          write (6,'(1X,A)')   'Spheric initial configuration'      
       endif

       read(5,*) title
       read (5,*) nbuff(6)
         write (6,'(1X,A,1X,I6/)')   'Number of Time Steps:', nbuff(6)
       read(5,*) title
       title=trim(title)
       open(unit=20,file=title,form='formatted')
       read(5,*) title
       title=trim(title)
       open(unit=30,file=title,form='formatted')
C



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

C   Not with MPI: simply call shell
      ks = 1
      ke = nz
      nzl = nz
      call shell(Re,Pr,nx,ny,nz,ks,ke,nzl,
     1           nuim,nuex2,nuex4,cfl,scheme,conf,time_steps)
      close(20)
      close(30)

      end

      subroutine shell(Re,Pr,nx,ny,nz,ks,ke,nzl,
     1                 nuim,nuex2,nuex4,cfl,scheme,conf,ni)

      implicit none
#if defined(SPEC_MPI)
      include "mpif.h"
#endif

#if !defined(SPEC)
      COMMON /scale/ ro1,p1,ro2,p2
#endif
      integer nx,ny,nz,ni,n,i,j,k,scheme,conf,si,sj,sk,l,m
      integer im2,im1,ip1,ip2,jm2,jm1,jp1,jp2,km2,km1,kp1,kp2
      integer ks,ke
      integer nzl, nzl1,nzl2,nzl3,nzl4,kg,kz,kq,ierr
      real(kind=8) nuim,nuex2,nuex4,cfl,epsilon,dx,dy,dz,dt,rad
      real(kind=8) gm,u1,v1,w1,p1,ro1,u2,v2,w2,p2,ro2,Re,Pr,cfll,dtl
      real(kind=8) time,t1,t2,t3,t4,t5,t6,t7,t8,mu
      real(kind=8) gtm,dqnorm
#if !defined(SPEC)
      real(kind=8) dtime,tm,tarray(2),rad1,tmg
#endif

#if defined(SPEC_MPI)
C     Global domain data and common block /dom/
      integer nprocs
      common /dom/ nprocs

C     Domain indexes and local common block /proc/
      integer npl,np,npr
      common /proc/ npl,np,npr
#endif

      real(kind=8) q(5,nx,ny,nzl+4), dq(5,nx,ny,nzl+2)
      real(kind=8) rhs(5,nx,ny,nzl), e(5,nx,ny,nzl), f(5,nx,ny,nzl)
      real(kind=8) g(5,nx,ny,nzl+2), ev(5,nx,ny,nzl), fv(5,nx,ny,nzl)
      real(kind=8) gv(5,nx,ny,nzl+2)

      real(kind=8) ae(5,5,nx,ny,nzl), be(5,5,nx,ny,nzl)
      real(kind=8) ce(5,5,nx,ny,nzl+2)
      real(kind=8) av(5,5,nx,ny,nzl), bv(5,5,nx,ny,nzl)
      real(kind=8) cv(5,5,nx,ny,nzl+2)

      real(kind=8) a(5,5,nx,ny,nzl), axp(5,5,nx,ny,nzl)
      real(kind=8) ayp(5,5,nx,ny,nzl)
      real(kind=8) azp(5,5,nx,ny,nzl), axm(5,5,nx,ny,nzl)
      real(kind=8) aym(5,5,nx,ny,nzl)
      real(kind=8) azm(5,5,nx,ny,nzl), ident(5,5,nx,ny,nzl)

#if !defined(SPEC_MPI)
      real(kind=8) u(nx,ny,nz),v(nx,ny,nz),w(nx,ny,nz),p(nx,ny,nz)
      real(kind=8) ro(nx,ny,nz),at(nx,ny,nz)
#endif



#if !defined(SPEC)
C     initialize solution timer
      tmg=0.
#endif

      nzl1=nzl+1
      nzl2=nzl+2
      nzl3=nzl+3
      nzl4=nzl+4


C       This particular problem is periodic only

C     Data initialization
      epsilon=1.0D-03

      si=nx/8
      sj=ny/8
      sk=nz/8
      dx=1./(nx-1)
      dy=1./(ny-1)
      dz=1./(nz-1)
      time=0.


#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(m,l,i,j,k)
#endif 
        do k = 1,nzl
           do j=1,ny
              do i=1,nx
                 do l=1,5
                    rhs(l,i,j,k)=0.0
                    do m=1,5
                       a(m,l,i,j,k)=0.0
                       axp(m,l,i,j,k)=0.0
                       ayp(m,l,i,j,k)=0.0
                       azp(m,l,i,j,k)=0.0
                       axm(m,l,i,j,k)=0.0
                       aym(m,l,i,j,k)=0.0
                       azm(m,l,i,j,k)=0.0
                       ident(m,l,i,j,k)=0.0
                    enddo
                    ident(l,l,i,j,k)=1.
                 enddo
              enddo
           enddo
        enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(l,i,j,k)
#endif 
        do k = 1,nzl2
           do j=1,ny
              do i=1,nx
                 do l=1,5
                    dq(l,i,j,k)=0.0
                 enddo
              enddo
           enddo
        enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

C Flow Initialization

      gm=1.4
      u1=0.0
      v1=0.0
      w1=0.0
      p1=1.0
      ro1=1.0
      u2=0.0
      v2=0.0
      w2=0.0
      p2=0.1
      ro2=0.1

      t1=ro2
      t2=ro2*u2
      t3=ro2*v2
      t4=ro2*w2
      t5=p2/(gm-1.)+0.5*ro2*(u2**2+v2**2+w2**2)


#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(i,j,k)
#endif 
        do k = 3,nzl2
           do j=1,ny
              do i=1,nx
                 q(1,i,j,k)=t1
                 q(2,i,j,k)=t2
                 q(3,i,j,k)=t3
                 q(4,i,j,k)=t4
                 q(5,i,j,k)=t5
              enddo
           enddo
        enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

      t1=ro1
      t2=ro1*u1
      t3=ro1*v1
      t4=ro1*w1
      t5=p1/(gm-1.)+0.5*ro1*(u1**2+v1**2+w1**2)

      if (conf.EQ.0) then
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(i,j,k,kg)
#endif 
         do k=3,nzl2
            kg=ks+k-3
            if((kg.ge.nz/2-sk).AND.(kg.le.nz/2+sk)) then
            do j=ny/2-sj,ny/2+sj
               do i=nx/2-si,nx/2+si
                 q(1,i,j,k)=t1
                 q(2,i,j,k)=t2
                 q(3,i,j,k)=t3
                 q(4,i,j,k)=t4
                 q(5,i,j,k)=t5
              enddo
           enddo
           endif
        enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

      else
         rad=MIN((si*dx)**2,(sj*dy)**2,(sk*dz)**2)
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(i,j,k,t6,t7,t8,kg)
#endif 
         do k=3,nzl2
            kg=ks+3-k
            t8=((kg-nz/2)*dz)**2
            do j=1,ny
               t7=((j-ny/2)*dy)**2
               do i=1,nx
                  t6=((i-nx/2)*dx)**2 +t7 + t8
                  if (t6.LE.rad) then
                     q(1,i,j,k)=t1
                     q(2,i,j,k)=t2
                     q(3,i,j,k)=t3
                     q(4,i,j,k)=t4
                     q(5,i,j,k)=t5
                  endif
               enddo
            enddo
         enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 
      endif


C Propagation in time

      time=0.
#if !defined(SPEC)
      tm=dtime(tarray)
      gtm=gtm+tm
#endif

      do n=1,ni
C   Fill overlaped region
         call fill2(q,nx,ny,nzl)

C     Time step definition

      cfll=0.1+(n-1.)*cfl/20.0
      if (cfll.ge.cfl) cfll=cfl
      t8=0.0

C**************************Comment out these two lines
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(i,j,k,t1,t2,t3,t4,t5,t6,t7,mu) 
!$OMP+            REDUCTION(MAX:t8)
#endif 
      do k=3,nzl2
         do j=1,ny
            do i=1,nx
               t1=q(1,i,j,k)
               t2=q(2,i,j,k)/t1
               t3=q(3,i,j,k)/t1
               t4=q(4,i,j,k)/t1
               t5=(gm-1.)*(q(5,i,j,k)-0.5*t1*(t2*t2+t3*t3+t4*t4))
               t6=SQRT(gm*t5/t1)
               mu=gm*Pr*(gm*t5/t1)**0.75*2./Re/t1
               t7=((abs(t2)+t6)/dx+mu/dx**2)**2 +
     1            ((abs(t3)+t6)/dy+mu/dy**2)**2 +
     2            ((abs(t4)+t6)/dz+mu/dz**2)**2

               t7=SQRT(t7)
               t8=max(t8,t7)
            enddo
         enddo
      enddo
C**************************Comment out this line
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

      dtl=cfll / t8

#if defined(SPEC_MPI)
      call mpi_allreduce(dtl,dt,1,mpi_double_precision,mpi_min,
     1                    MPI_COMM_WORLD, ierr)
#else
      dt = dtl
#endif


C Left hand side

      call jacobian (q, ae, av, 0.0d0, 1.0d0, 0.0d0, 0.0d0,
     1               Re, Pr, gm, nx, ny, nzl4, nzl, dx, 1)
      call jacobian (q, be, bv, 0.0d0, 0.0d0, 1.0d0, 0.0d0,
     1               Re, Pr, gm, nx, ny, nzl4, nzl, dy, 2)
      call jacobian (q, ce, cv, 0.0d0, 0.0d0, 0.0d0, 1.0d0,
     1               Re, Pr, gm, nx, ny, nzl4, nzl2, dz, 3)


#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(m,l,i,j,k,kz,km1,kp1,jm1,jp1,im1,ip1,t1)
#endif 
      do k=1,nzl
         kz=k+1
         km1=kz-1
         kp1=kz+1
         do j=1,ny
            jm1=mod(j-2+ny,ny)+1
            jp1=mod(j,ny)+1
            do i=1,nx
               im1=mod(i-2+nx,nx)+1
               ip1=mod(i,nx)+1
               do l=1,5
                  do m=1,5
                     t1=ident(m,l,i,j,k)
                     a(m,l,i,j,k)=t1 - 0.5*dt*(
     1               (av(m,l,i,j,k)-av(m,l,im1,j,k))/dx+
     2               (bv(m,l,i,j,k)-bv(m,l,i,jm1,k))/dy+
     3               (cv(m,l,i,j,kz)-cv(m,l,i,j,km1))/dz)/Re
     4               + 2.*nuim*dt*(1./dx+1./dy+1./dz) * t1     

                     axp(m,l,i,j,k)=0.5*dt/dx*
     1               (ae(m,l,ip1,j,k)-av(m,l,i,j,k)/Re) 
     2               - nuim*dt/dx * t1
                     axm(m,l,i,j,k)=-0.5*dt/dx*
     1                    (ae(m,l,im1,j,k)-av(m,l,im1,j,k)/Re)
     2               - nuim*dt/dx * t1 

                     ayp(m,l,i,j,k)=0.5*dt/dy*
     1               (be(m,l,i,jp1,k)-bv(m,l,i,j,k)/Re) 
     2               - nuim*dt/dy * t1
                     aym(m,l,i,j,k)=-0.5*dt/dy*
     1               (be(m,l,i,jm1,k)-bv(m,l,i,jm1,k)/Re) 
     2               - nuim*dt/dy * t1

                     azp(m,l,i,j,k)=0.5*dt/dz*
     1               (ce(m,l,i,j,kp1)-cv(m,l,i,j,kz)/Re)
     2               - nuim*dt/dz * t1
                     azm(m,l,i,j,k)=-0.5*dt/dz*
     1               (ce(m,l,i,j,km1)-cv(m,l,i,j,km1)/Re) 
     2               - nuim*dt/dz * t1

                  enddo
               enddo
            enddo
         enddo
      enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 


C Right hand side
      call flux (q,e,f,g,ev,fv,gv,Re,Pr,gm,nx,ny,nzl,dx,dy,dz)


#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(l,i,j,k,im2,im1,ip1,ip2,jm2,jm1,jp1,jp2,
!$OMP+        km2,km1,kp1,kp2,kq,kg,t1,t2)
#endif 
      do k=1,nzl
         kq=k+2
         km2=kq-2
         km1=kq-1
         kp1=kq+1
         kp2=kq+2
         kg=k+1
         do j=1,ny
            jm2=mod(j+ny-3,ny)+1
            jm1=mod(j+ny-2,ny)+1
            jp1=mod(j,ny)+1
            jp2=mod(j+1,ny)+1
            do i=1,nx
               im2=mod(i+nx-3,nx)+1
               im1=mod(i+nx-2,nx)+1
               ip1=mod(i,nx)+1
               ip2=mod(i+1,nx)+1
               do l=1,5
                  t1= -0.5*dt*(
     1            (e(l,ip1,j,k)-e(l,im1,j,k))/dx +
     2            (f(l,i,jp1,k)-f(l,i,jm1,k))/dy +
     3            (g(l,i,j,kg+1)-g(l,i,j,kg-1))/dz) +
     4            dt/Re*((ev(l,i,j,k)-ev(l,im1,j,k))/dx +
     5                  (fv(l,i,j,k)-fv(l,i,jm1,k))/dy +
     6                  (gv(l,i,j,kg)-gv(l,i,j,kg-1))/dz)

C     Artificial Viscosity
C     Explicit dissipation - second order
                  t2=dt*nuex2*(
     1 (q(l,ip1,j,kq)-2.*q(l,i,j,kq)+q(l,im1,j,kq))/dx +
     2 (q(l,i,jp1,kq)-2.*q(l,i,j,kq)+q(l,i,jm1,kq))/dy +
     3 (q(l,i,j,kp1) -2.*q(l,i,j,kq)+q(l,i,j,km1))/dz)

C     Explicit dissipation - fourth order
                  t2=t2-dt*nuex4*(
     1   (q(l,ip2,j,kq)-4.*q(l,ip1,j,kq)+6.*q(l,i,j,kq)-
     2   4.*q(l,im1,j,kq)+q(l,im2,j,kq))/dx +
     3   (q(l,i,jp2,kq)-4.*q(l,i,jp1,kq)+6.*q(l,i,j,kq)-
     4   4.*q(l,i,jm1,kq)+q(l,i,jm2,kq))/dy +
     5   (q(l,i,j,kp2)-4.*q(l,i,j,kp1)+6.*q(l,i,j,kq)-
     6   4.*q(l,i,j,km1)+q(l,i,j,km2))/dz )

                  rhs(l,i,j,k)=t1+t2
                  dq(l,i,j,kg)=rhs(l,i,j,k) !initial guess is rhs
               enddo
            enddo
         enddo
      enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 


#if defined(SPEC_MPI)
      if ( np .eq. 0 ) then
#endif
         write (6,'(1X,A,I6,A,E20.11)')
     1          'Time step: ',n,'  dt: ',dt
#if defined(SPEC_MPI)
      endif
#endif

      if (scheme.eq.1) call bi_cgstab_block(dq,rhs,a,axp,
     1                ayp,azp,axm,aym,azm,epsilon,5,nx,ny,nzl)

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(l,i,j,k)
#endif 
      do k=3,nzl2
         do j=1,ny
            do i=1,nx
               do l=1,5
                  q(l,i,j,k)=q(l,i,j,k)+dq(l,i,j,k-1)
               enddo
            enddo
         enddo
      enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 
      dqnorm = 0.0d0
      do k=1,nz
         do j=1,ny
            do i=1,nx
               do l=1,5
                  dqnorm = dqnorm + dq(l,i,j,k)*dq(l,i,j,k)
               enddo
            enddo
         enddo
      enddo

      write(30,*) 'dqnorm ==', dqnorm

#if !defined(SPEC)
      tm=dtime(tarray)
      tmg=tmg+tm
#if defined(SPEC_MPI)
      if (np.eq.0) then 
c         print *, 'Time step: ',n,'  dt: ',dt 
          print *,'Timing for time step:', tm
      endif
#else
      write (6,*)'Timing for time step:', tm
      gtm=gtm+tm
#endif
#endif

      time=time+dt
      enddo

#if !defined(SPEC)
#if defined(SPEC_MPI)
      if (np.eq.0) print *,'Solution Time:',tmg,' sec'
#else
      write (6,*)'Solution Time:',tmg,' sec'
#endif
#endif

c 1    FORMAT (2F10.3)

      return
      end

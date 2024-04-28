      subroutine flux(q,e,f,g,ev,fv,gv,Re,Pr,gm,nx,ny,nzl,
     $dx,dy,dz)
      implicit none
      integer nx,ny,nzl,kq
      real(kind=8) gm,Re,Pr,dx,dy,dz


      real(kind=8) q(5,nx,ny,nzl+4),
     1     e(5,nx,ny,nzl),f(5,nx,ny,nzl),g(5,nx,ny,nzl+2),
     2     ev(5,nx,ny,nzl),fv(5,nx,ny,nzl),gv(5,nx,ny,nzl+2)

      real(kind=8) u(nx,ny,nzl+2),v(nx,ny,nzl+2),w(nx,ny,nzl+2),
     1     p(nx,ny,nzl+2),
     2     ro(nx,ny,nzl+2),mu(nx,ny,nzl+2)
      real(kind=8) t0,t1,t2,t3
      integer im1,ip1,jm1,jp1,km1,kp1,i,j,k,l,kg
      real(kind=8) dx2,dy2,dz2

      dx2=2.*dx
      dy2=2.*dy
      dz2=2.*dz

c     First ghost Z layer
      k=1
      kq=2
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE (i,j)
#endif 
      do j=1,ny
         do i=1,nx
            ro(i,j,k)=q(1,i,j,kq)
            u(i,j,k)=q(2,i,j,kq)/ro(i,j,k)
            v(i,j,k)=q(3,i,j,kq)/ro(i,j,k)
            w(i,j,k)=q(4,i,j,kq)/ro(i,j,k)
            p(i,j,k)=(gm-1.)*(q(5,i,j,kq)-0.5*ro(i,j,k)*
     1      (u(i,j,k)**2+v(i,j,k)**2+w(i,j,k)**2))
            mu(i,j,k)=(gm*p(i,j,k)/ro(i,j,k))**0.75

            g(1,i,j,k)=ro(i,j,k)*w(i,j,k)
            g(2,i,j,k)=ro(i,j,k)*w(i,j,k)*u(i,j,k)
            g(3,i,j,k)=ro(i,j,k)*w(i,j,k)*v(i,j,k)
            g(4,i,j,k)=ro(i,j,k)*w(i,j,k)*w(i,j,k)+p(i,j,k)
            g(5,i,j,k)=w(i,j,k)*(q(5,i,j,kq)+p(i,j,k))

         enddo
      enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

C     Inner Layers
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE (i,j,k,kq,kg)
#endif 
      do k=1,nzl
         kq=k+2
         kg=k+1
         do j=1,ny
            do i=1,nx

               ro(i,j,kg)=q(1,i,j,kq)
               u(i,j,kg)=q(2,i,j,kq)/ro(i,j,kg)
               v(i,j,kg)=q(3,i,j,kq)/ro(i,j,kg)
               w(i,j,kg)=q(4,i,j,kq)/ro(i,j,kg)
               p(i,j,kg)=(gm-1.)*(q(5,i,j,kq)-0.5*ro(i,j,kg)*
     1         (u(i,j,kg)**2+v(i,j,kg)**2+w(i,j,kg)**2))
               mu(i,j,kg)=(gm*p(i,j,kg)/ro(i,j,kg))**0.75

C     Euler's fluxes
               e(1,i,j,k)=ro(i,j,kg)*u(i,j,kg)
               e(2,i,j,k)=ro(i,j,kg)*u(i,j,kg)*u(i,j,kg)+p(i,j,kg)
               e(3,i,j,k)=ro(i,j,kg)*u(i,j,kg)*v(i,j,kg)
               e(4,i,j,k)=ro(i,j,kg)*u(i,j,kg)*w(i,j,kg)
               e(5,i,j,k)=u(i,j,kg)*(q(5,i,j,kq)+p(i,j,kg))


               f(1,i,j,k)=ro(i,j,kg)*v(i,j,kg)
               f(2,i,j,k)=ro(i,j,kg)*v(i,j,kg)*u(i,j,kg)
               f(3,i,j,k)=ro(i,j,kg)*v(i,j,kg)*v(i,j,kg)+p(i,j,kg)
               f(4,i,j,k)=ro(i,j,kg)*v(i,j,kg)*w(i,j,kg)
               f(5,i,j,k)=v(i,j,kg)*(q(5,i,j,kq)+p(i,j,kg))

               g(1,i,j,kg)=ro(i,j,kg)*w(i,j,kg)
               g(2,i,j,kg)=ro(i,j,kg)*w(i,j,kg)*u(i,j,kg)
               g(3,i,j,kg)=ro(i,j,kg)*w(i,j,kg)*v(i,j,kg)
               g(4,i,j,kg)=ro(i,j,kg)*w(i,j,kg)*w(i,j,kg)+p(i,j,kg)
               g(5,i,j,kg)=w(i,j,kg)*(q(5,i,j,kq)+p(i,j,kg))
            enddo
         enddo
      enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

c Last ghost Z layer

      k=nzl+2
      kq=nzl+3
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE (i,j)
#endif 
      do j=1,ny
         do i=1,nx
            ro(i,j,k)=q(1,i,j,kq)
            u(i,j,k)=q(2,i,j,kq)/ro(i,j,k)
            v(i,j,k)=q(3,i,j,kq)/ro(i,j,k)
            w(i,j,k)=q(4,i,j,kq)/ro(i,j,k)
            p(i,j,k)=(gm-1.)*(q(5,i,j,kq)-0.5*ro(i,j,k)*
     1      (u(i,j,k)**2+v(i,j,k)**2+w(i,j,k)**2))
            mu(i,j,k)=(gm*p(i,j,k)/ro(i,j,k))**0.75

            g(1,i,j,k)=ro(i,j,k)*w(i,j,k)
            g(2,i,j,k)=ro(i,j,k)*w(i,j,k)*u(i,j,k)
            g(3,i,j,k)=ro(i,j,k)*w(i,j,k)*v(i,j,k)
            g(4,i,j,k)=ro(i,j,k)*w(i,j,k)*w(i,j,k)+p(i,j,k)
            g(5,i,j,k)=w(i,j,k)*(q(5,i,j,kq)+p(i,j,k))

         enddo
      enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 


C     Viscous fluxes
C     GV in the ghost layer k=1

      k=1
      kp1=2
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(i,j,jm1,jp1,im1,ip1,t0,t1,t2,t3)
#endif 
      do j=1,ny
         jm1=mod(j+ny-2,ny)+1
         jp1=mod(j,ny)+1
         do i=1,nx
            im1=mod(i+nx-2,nx)+1
            ip1=mod(i,nx)+1
            t3=gm*p(i,j,k)/ro(i,j,k)

            gv(1,i,j,k)=0.0
            t0=(mu(i,j,k)+mu(i,j,kp1))/2.
            t1=(w(ip1,j,k)-w(im1,j,k))/dx2  
            gv(2,i,j,k)=t0*(
     1      ((w(ip1,j,kp1)-w(im1,j,kp1))/dx2+t1)/2.+
     2      (u(i,j,kp1)-u(i,j,k))/dz)

            t1=(w(i,jp1,k)-w(i,jm1,k))/dy2
            gv(3,i,j,k)=t0*(
     1      ((w(i,jp1,kp1)-w(i,jm1,kp1))/dy2+t1)/2.+
     2      (v(i,j,kp1)-v(i,j,k))/dz)

            t1=(u(ip1,j,k)-u(im1,j,k))/dx2
            t2=(v(i,jp1,k)-v(i,jm1,k))/dy2
            gv(4,i,j,k)=t0/3.*(4.*(w(i,j,kp1)-w(i,j,k))/dz-
     1      ((u(ip1,j,kp1)-u(im1,j,kp1))/dx2+t1+
     2      (v(i,jp1,kp1)-v(i,jm1,kp1))/dy2+t2))

            gv(5,i,j,k)=0.5*(
     1      (u(i,j,kp1)+u(i,j,k))*gv(2,i,j,k)+
     2      (v(i,j,kp1)+v(i,j,k))*gv(3,i,j,k)+
     4      (w(i,j,kp1)+w(i,j,k))*gv(4,i,j,k))+
     5      t0/Pr/(gm-1.)*(gm*p(i,j,kp1)/ro(i,j,kp1)-t3)/dz
         enddo
      enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 

C     Fluxes ev,fv,gv in inner points and ghost layer nzl+1
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP PARALLEL DO PRIVATE(i,j,k,kg,km1,kp1,jm1,jp1,im1,ip1,
!$OMP+              t0,t1,t2,t3)
#endif 
      do k=1,nzl
         kg=k+1
         km1=kg-1
         kp1=kg+1
         do j=1,ny
            jm1=mod(j+ny-2,ny)+1
            jp1=mod(j,ny)+1
            do i=1,nx
               im1=mod(i+nx-2,nx)+1
               ip1=mod(i,nx)+1


               ev(1,i,j,k)=0.
               t0=0.5*(mu(i,j,kg)+mu(ip1,j,kg))
               t3=gm*p(i,j,kg)/ro(i,j,kg)

               t1=(v(i,jp1,kg)-v(i,jm1,kg))/dy2
               t2=(w(i,j,kp1)-w(i,j,km1))/dz2
               ev(2,i,j,k)=t0/3.*(4.*(u(ip1,j,kg)-u(i,j,kg))/dx-
     1         (t1+(v(ip1,jp1,kg)-v(ip1,jm1,kg))/dy2+
     2         t2+(w(ip1,j,kp1)-w(ip1,j,km1))/dz2))

               t1=(u(i,jp1,kg)-u(i,jm1,kg))/dy2
               ev(3,i,j,k)=t0*((t1+(u(ip1,jp1,kg)-u(ip1,jm1,kg)/dy2))/2.
     1                    + (v(ip1,j,kg)-v(i,j,kg))/dx)

               t2=(u(i,j,kp1)-u(i,j,km1))/dz2
               ev(4,i,j,k)=t0*((t2+(u(ip1,j,kp1)-u(ip1,j,km1))/dz2)/2.+
     1         (w(ip1,j,kg)-w(i,j,kg))/dx)

               ev(5,i,j,k)=0.5*((u(ip1,j,kg)+u(i,j,kg))*ev(2,i,j,k)+
     1         (v(ip1,j,kg)+v(i,j,kg))*ev(3,i,j,k)+
     2         (w(ip1,j,kg)+w(i,j,kg))*ev(4,i,j,k))+
     3         t0/Pr/(gm-1.)*(gm*p(ip1,j,kg)/ro(ip1,j,kg)-t3)/dx

c     ************************************************************   
               fv(1,i,j,k)=0.0
               t0=(mu(i,j,kg)+mu(i,jp1,kg))/2.

               t1=(v(ip1,j,kg)-v(im1,j,kg))/dx2
               fv(2,i,j,k)=t0*(((v(ip1,jp1,kg)-v(im1,jp1,kg))+t1)/2.+
     1         (u(i,jp1,kg)-u(i,j,kg))/dy)

               t1=(u(ip1,j,kg)-u(im1,j,kg))/dx2
               t2=(w(i,j,kp1)-w(i,j,km1))/dz2
               fv(3,i,j,k)=t0/3.*(4.*(v(i,jp1,kg)-v(i,j,kg))/dy-
     1         ((u(ip1,jp1,kg)-u(im1,jp1,kg))/dx2+t1+
     2         (w(i,jp1,kp1)-w(i,jp1,km1))+t2))

               fv(4,i,j,k)=t0*(
     1         0.5*((u(ip1,jp1,kg)-u(im1,jp1,kg))/dx2+t1)+
     2         (w(i,jp1,kg)-w(i,j,kg))/dy)

               fv(5,i,j,k)=0.5*(
     1         (u(i,jp1,kg)+u(i,j,kg))*fv(2,i,j,k)+
     2         (v(i,jp1,kg)+v(i,j,kg))*fv(3,i,j,k)+
     3         (w(i,jp1,kg)+w(i,j,kg))*fv(4,i,j,k))+
     4         t0/Pr/(gm-1.)*(gm*p(i,jp1,kg)/ro(i,jp1,kg)-t3)/dy

C     *************************************************************

               gv(1,i,j,kg)=0.0
               t0=(mu(i,j,kg)+mu(i,j,kp1))/2.
               t1=(w(ip1,j,kg)-w(im1,j,kg))/dx2  
               gv(2,i,j,kg)=t0*(
     1         ((w(ip1,j,kp1)-w(im1,j,kp1))/dx2+t1)/2.+
     2         (u(i,j,kp1)-w(i,j,kg))/dz)

               t1=(w(i,jp1,kg)-w(i,jm1,kg))/dy2
               gv(3,i,j,kg)=t0*(
     1         ((w(i,jp1,kp1)-w(i,jm1,kp1))/dy2+t1)/2.+
     2         (v(i,j,kp1)-v(i,j,kg))/dz)

               t1=(u(ip1,j,kg)-u(im1,j,kg))/dx2
               t2=(v(i,jp1,kg)-v(i,jm1,kg))/dy2
               gv(4,i,j,kg)=t0/3.*(4.*(w(i,j,kp1)-w(i,j,kg))/dz-
     1         ((u(ip1,j,kp1)-u(im1,j,kp1))/dx2+t1+
     2          (v(i,jp1,kp1)-v(i,jm1,kp1))/dy2+t2))

                gv(5,i,j,kg)=0.5*(
     1          (u(i,j,kp1)+u(i,j,kg))*gv(2,i,j,kg)+
     2          (v(i,j,kp1)+v(i,j,kg))*gv(3,i,j,kg)+
     4          (w(i,j,kp1)+w(i,j,kg))*gv(4,i,j,kg))+
     5          t0/Pr/(gm-1.)*(gm*p(i,j,kp1)/ro(i,j,kp1)-t3)/dz

                

             enddo
          enddo
       enddo
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
!$OMP END PARALLEL DO
#endif 


     
        return
      end

      subroutine bi_cgstab_block(x,b,a,axp,ayp,azp,axm,aym,azm,
     1                             epsilon,nb,nx,ny,nzl)
c       /* Bi-CGSTAB */
      implicit none


      integer nb,nx,ny,nzl,kit, i,j,k,l,m
      integer kg, ierr


      real(kind=8) b(nb,nx,ny,nzl), a(nb,nb,nx,ny,nzl)
      real(kind=8) axp(nb,nb,nx,ny,nzl), ayp(nb,nb,nx,ny,nzl),
     1       azp(nb,nb,nx,ny,nzl)
      real(kind=8) axm(nb,nb,nx,ny,nzl), aym(nb,nb,nx,ny,nzl),
     1       azm(nb,nb,nx,ny,nzl)
      real(kind=8) x(nb,nx,ny,nzl+2)
      real(kind=8) p(nb,nx,ny,nzl+2), r(nb,nx,ny,nzl+2),
     1       rhat(nb,nx,ny,nzl+2)
      real(kind=8) v(nb,nx,ny,nzl+2), t(nb,nx,ny,nzl+2)

      real(kind=8) epsilon,r2,alpha,beta,rho,oldrho,omega,oldomega
      real(kind=8) r_r,target,tmp,tmp1
      real(kind=8) rh_r,rh_v,t_r,t_t


C       Fill ghost points in X
        call fill1(x,nb,nx,ny,nzl)
        
        kit = 0
!$OMP PARALLEL DO PRIVATE(l,i,j,k)
        do k=1,nzl+2
           do j=1,ny
              do i=1,nx
                 do l=1,nb
                    r(l,i,j,k) = 0.0
                    p(l,i,j,k) = 0.
                    v(l,i,j,k) = 0.
                    t(l,i,j,k) = 0.
                 enddo
              enddo
           enddo
        enddo
!$OMP END PARALLEL DO

      call mat_times_vec(r,x,
     1           a,axp,ayp,azp,axm,aym,azm,nb,nx,ny,nzl)

C       Local r*r norm
        r2=0.0
!$OMP PARALLEL DO PRIVATE(l,i,j,k) REDUCTION(+:r2)
        do k=2,nzl+1
           do j=1,ny
              do i=1,nx
                 do l=1,nb
                    r(l,i,j,k) = b(l,i,j,k-1) - r(l,i,j,k)
                    r2 =r2+r(l,i,j,k)**2
                    rhat(l,i,j,k) = r(l,i,j,k)
                 enddo
              enddo
           enddo
        enddo
!$OMP END PARALLEL DO

        r_r = r2

        oldrho = 1.0
        oldomega = 1.0
        alpha = 1.0

        write (6,'(2(1X,G20.11))') r_r,epsilon

        target =  r_r * epsilon*epsilon
C       target =  epsilon
        DO WHILE (r_r .GT. target)

                kit = kit + 1

C       Compute local rhat * r norm
                rho=0.
!$OMP PARALLEL DO PRIVATE(l,i,j,k) REDUCTION(+:rho)
                do k=2,nzl+1
                   do j=1,ny
                      do i=1,nx
                         do l=1,nb
                            rho =rho+rhat(l,i,j,k)*r(l,i,j,k)
                         enddo
                      enddo
                   enddo
                enddo
!$OMP END PARALLEL DO

C       Compute global rhat * r norm
                beta = (rho / oldrho)*(alpha/oldomega)

!$OMP PARALLEL DO PRIVATE(l,i,j,k)
                do k=2,nzl+1
                   do j=1,ny
                      do i=1,nx
                         do l=1,nb
                            p(l,i,j,k) = r(l,i,j,k) + beta * 
     1                      ( p(l,i,j,k) - oldomega * v(l,i,j,k) )
                         enddo
                      enddo
                   enddo
                enddo
!$OMP END PARALLEL DO

C       Fill ghost points in P
                call fill1(p,nb,nx,ny,nzl)

                call mat_times_vec(v,p,
     1                   a,axp,ayp,azp,axm,aym,azm,nb,nx,ny,nzl)

C       Compute local rhat * v norm
                tmp=0.
!$OMP PARALLEL DO PRIVATE(l,i,j,k) REDUCTION(+:tmp)
                do k=2,nzl+1
                   do j=1,ny
                      do i=1,nx
                         do l=1,nb
                            tmp=tmp+rhat(l,i,j,k)*v(l,i,j,k)
                         enddo
                      enddo
                   enddo
                enddo
!$OMP END PARALLEL DO

                alpha = rho / tmp

!$OMP PARALLEL DO PRIVATE(l,i,j,k)
                do k=2,nzl+1
                   do j=1,ny
                      do i=1,nx
                         do l=1,nb
                            r(l,i,j,k)=r(l,i,j,k)-
     1                                    alpha * v(l,i,j,k)
                         enddo
                      enddo
                   enddo
                enddo
!$OMP END PARALLEL DO

C       Fill ghost points in R
                call fill1(r,nb,nx,ny,nzl)

                call mat_times_vec(t,r,
     1                         a,axp,ayp,azp,axm,aym,azm,nb,nx,ny,nzl)

C       Compute local t * r and t * t norms
                tmp=0.
                tmp1=0.
!$OMP PARALLEL DO PRIVATE(l,i,j,k) REDUCTION(+:tmp,tmp1)
                do k=2,nzl+1
                   do j=1,ny
                      do i=1,nx
                         do l=1,nb
                            tmp=tmp+t(l,i,j,k)*r(l,i,j,k)
                            tmp1=tmp1+t(l,i,j,k)*t(l,i,j,k)
                         enddo
                      enddo
                   enddo
                enddo
!$OMP END PARALLEL DO

                omega = tmp/tmp1

!$OMP PARALLEL DO PRIVATE(l,i,j,k)
                do k=2,nzl+1
                   do j=1,ny
                      do i=1,nx
                         do l=1,nb
                            x(l,i,j,k)= x(l,i,j,k)+
     1                      alpha*p(l,i,j,k)+omega*r(l,i,j,k)
                            r(l,i,j,k)=r(l,i,j,k)-omega*t(l,i,j,k)
                         enddo
                      enddo
                   enddo
                enddo
!$OMP END PARALLEL DO

                oldrho = rho
                oldomega = omega

c       Compute local r * r norm
                r2=0
!$OMP PARALLEL DO PRIVATE(l,i,j,k) REDUCTION(+:r2)
                do k=2,nzl+1
                   do j=1,ny
                      do i=1,nx
                         do l=1,nb
                            r2=r2+r(l,i,j,k)*r(l,i,j,k)
                         enddo
                      enddo
                   enddo
                enddo
!$OMP END PARALLEL DO

c       Compute global r * r norm
                r_r = r2

        END DO

!       write (6,'(1X,A,I6,A)')'convergence after ',kit,' iterations.'
        write (20,*)'  |residual|^2 =',r2

        RETURN
        END

        subroutine mat_times_vec(y,x,a,axp,ayp,azp,axm,aym,azm,
     $  nb,nx,ny,nzl)
        implicit none
        integer nb,nx,ny,nzl,i,j,k,m,l,kit,im1,ip1,jm1,jp1,km1,kp1,kg

        real(kind=8) y(nb,nx,ny,nzl+2),x(nb,nx,ny,nzl+2)

        real(kind=8) a(nb,nb,nx,ny,nzl),
     1  axp(nb,nb,nx,ny,nzl),ayp(nb,nb,nx,ny,nzl),azp(nb,nb,nx,ny,nzl),
     2  axm(nb,nb,nx,ny,nzl),aym(nb,nb,nx,ny,nzl),azm(nb,nb,nx,ny,nzl)

!$OMP PARALLEL DO PRIVATE(m,l,i,j,k,im1,ip1,jm1,jp1,km1,kp1,kg)
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
               do l=1,nb
                  y(l,i,j,kg)=0.0
                  do m=1,nb
                     y(l,i,j,kg)=y(l,i,j,kg)+
     1               a(l,m,i,j,k)*x(m,i,j,kg)+
     2               axp(l,m,i,j,k)*x(m,ip1,j,kg)+
     3               ayp(l,m,i,j,k)*x(m,i,jp1,kg)+
     4               azp(l,m,i,j,k)*x(m,i,j,kp1)+
     5               axm(l,m,i,j,k)*x(m,im1,j,kg)+
     6               aym(l,m,i,j,k)*x(m,i,jm1,kg)+
     7               azm(l,m,i,j,k)*x(m,i,j,km1)
                  enddo
               enddo
            enddo
         enddo
        enddo
!$OMP END PARALLEL DO

c        y=x
c        where (mask) y=tmp

        return
        end

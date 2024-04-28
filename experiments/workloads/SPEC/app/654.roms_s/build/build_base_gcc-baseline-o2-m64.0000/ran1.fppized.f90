      SUBROUTINE ran1_s (harvest)
      USE mod_kinds
      real(r8), intent(out) :: harvest
      print 4, "ran1_s"
   4  FORMAT(a, " should not be called by this  benchmark")
      stop
      END SUBROUTINE ran1_s

      SUBROUTINE ran1_v (harvest)
      USE mod_kinds
      real(r8), dimension(:), intent(out) :: harvest
      print 4, "ran1_v"
   4  FORMAT(a, " should not be called by this  benchmark")
      stop
      END SUBROUTINE ran1_v


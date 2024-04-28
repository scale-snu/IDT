module cctk_MemAlloc
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_Malloc (ptr, size, line, file)
       use iso_c_binding
       implicit none
       integer(c_size_t) ptr
       integer      size
       integer      line
       character(c_char) file(*)
     end subroutine CCTK_Malloc
     subroutine CCTK_Free (ptr)
       use iso_c_binding
       implicit none
       integer(c_size_t) ptr
     end subroutine CCTK_Free
     !  CCTK_TotalMemory is already declared in cctk_Malloc1
  end interface
end module cctk_MemAlloc

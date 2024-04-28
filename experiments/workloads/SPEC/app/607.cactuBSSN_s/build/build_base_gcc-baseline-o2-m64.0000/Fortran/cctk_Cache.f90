module cctk_Cache
  use iso_c_binding
  implicit none
  interface
     subroutine Util_CacheMalloc (pointer, index, size, realstart)
       use iso_c_binding
       implicit none
       integer(c_size_t) pointer
       integer(c_int)      index
       integer(c_int)      size
       integer(c_size_t) realstart
     end subroutine Util_CacheMalloc
  end interface
end module cctk_Cache

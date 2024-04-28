module util_Table
  use iso_c_binding
       use iso_c_binding 
  implicit none
  interface
     ! create/destroy
     subroutine Util_TableCreate (handle, flags) bind(c,name="util_tablecreate_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               handle
       integer(c_int) ::               flags
     end subroutine Util_TableCreate
     subroutine Util_TableClone (ierr, handle) bind(c,name="util_tableclone_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               ierr
       integer(c_int) ::               handle
     end subroutine Util_TableClone
     subroutine Util_TableDestroy (ierr, handle) bind(c,name="util_tabledestroy_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               ierr
       integer(c_int) ::               handle
     end subroutine Util_TableDestroy
     ! query
     subroutine Util_TableQueryFlags (flags, handle) bind(c,name="util_tablequeryflags_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               flags
       integer(c_int) ::               handle
     end subroutine Util_TableQueryFlags
     subroutine Util_TableQueryNKeys (nkeys, handle) bind(c,name="util_tablequerynkeys_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               nkeys
       integer(c_int) ::               handle
     end subroutine Util_TableQueryNKeys
     subroutine Util_TableQueryMaxKeyLength (maxkeylength, handle) bind(c,name="util_tablequerymaxkeylength_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               maxkeylength
       integer(c_int) ::               handle
     end subroutine Util_TableQueryMaxKeyLength
     subroutine Util_TableQueryValueInfo &
          (keyinfo, handle, type_code, N_elements, key)
       implicit none
       integer              keyinfo
       integer               handle
       INTEGER              type_code
       INTEGER              N_elements
       character key(*) 
     end subroutine Util_TableQueryValueInfo
     ! misc stuff
     subroutine Util_TableDeleteKey (ierr, handle, key) bind(c,name="util_tabledeletekey_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               ierr
       integer(c_int) ::               handle
       character(kind=c_char) :: key(*)
     end subroutine Util_TableDeleteKey
     ! convenience routines to create and/or set from a parameter-file string
     subroutine Util_TableCreateFromString (handle, string) bind(c,name="util_tablecreatefromstring_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               handle
       character(c_char) :: string(*) 
     end subroutine Util_TableCreateFromString
     ! set/get a C-style null-terminated character string
     subroutine Util_TableSetString (info, handle, string, key) bind(c,name="util_tablesetstring_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       character(c_char) :: string(*) 
       character(c_char) :: key(*) 
     end subroutine Util_TableSetString
     subroutine Util_TableGetString (length, handle, buffer, key)
       implicit none
       integer               length
       integer              handle
       character buffer(*) 
       character key(*) 
     end subroutine Util_TableGetString
     ! set/get generic types described by CCTK_VARIABLE_* type codes
     subroutine Util_TableSetGeneric (info, handle, type_code, value_ptr, key) bind(c,name="util_tablesetgeneric_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               type_code
       integer(c_size_t) :: value_ptr
       character(c_char) :: key(*) 
     end subroutine Util_TableSetGeneric
     subroutine Util_TableSetGenericArray &
          (info, handle, type_code, N_elements, array, key)
       implicit none
       integer     info
       integer      handle
       integer              type_code
       integer               N_elements
       integer array(*)
       character key(*) 
     end subroutine Util_TableSetGenericArray
     subroutine Util_TableGetGeneric &
          (length, handle, type_code, value_ptr, key)
       implicit none
       integer               length
       integer              handle
       integer               type_code
       integer          value_ptr
       character key(*) 
     end subroutine Util_TableGetGeneric
     subroutine Util_TableGetGenericArray &
          (length, handle, type_code, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               type_code
       integer               N_elements
       integer          array(*)
       character key(*) 
     end subroutine Util_TableGetGenericArray
     ! set routines
     ! pointers
     subroutine Util_TableSetPointer (info, handle, value, key) bind(c,name="util_tablesetpointer_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_size_t) :: value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetPointer
     subroutine Util_TableSetFPointer (info, handle, value, key) bind(c,name="util_tablesetfpointer_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_size_t) ::        value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetFPointer
     ! integer(c_int) ::s
     subroutine Util_TableSetByte (info, handle, value, key) bind(c,name="util_tablesetbyte_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       INTEGER(c_int8_t) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetByte
     subroutine Util_TableSetInt (info, handle, value, key) bind(c,name="util_tablesetint_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       INTEGER(c_int) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetInt
     subroutine Util_TableSetInt1 (info, handle, value, key) bind(c,name="util_tablesetint1_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       INTEGER(c_int8_t) ::            value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetInt1
     subroutine Util_TableSetInt2 (info, handle, value, key) bind(c,name="util_tablesetint2_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       INTEGER(c_int16_t) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetInt2
     subroutine Util_TableSetInt4 (info, handle, value, key) bind(c,name="util_tablesetint4_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       INTEGER(c_int) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetInt4
     subroutine Util_TableSetInt8 (info, handle, value, key) bind(c,name="util_tablesetint8_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       INTEGER(c_int64_t) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetInt8
     ! real numbers
     subroutine Util_TableSetReal (info, handle, value, key) bind(c,name="util_tablesetreal_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       REAL(c_double) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetReal
     subroutine Util_TableSetReal4 (info, handle, value, key) bind(c,name="util_tablesetreal4_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       REAL(c_float)  ::               value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetReal4
     subroutine Util_TableSetReal8 (info, handle, value, key) bind(c,name="util_tablesetreal8_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       REAL(c_double) ::            value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetReal8
     subroutine Util_TableSetReal16 (info, handle, value, key) bind(c,name="util_tablesetreal16_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       REAL(c_double) ::           value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetReal16
     ! complex numbers
     subroutine Util_TableSetComplex (info, handle, value, key) bind(c,name="util_tablesetcomplex_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       COMPLEX(c_double_complex) ::          value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetComplex
     subroutine Util_TableSetComplex8 (info, handle, value, key) bind(c,name="util_tablesetcomplex8_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       COMPLEX(c_float_complex) ::         value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetComplex8
     subroutine Util_TableSetComplex16 (info, handle, value, key) bind(c,name="util_tablesetcomplex16_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       COMPLEX(c_double_complex) ::        value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetComplex16
     subroutine Util_TableSetComplex32 (info, handle, value, key) bind(c,name="util_tablesetcomplex32_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       COMPLEX(c_double_complex) ::        value
       character(c_char) :: key(*) 
     end subroutine Util_TableSetComplex32
     ! arrays of pointers
     subroutine Util_TableSetPointerArray &
          (info, handle, N_elements, array, key)
       implicit none
       integer               info
       integer               handle
       integer               N_elements
       integer array(*)
       character key(*) 
     end subroutine Util_TableSetPointerArray
     subroutine Util_TableSetFPointerArray &
          (info, handle, N_elements, array, key)
       implicit none
       integer               info
       integer               handle
       integer               N_elements
       integer         array(*)
       character key(*) 
     end subroutine Util_TableSetFPointerArray
     ! arrays of integer(c_int) ::s
     subroutine Util_TableSetByteArray (info, handle, N_elements, array, key) bind(c,name="util_tablesetbytearray_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_int8_t) ::              array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetByteArray
     subroutine Util_TableSetIntArray (info, handle, N_elements, array, key) bind(c,name="util_tablesetintarray_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_int) ::              array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetIntArray
     subroutine Util_TableSetInt1Array (info, handle, N_elements, array, key) bind(c,name="util_tablesetint1array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_int8_t) ::             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetInt1Array
     subroutine Util_TableSetInt2Array (info, handle, N_elements, array, key) bind(c,name="util_tablesetint2array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_int16_t)             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetInt2Array
     subroutine Util_TableSetInt4Array (info, handle, N_elements, array, key) bind(c,name="util_tablesetint4array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_int) ::             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetInt4Array
     subroutine Util_TableSetInt8Array (info, handle, N_elements, array, key) bind(c,name="util_tablesetint8array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_int64_t) ::             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetInt8Array
     ! arrays of real numbers
     subroutine Util_TableSetRealArray (info, handle, N_elements, array, key) bind(c,name="util_tablesetrealarray_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       REAL(c_double) ::             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetRealArray
     subroutine Util_TableSetReal4Array (info, handle, N_elements, array, key) bind(c,name="util_tablesetreal4array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       REAL(c_float)  ::               array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetReal4Array
     subroutine Util_TableSetReal8Array (info, handle, N_elements, array, key) bind(c,name="util_tablesetreal8array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       REAL(c_double) ::            array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetReal8Array
     subroutine Util_TableSetReal16Array (info, handle, N_elements, array, key) bind(c,name="util_tablesetreal16array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               info
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       REAL(c_double) ::           array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableSetReal16Array
     ! arrays of complex numbers
     subroutine Util_TableSetComplexArray &
          (info, handle, N_elements, array, key)
       implicit none
       integer               info
       integer               handle
       integer               N_elements
       COMPLEX(8)          array(*)
       character key(*) 
     end subroutine Util_TableSetComplexArray
     subroutine Util_TableSetComplex8Array &
          (info, handle, N_elements, array, key)
       implicit none
       integer               info
       integer               handle
       integer               N_elements
       COMPLEX(4)         array(*)
       character key(*) 
     end subroutine Util_TableSetComplex8Array
     subroutine Util_TableSetComplex16Array &
          (info, handle, N_elements, array, key)
       implicit none
       integer               info
       integer               handle
       integer               N_elements
       COMPLEX(8)         array(*)
       character key(*) 
     end subroutine Util_TableSetComplex16Array
     subroutine Util_TableSetComplex32Array &
          (info, handle, N_elements, array, key)
       implicit none
       integer               info
       integer               handle
       integer               N_elements
       COMPLEX(8)         array(*)
       character key(*) 
     end subroutine Util_TableSetComplex32Array
     ! get routines
     ! pointers
     subroutine Util_TableGetPointer (length, handle, value, key) bind(c,name="util_tablegetpointer_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       integer(c_size_t)          value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetPointer
     subroutine Util_TableGetFPointer (length, handle, value, key) bind(c,name="util_tablegetfpointer_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       integer(c_size_t)         value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetFPointer
     ! integer ::s
     subroutine Util_TableGetByte (length, handle, value, key) bind(c,name="util_tablegetbyte_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       INTEGER(c_int8_t) ::           value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetByte
     subroutine Util_TableGetInt (length, handle, value, key) bind(c,name="util_tablegetint_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       INTEGER(c_int) ::              value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetInt
     subroutine Util_TableGetInt1 (length, handle, value, key) bind(c,name="util_tablegetint1_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       INTEGER(c_int8_t) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetInt1
     subroutine Util_TableGetInt2 (length, handle, value, key) bind(c,name="util_tablegetint2_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       INTEGER(c_int16_t) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetInt2
     subroutine Util_TableGetInt4 (length, handle, value, key) bind(c,name="util_tablegetint4_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       INTEGER(c_int) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetInt4
     subroutine Util_TableGetInt8 (length, handle, value, key) bind(c,name="util_tablegetint8_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       INTEGER(c_int64_t) ::             value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetInt8
     ! real numbers
     subroutine Util_TableGetReal (length, handle, value, key) bind(c,name="util_tablegetreal_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       REAL(c_double) ::          value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetReal
     subroutine Util_TableGetReal4 (length, handle, value, key) bind(c,name="util_tablegetreal4_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       REAL(c_float) ::            value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetReal4
     subroutine Util_TableGetReal8 (length, handle, value, key) bind(c,name="util_tablegetreal8_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       REAL(c_double) ::            value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetReal8
     subroutine Util_TableGetReal16 (length, handle, value, key) bind(c,name="util_tablegetreal16_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       REAL(c_double) ::          value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetReal16
     ! complex numbers
     subroutine Util_TableGetComplex (length, handle, value, key) bind(c,name="util_tablegetcomplex_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       COMPLEX(c_double_complex) ::         value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetComplex
     subroutine Util_TableGetComplex8 (length, handle, value, key) bind(c,name="util_tablegetcomplex8_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       COMPLEX(c_float_complex) ::     value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetComplex8
     subroutine Util_TableGetComplex16 (length, handle, value, key) bind(c,name="util_tablegetcomplex16_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       COMPLEX(c_double_complex) ::    value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetComplex16
     subroutine Util_TableGetComplex32 (length, handle, value, key) bind(c,name="util_tablegetcomplex32_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       COMPLEX(c_double_complex) ::    value
       character(c_char) :: key(*) 
     end subroutine Util_TableGetComplex32
     ! arrays of pointers
     subroutine Util_TableGetPointerArray &
          (length, handle, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               N_elements
       integer          array(*)
       character :: key(*) 
     end subroutine Util_TableGetPointerArray
     subroutine Util_TableGetFPointerArray &
          (length, handle, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               N_elements
       integer         array(*)
       character key(*) 
     end subroutine Util_TableGetFPointerArray
     ! integer(c_int) ::s
     subroutine Util_TableGetByteArray (length, handle, N_elements, array, key) bind(c,name="util_tablegetbytearray_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_size_t) ::          array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableGetByteArray
     subroutine Util_TableGetIntArray (length, handle, N_elements, array, key) bind(c,name="util_tablegetintarray_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_size_t) ::           array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableGetIntArray
     subroutine Util_TableGetInt1Array (length, handle, N_elements, array, key) bind(c,name="util_tablegetint1array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_size_t) ::             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableGetInt1Array
     subroutine Util_TableGetInt2Array (length, handle, N_elements, array, key) bind(c,name="util_tablegetint2array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_int16_t)             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableGetInt2Array
     subroutine Util_TableGetInt4Array (length, handle, N_elements, array, key) bind(c,name="util_tablegetint4array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_int) ::             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableGetInt4Array
     subroutine Util_TableGetInt8Array (length, handle, N_elements, array, key) bind(c,name="util_tablegetint8array_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       INTEGER(c_int64_t) ::             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableGetInt8Array
     ! real numbers
     subroutine Util_TableGetRealArray (length, handle, N_elements, array, key) bind(c,name="util_tablegetrealarray_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::               length
       integer(c_int) ::               handle
       integer(c_int) ::               N_elements
       REAL(c_double) ::             array(*)
       character(c_char) :: key(*) 
     end subroutine Util_TableGetRealArray
     subroutine Util_TableGetReal4Array &
          (length, handle, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               N_elements
       REAL(4)            array(*)
       character key(*) 
     end subroutine Util_TableGetReal4Array
     subroutine Util_TableGetReal8Array &
          (length, handle, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               N_elements
       REAL(8)            array(*)
       character key(*) 
     end subroutine Util_TableGetReal8Array
     subroutine Util_TableGetReal16Array &
          (length, handle, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               N_elements
       REAL(8)           array(*)
       character key(*) 
     end subroutine Util_TableGetReal16Array
     ! complex numbers
     subroutine Util_TableGetComplexArray &
          (length, handle, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               N_elements
       COMPLEX(8)          array(*)
       character :: key(*) 
     end subroutine Util_TableGetComplexArray
     subroutine Util_TableGetComplex8Array &
          (length, handle, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               N_elements
       COMPLEX(4)         array(*)
       character key(*) 
     end subroutine Util_TableGetComplex8Array
     subroutine Util_TableGetComplex16Array &
          (length, handle, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               N_elements
       COMPLEX(8)        array(*)
       character key(*) 
     end subroutine Util_TableGetComplex16Array
     subroutine Util_TableGetComplex32Array &
          (length, handle, N_elements, array, key)
       implicit none
       integer               length
       integer               handle
       integer               N_elements
       COMPLEX(8)        array(*)
       character key(*) 
     end subroutine Util_TableGetComplex32Array
     ! Table Iterator API
     ! create/destroy
     subroutine Util_TableItCreate (ihandle, handle)
       implicit none
       integer               ihandle
       integer               handle
     end subroutine Util_TableItCreate
     subroutine Util_TableItClone (newihandle, ihandle)
       implicit none
       integer               newihandle
       integer               ihandle
     end subroutine Util_TableItClone
     subroutine Util_TableItDestroy (ierr, ihandle)
       implicit none
       integer               ierr
       integer               ihandle
     end subroutine Util_TableItDestroy
     ! test for null-pointer state
     subroutine Util_TableItQueryIsNull (info, ihandle)
       implicit none
       integer               info
       integer               ihandle
     end subroutine Util_TableItQueryIsNull
     subroutine Util_TableItQueryIsNonNull (info, ihandle)
       implicit none
       integer               info
       integer               ihandle
     end subroutine Util_TableItQueryIsNonNull
     ! query what the iterator points to
     subroutine Util_TableItQueryTableHandle (handle, ihandle)
       implicit none
       integer               handle
       integer               ihandle
     end subroutine Util_TableItQueryTableHandle
     subroutine Util_TableItQueryKeyValueInfo &
          (length, ihandle, key_buffer, type_code, N_elements)
       implicit none
       integer               length
       integer               ihandle
       character key_buffer(*) 
       INTEGER              type_code
       INTEGER              N_elements
     end subroutine Util_TableItQueryKeyValueInfo
     ! change value of iterator
     subroutine Util_TableItAdvance (info, ihandle)
       implicit none
       integer               info
       integer               ihandle
     end subroutine Util_TableItAdvance
     subroutine Util_TableItResetToStart (info, ihandle)
       implicit none
       integer               info
       integer               ihandle
     end subroutine Util_TableItResetToStart
     subroutine Util_TableItSetToNull (ierr, ihandle)
       implicit none
       integer               ierr
       integer               ihandle
     end subroutine Util_TableItSetToNull
     subroutine Util_TableItSetToKey (ierr, ihandle, key)
       implicit none
       integer               ierr
       integer               ihandle
       character :: key(*) 
     end subroutine Util_TableItSetToKey
  end interface
end module util_Table

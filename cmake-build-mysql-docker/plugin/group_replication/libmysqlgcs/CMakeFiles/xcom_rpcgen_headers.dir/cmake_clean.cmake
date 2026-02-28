file(REMOVE_RECURSE
  "xdr_gen/xcom_vp.h"
  "xdr_gen/xcom_vp_xdr.c"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/xcom_rpcgen_headers.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

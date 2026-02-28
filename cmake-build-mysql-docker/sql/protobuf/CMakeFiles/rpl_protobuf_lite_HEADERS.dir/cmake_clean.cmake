file(REMOVE_RECURSE
  "generated/protobuf_lite/replication_asynchronous_connection_failover.pb.cc"
  "generated/protobuf_lite/replication_asynchronous_connection_failover.pb.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/rpl_protobuf_lite_HEADERS.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

file(REMOVE_RECURSE
  "../generated/protobuf_lite/replication_group_member_actions.pb.cc"
  "../generated/protobuf_lite/replication_group_member_actions.pb.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/gr_protobuf_lite_HEADERS.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

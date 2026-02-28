file(REMOVE_RECURSE
  "../../archive_output_directory/liboci_common.a"
  "../../archive_output_directory/liboci_common.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/oci_common.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

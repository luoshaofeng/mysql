file(REMOVE_RECURSE
  "../../../../archive_output_directory/libndbapi.a"
  "../../../../archive_output_directory/libndbapi.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/ndbapi.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

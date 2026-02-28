file(REMOVE_RECURSE
  "../../archive_output_directory/libzstd.a"
  "../../archive_output_directory/libzstd.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/zstd.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

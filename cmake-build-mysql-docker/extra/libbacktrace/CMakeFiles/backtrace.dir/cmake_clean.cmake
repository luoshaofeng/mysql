file(REMOVE_RECURSE
  "../../archive_output_directory/libbacktrace.a"
  "../../archive_output_directory/libbacktrace.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/backtrace.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

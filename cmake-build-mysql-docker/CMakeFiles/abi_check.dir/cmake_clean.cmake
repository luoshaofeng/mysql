file(REMOVE_RECURSE
  "archive_output_directory"
  "library_output_directory"
  "CMakeFiles/abi_check"
  "abi_check.out"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/abi_check.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

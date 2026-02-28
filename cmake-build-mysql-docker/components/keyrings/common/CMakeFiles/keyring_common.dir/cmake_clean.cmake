file(REMOVE_RECURSE
  "../../../archive_output_directory/libkeyring_common.a"
  "../../../archive_output_directory/libkeyring_common.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/keyring_common.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

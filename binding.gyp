{
  "targets": [
    {
      "target_name": "nan-async-binding",
      "sources": [
        "./nan_async.cc"
      ],
      "include_dirs" : [ "<!(node -e \"require('nan')\")" ]
    }
  ]
}

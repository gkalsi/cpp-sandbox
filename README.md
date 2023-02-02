# C++ Sandbox
All sketches are built using [Bazel](https://bazel.build/).

To list all availabe targets:
```
 $ bazel query //...
```

To run a particular target:
```
 $ bazel run <target_name>
```

Targets are generally named after their top level-directory. For example to
run the `std_filesystem` target one would invoke `bazel run std_filesystem`.
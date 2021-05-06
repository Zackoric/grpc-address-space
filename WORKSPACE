workspace(name = "embed-envoy-example")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "gtest",
    sha256 = "9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb",
    strip_prefix = "googletest-release-1.10.0",
    url = "https://github.com/google/googletest/archive/release-1.10.0.tar.gz",
)

########################################################################

local_repository(
    name = "envoy",
    path = "envoy",
)

load("@envoy//bazel:api_binding.bzl", "envoy_api_binding")

envoy_api_binding()

load("@envoy//bazel:api_repositories.bzl", "envoy_api_dependencies")

envoy_api_dependencies()

load("@envoy//bazel:repositories.bzl", "envoy_dependencies")

envoy_dependencies()

load("@envoy//bazel:repositories_extra.bzl", "envoy_dependencies_extra")

envoy_dependencies_extra()

load("@envoy//bazel:dependency_imports.bzl", "envoy_dependency_imports")

envoy_dependency_imports()

########################################################################

http_archive(
    name = "com_github_grpc_grpc",
    sha256 = "27dd2fc5c9809ddcde8eb6fa1fa278a3486566dfc28335fca13eb8df8bd3b958",
    strip_prefix = "grpc-1.35.0",
    urls = ["https://github.com/grpc/grpc/archive/v1.35.0.tar.gz"],
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

#For some reason, this line makes the build fail with zlib errors :(
#grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")

grpc_extra_deps()

http_archive(
    name = "stout-grpc",
    sha256 = "6ed6acc3928e5517eaf132a06454ed459e5a989ca5fbf5b128ca9b1f966dc1df",
    strip_prefix = "stout-grpc-0.7.0",
    url = "https://github.com/benh/stout-grpc/archive/0.7.0.tar.gz",
)

load("@stout-grpc//bazel:stout_grpc_deps.bzl", "stout_grpc_deps")

stout_grpc_deps()

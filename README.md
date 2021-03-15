# gRPC + (Immutable) Address Space

> NOTE: uses the repository at
> [3rdparty/embed-envoy-example](https://github.com/3rdparty/embed-envoy-example)
> as a starting point for embedding Envoy as a C++ library.

Provides a library wrapper on top of (stout-)gRPC that includes an
immutable address space for storing data that allows passing a
***reference*** (see (this)[] protocol buffer message definition) in
gRPC calls instead of the actual protocol buffer itself.

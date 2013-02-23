Render Backend (RB)
===================

This project defines a render backend API and provides two implementations of it. 

1. The `null' implementation actually does nothing. It simply ensures a dummy
comportment that may be valid in all situations. 

2. The ogl3 implementation is based on the OpenGL3.3 API and consequently,
only softwares executed on systems with a compatible driver can rely on it.
Note that the caller has to ensure that a valid OpenGL3.3 window exists
before invoking any of the render backend functions.

In addition, this project proposes a "render backend interface" library (rbi)
that load dynamically any render backend implementation. However one can use
the rb libraries without using this "rbi" since public render backend headers
are provided through the rb.h file.

# Graph Manipulator


This is an mini service where client can generate random undirected graph with shortest pathes

Options are:

- vertex name length (like: Bob, Bobnoxious, ...)
- count of nodes 
- edges weight ("power" of it's value set as length of characters)


**"Client" side implemented by**:

- pure js (websockets, event handlers mainly)
- D3 (graph visualization as svg object)
- graphlib-dot (deserialize incoming bits thread to graphlib object)

To build with CMakeLists pass library pathes to:

- OpenSSL (1.1.0+)
- LIBUV
- ZLIB
- Boost (1.67+)


For example On Windows x64, MVS2017:

cmake -DCMAKE_BUILD_TYPE=release 
-DLIBUV_ROOT_DIR=$LIBUV_PATH 
-DZLIB_ROOT_DIR=$ZLIB_ROOT_DIR 
-DZLIB_LIBRARY=$ZLIB_LIB_PATH 
-DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_PATH 
-DOPENSSL_ROOT_DIR=$OPENSSL_PATH 
-DBOOST_ROOT=$BOOST_PATH
-G "Visual Studio 15 2017 Win64" .. > log_cmake.txt 2>&1

In all cases, average errors connected with OPENSSL version and it might be errors for export symbols in uWS.lib like:

076 00000000 UNDEF notype () External | SSL_CTX_set_options 
077 00000000 UNDEF notype () External | SSL_CTX_new 
078 00000000 UNDEF notype () External | SSL_CTX_up_ref 
079 00000000 UNDEF notype () External | SSL_CTX_free 

Check your OPENSSL version.

build target:

cmake --build . --target PathMapper --config release > log_build.txt 2>&1

**UI (design) side was implemented by**:

- HTML, CSS, bootstrap (3.3.7)

**"Back-end" side was implemented by C++**:

- boost, graph library
- uWS (uWebsockets)
- json (nlohmann  json or JSON for Modern C++)

**Example**:

<pre>
<a href="https://github.com/NuclearRazor/GraphManipulator/blob/master/common/img/view.png"><img src="https://github.com/NuclearRazor/GraphManipulator/blob/master/common/img/view.png" align="middle">
</a>
</pre>

In production:

- various options on undirected graphs 
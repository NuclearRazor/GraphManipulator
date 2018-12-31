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

- OpenSSL
- Libuv
- Boost

For example On Windows x64, MVS2017:

cmake -DCMAKE_BUILD_TYPE=release cmake -DLIBUV_ROOT_DIR=C:/Users/ivan-/vcpkg/installed/x64-windows -DOPENSSL_ROOT_DIR=C:/OpenSSL-Win64-1.1.1/ -DBOOST_ROOT=C:/boost_1_68_0 -G "Visual Studio 15 2017 Win64" .. > log_cmake.txt 2>&1

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

- port input support
- various options on undirected graphs 
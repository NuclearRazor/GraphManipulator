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


**UI (design) side was implemented by**:

- HTML, CSS, bootstrap (3.3.7)

**"Back-end" side was implemented by C++**:

- boost, graph library
- uWS (uWebsockets)
- json (nlohmann  json or JSON for Modern C++)

**Example**:

<pre>
<a href="https://github.com/NuclearRazor/GraphManipulator/blob/master/common/img/view.png"><img src="https://github.com/NuclearRazor/GraphManipulator/blob/master/common/img/view.png" align="middle" height="450">
</a>
</pre>

In production:

- port input support
- various options on undirected graphs 
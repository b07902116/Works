# Design Report
- Main  
  It creates FileSystemCLI object and start to run the CLI. 
- FileSystemCLI  
  It keeps reading commands and perform them.  
  It handles the file system by maintaining two Directory: root and currentDir.
- Node  
  It is an abstract class and can be extended by Directory, File and Link.  
  It has its name and path.
- Directory  
  It has a list of nodes and its parent directory.  
  It needs to maintain its node_list and sometimes search nodes that have keyword in their name.
- File  
  It has file content given by command touch.
- Link  
  It has the name and path of the target directory given by command link.

## Software Design
![FOOP](FOOP.png)

- OCP for adding Link node  
  Adding Link class needs to implement the Link class itself, and modified the FileSystemCLI.
  But other node classes, Node, Directory, File, do not need any modification when Link node is added.
  So my design of node classes follow the OCP on accommodating the Link node.



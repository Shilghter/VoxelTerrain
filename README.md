VoxelTerrain
============

Simple voxelized terrain engine. Creates voxel terrain based on any heightmap you supply available for freelook and rendering tests using different methods. You can select the way your terrain is loaded into memory and how is it rendered. You can choose from set of additional performance optimizations and measure time of all the operations.

Currently Working
============

1. Opening heightmap and reading it into memory using simplified or complex algorithm.
  - Includes time measuring for these operations
  - Includes breaking terrain into CHUNK_SIZE x CHUNK_SIZE chunks
  - Includes checking for surface and empty/full chunked analyzis
2. Meshing chunks for different rendering methods
  - Meshing for triangles
  - Meshing for quads
  - Meshing for indexed quads
  - Meshing for optimized indexed quad surface
  - Includes active voxel, primitive and vertex counters
3. Rendering with different methods
  - Triangle rendering
  - Quad rendering
  - Indexed quad rendering
4. Optimizations
  - Primitive reducing using optimized meshing
  - Frustum and proximity culling
  - Include active chunks check on move
  
Immediate needs
============

1. Frustum and proximity culling
  - Needs fixing for accuarcy. For some reason sometimes chunks disappear when they should be visible. Visibility check is based on chunk centers generated as early as map loading. Uses spherical boundary for frustum and euclidean distance for proximity. Both have visible issues though not with the check itself, probably it's an issue with calculating chunk centers.
  - For the time beeing both cullings cannot be used together, but best visual and performance effects could probably be achieved when mixed. For instance, proximity could be set just a chunk away from far vision boundary allowing to load chunks that are not yet visible.
2. Memory release
- Maps as big as 512x512 require a lot of memory to load. Program needs a mechanism to clean unused memory objects, such as block table in each chunk containing active elements provided by heightmap loading algorithm. Mesh geometry could also be released as soon as it's loaded into VBO objects. Removing both memory loads would leave us with only VBO memory usage so VRAM + some system RAM for large maps. 
3. Threading
- Program needs a thread split for producing chunks on the run instead of loading whole map beforehand.
4. Interface changes
  - Needs mesageboxes for several cases that cannot work together i.e. indexed triangles (or implement proper mesh creating algorithm for the case).
  

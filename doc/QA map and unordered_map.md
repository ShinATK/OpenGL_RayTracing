
Q：
对于数量少的多个变量，为了访问速度更快，应当使用 map 还是 unordered_map

ChatGPT：
**对于数量少的多个变量，通常建议使用 std::map**，而不是 std::unordered_map。

虽然 std::unordered_map 在大量数据时可以提供更快的查找速度，但是在数据量较小的情况下，std::map 通常具有更好的性能。

这是因为 std::map 是基于红黑树实现的，对于少量数据，红黑树的额外开销相对较小，并且在有序性要求较高时 std::map 的有序性可以提供更好的性能。


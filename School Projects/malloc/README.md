An implementation of malloc in C. First the user calls hl_init to initialize a heap of some size. Then calling hl_alloc will alloc space in
the heap and a pointer to the space will be returned. hl_release frees that space and hl_resize resizes that space. 

<p>Issues</p>
<ul>
<li>There is some bug causing this implementation to fail very, very large stress tests.
</ul>

# 简介
一个好的驱动应该同时实现阻塞io和非阻塞io。具体要不要阻塞，取决于用户空间的open函数的操作mask。
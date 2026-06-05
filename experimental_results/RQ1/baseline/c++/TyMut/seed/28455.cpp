Created attachment 53491 [details]
Reduced test case

Alright, managed to get a reduced test case out of C-Vise. Though, this doesn't build on clang (since the std:: namespace also got reduced; this can be recovered by replacing the first one with <atomic> and the second one with <coroutine> and fixing a deduction failure that happens on it)
---------------------------------------------------------


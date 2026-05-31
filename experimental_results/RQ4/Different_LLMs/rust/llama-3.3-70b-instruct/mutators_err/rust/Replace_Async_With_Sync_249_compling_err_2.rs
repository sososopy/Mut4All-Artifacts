impl Mutator for Replace_Async_With_Sync_249 {
    fn name(&self) -> &str {
        "Replace_Async_With_Sync_249"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    func.sig.asyncness = None;
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            func.sig.asyncness = None;
                        }
                    }
                }
            }
            if let syn::Item::Expr(expr) = item {
                if let syn::Expr::Async(ExprAsync { expr: inner_expr, .. }) = expr {
                    *item = syn::Item::Expr(inner_expr.clone());
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces async functions and closures with their synchronous counterparts, effectively removing the async keyword. This transformation aims to test the compiler's handling of async and sync code, potentially leading to bugs related to incorrect usage of await or async move, or issues with coroutine layout and promotion."
    }
}
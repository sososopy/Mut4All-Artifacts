use syn::{Expr, ExprAsync, File, Item, ItemFn, ItemImpl, ImplItem, ImplItemFn, Signature};
use crate::mutator::Mutator;

struct Replace_Async_With_Sync_249;

impl Mutator for Replace_Async_With_Sync_249 {
    fn name(&self) -> &str {
        "Replace_Async_With_Sync_249"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    func.sig.asyncness = None;
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Method(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            func.sig.asyncness = None;
                        }
                    }
                }
            }
            if let Item::Expr(expr) = item {
                if let Expr::Async(ExprAsync { body, .. }) = expr {
                    *item = Item::Expr(Expr::Block(body));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces async functions and closures with their synchronous counterparts, effectively removing the async keyword. This transformation aims to test the compiler's handling of async and sync code, potentially leading to bugs related to incorrect usage of await or async move, or issues with coroutine layout and promotion."
    }
}
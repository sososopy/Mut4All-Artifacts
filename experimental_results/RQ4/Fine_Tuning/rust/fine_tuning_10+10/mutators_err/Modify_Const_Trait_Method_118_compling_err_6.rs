use crate::mutator::Mutator;
use std::collections::HashSet;
use syn::{Item, Ident, Stmt, Expr};

pub struct Modify_Const_Trait_Method_118;

impl Mutator for Modify_Const_Trait_Method_118 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Method_118"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut non_const_methods = HashSet::new();

        // Identify const trait methods and create non-const versions
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let syn::TraitItem::Fn(method) = item {
                        if method.sig.constness.is_some() {
                            let non_const_method = syn::TraitItem::Fn(syn::TraitItemFn {
                                sig: syn::Signature {
                                    constness: None,
                                    ident: Ident::new(&format!("{}_non_const", method.sig.ident), method.sig.ident.span()),
                                    ..method.sig.clone()
                                },
                                ..method.clone()
                            });
                            non_const_methods.insert((method.sig.ident.clone(), non_const_method));
                        }
                    }
                }
            }
        }

        // Apply mutation to function implementations
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for method in &mut item_impl.items {
                    if let syn::ImplItem::Fn(impl_method) = method {
                        for stmt in &mut impl_method.block.stmts {
                            if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                                if let Expr::Path(expr_path) = &*expr_call.func {
                                    if let Some(segment) = expr_path.path.segments.last_mut() {
                                        if let Some((_, non_const_method)) = non_const_methods.iter().find(|(ident, _)| ident == &segment.ident) {
                                            segment.ident = non_const_method.sig.ident.clone();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies const methods in traits and creates non-const versions of these methods. It then scans function implementations and replaces calls to const methods with calls to their non-const counterparts. This highlights mismatches between const and non-const contexts, testing the Rust compiler's handling of const trait method calls in non-const contexts."
    }
}
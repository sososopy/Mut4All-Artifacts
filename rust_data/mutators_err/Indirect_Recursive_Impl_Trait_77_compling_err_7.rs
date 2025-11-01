use crate::mutator::Mutator;
use syn::{parse_quote, Item, ReturnType, Type, FnArg, Stmt, Expr, ExprCall, ExprPath, ItemFn};
use proc_macro2::{Ident, Span};

pub struct Indirect_Recursive_Impl_Trait_77;

impl Mutator for Indirect_Recursive_Impl_Trait_77 {
    fn name(&self) -> &str {
        "Indirect_Recursive_Impl_Trait_77"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut functions_to_modify = vec![];

        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = **return_type {
                        if item_fn.sig.inputs.iter().all(|arg| matches!(arg, FnArg::Typed(_))) {
                            if item_fn.block.stmts.iter().any(|stmt| {
                                if let Stmt::Expr(Expr::Call(ExprCall { func, .. }), _) = stmt {
                                    if let Expr::Path(ExprPath { path, .. }) = &**func {
                                        return path.is_ident(&item_fn.sig.ident);
                                    }
                                }
                                false
                            }) {
                                functions_to_modify.push(item_fn.clone());
                            }
                        }
                    }
                }
            }
        }

        for item_fn in functions_to_modify {
            let original_fn_name = &item_fn.sig.ident;
            let new_fn_name = Ident::new(&format!("indirect_{}", original_fn_name), Span::call_site());

            let new_fn = ItemFn {
                sig: syn::Signature {
                    ident: new_fn_name.clone(),
                    ..item_fn.sig.clone()
                },
                block: Box::new(parse_quote!({
                    #original_fn_name(#(#item_fn.sig.inputs.iter().map(|arg| {
                        if let FnArg::Typed(pat_type) = arg {
                            pat_type.pat.clone()
                        } else {
                            unreachable!()
                        }
                    })),*)
                })),
                ..item_fn.clone()
            };

            file.items.push(Item::Fn(new_fn));

            for item in &mut file.items {
                if let Item::Fn(fn_item) = item {
                    if fn_item.sig.ident == *original_fn_name {
                        for stmt in &mut fn_item.block.stmts {
                            if let Stmt::Expr(ExprCall { func, .. }, _) = stmt {
                                if let Expr::Path(ExprPath { path, .. }) = &mut **func {
                                    if path.is_ident(original_fn_name) {
                                        *path = new_fn_name.clone().into();
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
        "The mutation operator targets functions with recursive calls and `impl Trait` return types. It introduces an additional level of indirection by creating a new function that calls the original function recursively. This new function has the same parameters and return type, facilitating indirect recursive calls and potentially exposing compiler weaknesses in handling such patterns."
    }
}
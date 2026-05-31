use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, path::Path, process::Command};
use syn::{
    Expr, ExprCall, ExprPath, File, Item, ItemFn, ItemImpl, ItemTrait, TraitItem, ImplItem, Ident,
    parse_quote, Stmt, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Trait_Method_308;

impl Mutator for Modify_Const_Trait_Method_308 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Method_308"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_methods: HashSet<String> = HashSet::new();

        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &item_trait.items {
                    if let TraitItem::Fn(method) = trait_item {
                        if method.sig.constness.is_some() {
                            let method_name = method.sig.ident.to_string();
                            trait_methods.insert(method_name.clone());

                            // Add a non-const version of the method
                            let non_const_method: TraitItem = parse_quote! {
                                fn #method_name(&self) -> i32;
                            };
                            let mut new_items = item_trait.items.clone();
                            new_items.push(non_const_method);
                            item_trait.items = new_items;
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if trait_methods.contains(&method.sig.ident.to_string()) {
                            method.sig.constness = None;
                            method.sig.ident = Ident::new(
                                &format!("{}_non_const", method.sig.ident),
                                method.sig.ident.span(),
                            );
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for stmt in &mut method.block.stmts {
                            if let Stmt::Expr(Expr::Call(call_expr), _) = stmt {
                                if let Expr::Path(ExprPath { path, .. }) = &mut *call_expr.func {
                                    if let Some(last_segment) = path.segments.last_mut() {
                                        let method_name = last_segment.ident.to_string();
                                        if trait_methods.contains(&method_name) {
                                            last_segment.ident = Ident::new(
                                                &format!("{}_non_const", method_name),
                                                last_segment.ident.span(),
                                            );
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
        "The mutation operator identifies const trait methods and replaces their calls with non-const versions, introducing a mismatch in contexts that require const correctness. This transformation is intended to test the compiler's handling of const trait methods and expose potential inconsistencies in const trait logic."
    }
}
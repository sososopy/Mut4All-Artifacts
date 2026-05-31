use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, Item, ItemImpl, ImplItem, Stmt, parse_quote,
    visit_mut::VisitMut, token::Semi,
};

use crate::mutator::Mutator;

pub struct ModifyTraitImplWithRecursiveAsync305;

impl Mutator for ModifyTraitImplWithRecursiveAsync305 {
    fn name(&self) -> &str {
        "ModifyTraitImplWithRecursiveAsync305"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut has_async_trait_fn = false;

                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if func.sig.asyncness.is_some() {
                            has_async_trait_fn = true;
                            break;
                        }
                    }
                }

                if has_async_trait_fn {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(func) = impl_item {
                            if func.sig.asyncness.is_some() {
                                let recursive_call: Expr = parse_quote! {
                                    self.#func.sig.ident().await
                                };
                                let new_stmt = Stmt::Expr(recursive_call, None);
                                func.block.stmts.push(new_stmt);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies trait implementations with async functions and introduces recursive async calls using existing methods. This transformation tests the compiler's handling of recursion and async method chaining within trait implementations, potentially exposing issues with normalization or recursion in async contexts."
    }
}
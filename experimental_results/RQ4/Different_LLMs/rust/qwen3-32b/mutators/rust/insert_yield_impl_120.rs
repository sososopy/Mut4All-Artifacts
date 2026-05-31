use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};
use syn::token::Async;

use crate::mutator::Mutator;

pub struct Insert_Yield_Impl_120;

impl Mutator for Insert_Yield_Impl_120 {
    fn name(&self) -> &str {
        "Insert_Yield_Impl_120"
    }
    fn mutate(&self, file: &mut syn::File) {
        file.items.retain(|item| {
            if let syn::Item::Fn(func) = item {
                func.sig.ident != "main"
            } else {
                true
            }
        });

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        if !type_impl_trait.bounds.is_empty() {
                            type_impl_trait.bounds.clear();
                            func.sig.asyncness = Some(Async::default());
                            let yield_expr: Expr = parse_quote! { yield };
                            let mut new_block = func.block.clone();
                            new_block.stmts.clear();
                            new_block.stmts.push(Stmt::Expr(yield_expr, None));
                            func.block = new_block;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait` and transforms them into async functions with a `yield` statement in their body, while omitting the trait bounds in the return type. This exploits the compiler's handling of yield expressions in async contexts and the absence of trait bounds, potentially triggering ICEs during borrow-checking due to type assertion mismatches."
    }
}
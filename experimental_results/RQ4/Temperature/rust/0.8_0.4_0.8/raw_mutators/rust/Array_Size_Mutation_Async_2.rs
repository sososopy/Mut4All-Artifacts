use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

use crate::mutator::Mutator;

pub struct Array_Size_Mutation_Async_2;

impl Mutator for Array_Size_Mutation_Async_2 {
    fn name(&self) -> &str {
        "Array_Size_Mutation_Async_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Await(expr_await)) = stmt {
                            if let Expr::Array(expr_array) = &*expr_await.base {
                                if let Expr::Lit(expr_lit) = &expr_array.elems[1] {
                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                        let sizes = [1, 10, 1000, 0xffffffff];
                                        let mut rng = thread_rng();
                                        let new_size = sizes.choose(&mut rng).unwrap();
                                        let new_size_expr: Expr = parse_quote! { #new_size };
                                        expr_array.elems[1] = Expr::Lit(ExprLit {
                                            attrs: vec![],
                                            lit: syn::Lit::Int(syn::LitInt::new(
                                                &new_size.to_string(),
                                                lit_int.span(),
                                            )),
                                        });
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
        "The mutation operator targets array size declarations within async functions where the array is awaited. It randomly changes the array size to different values (small, medium, and large) to test the Rust compiler's handling of various array sizes in async contexts, potentially revealing issues with memory management or overflow handling."
    }
}
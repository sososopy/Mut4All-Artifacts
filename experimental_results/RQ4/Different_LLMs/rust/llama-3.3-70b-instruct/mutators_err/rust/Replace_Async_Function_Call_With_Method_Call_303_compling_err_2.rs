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

pub struct Replace_Async_Function_Call_With_Method_Call_303;

impl Mutator for Replace_Async_Function_Call_With_Method_Call_303 {
    fn name(&self) -> &str {
        "Replace_Async_Function_Call_With_Method_Call_303"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for method in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = method {
                        for statement in &mut method.block.stmts {
                            if let Stmt::Expr(expr, _) = statement {
                                if let Expr::Call(ExprCall { func, args, .. }) = expr {
                                    if let Expr::Path(ExprPath { path, .. }) = func.as_ref() {
                                        if path.segments.last().unwrap().ident.to_string().starts_with("async_") {
                                            let method_name = path.segments.last().unwrap().ident.clone();
                                            let method_name_str = method_name.to_string();
                                            let new_method_name = method_name_str.trim_start_matches("async_");
                                            let new_method_ident = Ident::new(&new_method_name, method_name.span());
                                            let new_expr = parse_quote! {
                                                self.#new_method_ident(#args)
                                            };
                                            *expr = Box::new(new_expr);
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
        "The mutation operator targets async function calls within traits and replaces them with method calls. It aims to test the compiler's handling of async functions in traits by introducing method calls that may alter the execution flow or type checking."
    }
}
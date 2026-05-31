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

pub struct Modify_Macro_Invocation_203;

impl Mutator for Modify_Macro_Invocation_203 {
    fn name(&self) -> &str {
        "Modify_Macro_Invocation_203"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut rng = thread_rng();
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Macro(mac) = stmt {
                        if let Some(last_segment) = mac.mac.path.segments.last() {
                            if last_segment.ident == "example_macro" {
                                let mut tokens = mac.mac.tokens.clone().into_iter().collect::<Vec<_>>();
                                if !tokens.is_empty() {
                                    let stmt_index = rng.gen_range(0..tokens.len());
                                    let new_stmt: Expr = if rng.gen_bool(0.5) {
                                        parse_quote! { return; }
                                    } else {
                                        parse_quote! { some_function(); }
                                    };
                                    tokens[stmt_index] = TokenTree::Group(proc_macro2::Group::new(proc_macro2::Delimiter::None, new_stmt.to_token_stream()));
                                    mac.mac.tokens = tokens.into_iter().collect();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations within function bodies, specifically those that take statements as parameters. It replaces one of the macro's statement parameters with a new statement that modifies control flow or introduces a function call, such as `return;` or `some_function();`. This transformation aims to test the compiler's handling of macro expansions and control flow changes."
    }
}
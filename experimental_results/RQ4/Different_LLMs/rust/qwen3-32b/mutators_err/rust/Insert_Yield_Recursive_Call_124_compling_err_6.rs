use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    *,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus, Semi},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Insert_Yield_Recursive_Call_124;

impl Mutator for Insert_Yield_Recursive_Call_124 {
    fn name(&self) -> &str {
        "Insert_Yield_Recursive_Call_124"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(header) = &item_fn.sig.header {
                    if header.gen.is_some() {
                        if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                let has_iterator = type_impl_trait.bounds.iter().any(|bound| {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        trait_bound.path.is_ident("Iterator")
                                    } else {
                                        false
                                    }
                                });
                                if has_iterator {
                                    let func_ident = &item_fn.sig.ident;
                                    let yield_stmt: syn::Stmt = parse_quote! { yield 42; };
                                    let call_expr: syn::Expr = parse_quote! {
                                        #func_ident().collect::<Vec<_>>()
                                    };
                                    let mut new_block = Box::new(syn::Block {
                                        stmts: vec![
                                            yield_stmt,
                                            syn::Stmt::Expr(call_expr, Some(token::Semi::default())),
                                        ],
                                        ..item_fn.block.as_ref().clone()
                                    });
                                    item_fn.block = new_block;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
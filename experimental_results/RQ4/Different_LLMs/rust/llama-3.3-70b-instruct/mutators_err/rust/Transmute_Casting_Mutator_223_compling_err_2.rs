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

pub struct Transmute_Casting_Mutator_223;

impl Mutator for Transmute_Casting_Mutator_223 {
    fn name(&self) -> &str {
        "Transmute_Casting_Mutator_223"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call) = expr {
                            if let Expr::Path(path) = &*call.func {
                                if path.path.segments.last().unwrap().ident == "transmute" {
                                    let new_type = match call.args.first().unwrap() {
                                        Expr::Path(path) => {
                                            if path.path.segments.last().unwrap().ident == "u64" {
                                                parse_quote!(i32)
                                            } else if path.path.segments.last().unwrap().ident == "i32" {
                                                parse_quote!(u64)
                                            } else {
                                                parse_quote!(i32)
                                            }
                                        }
                                        _ => parse_quote!(i32),
                                    };
                                    *stmt = parse_quote! {
                                        {
                                            let value = #call;
                                            #new_type::from(value)
                                        }
                                    };
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The Transmute_Casting_Mutator_223 mutation operator targets the use of transmute casting in the given Rust code. It replaces existing transmute casts with new casts that may trigger bugs in the compiler, such as casting between a reference type and a primitive type, or casting between types of different sizes."
    }
}
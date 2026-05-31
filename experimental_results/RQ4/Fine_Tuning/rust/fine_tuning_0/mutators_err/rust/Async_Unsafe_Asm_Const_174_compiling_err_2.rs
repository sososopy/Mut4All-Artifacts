use proc_macro2::{Span, TokenStream};
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

pub struct Async_Unsafe_Asm_Const_174;

impl Mutator for Async_Unsafe_Asm_Const_174 {
    fn name(&self) -> &str {
        "Async_Unsafe_Asm_Const_174"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() && func.sig.unsafety.is_some() {
                    if let Some(asm_stmt) = func.block.stmts.iter_mut().find_map(|stmt| {
                        if let Stmt::Expr(Expr::Macro(mac)) = stmt {
                            if mac.mac.path.is_ident("asm") {
                                return Some(mac);
                            }
                        }
                        None
                    }) {
                        // Add a const parameter to the function signature
                        func.sig.generics.params.push(parse_quote!(const M: usize));

                        // Modify the inline assembly statement to use the new const parameter
                        if let Some(tt) = asm_stmt.mac.tokens.to_string().split(',').nth(1) {
                            let new_tokens = format!("const M{}", tt);
                            asm_stmt.mac.tokens = new_tokens.parse().unwrap();
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
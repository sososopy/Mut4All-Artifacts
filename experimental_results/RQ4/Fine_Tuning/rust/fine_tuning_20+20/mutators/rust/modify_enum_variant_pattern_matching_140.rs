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

pub struct Modify_Enum_Variant_Pattern_Matching_140;

impl Mutator for Modify_Enum_Variant_Pattern_Matching_140 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_Pattern_Matching_140"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_stmts = Vec::new();
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::TupleStruct(pat_tuple_struct) = &local.pat {
                            if pat_tuple_struct.elems.len() == 1
                                && matches!(pat_tuple_struct.elems[0], Pat::Wild(_))
                            {
                                let path = &pat_tuple_struct.path;
                                let new_local: Stmt = parse_quote! {
                                    let #path = #path(10)
                                };
                                new_stmts.push(new_local);
                                continue;
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                item_fn.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets `let` statements with enum variant pattern matching using a wildcard, such as `let E::V(..) =`. It replaces the pattern with a direct assignment from a constructed enum variant, e.g., `let E::V = E::V(10)`. This transformation alters the pattern matching semantics and forces the program to use concrete enum variant construction, which can reveal subtle bugs in the compiler's handling of enums and pattern matching."
    }
}
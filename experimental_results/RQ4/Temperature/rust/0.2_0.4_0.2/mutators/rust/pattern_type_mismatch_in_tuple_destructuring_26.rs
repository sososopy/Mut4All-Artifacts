use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use rand::prelude::IteratorRandom;
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Pattern_Type_Mismatch_In_Tuple_Destructuring_26;

impl Mutator for Pattern_Type_Mismatch_In_Tuple_Destructuring_26 {
    fn name(&self) -> &str {
        "Pattern_Type_Mismatch_In_Tuple_Destructuring_26"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut rng = thread_rng();
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(Local { pat, init: Some(LocalInit { expr, .. }), .. }) = stmt {
                        if let (Pat::Tuple(pat_tuple), Expr::Tuple(expr_tuple)) = (pat, &**expr) {
                            if pat_tuple.elems.len() == expr_tuple.elems.len() {
                                let mut mutated_expr = expr_tuple.clone();
                                if let Some(mut elem) = mutated_expr.elems.iter_mut().choose(&mut rng) {
                                    *elem = parse_quote! { "string" };
                                }
                                *expr = Box::new(Expr::Tuple(mutated_expr));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets tuple destructuring assignments within function bodies. It randomly changes one element in the tuple being assigned to an incompatible type, such as a string, to create a type mismatch. This transformation aims to expose bugs related to pattern type mismatches during the compiler's pattern analysis phase."
    }
}
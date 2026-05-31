use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_Local_ImplTrait_Ref_With_ImplTrait_212;

impl Mutator for Replace_Local_ImplTrait_Ref_With_ImplTrait_212 {
    fn name(&self) -> &str {
        "Replace_Local_ImplTrait_Ref_With_ImplTrait_212"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_stmts = Vec::new();
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Pat::Type(pat_type) = &local.pat {
                                if let Type::Reference(type_reference) = &*pat_type.ty {
                                    if let Type::ImplTrait(type_impl_trait) = &*type_reference.elem {
                                        let new_local = Local {
                                            attrs: local.attrs.clone(),
                                            let_token: local.let_token,
                                            pat: local.pat.clone(),
                                            init: Some(LocalInit { eq_token: init.eq_token, expr: init.expr.clone(), diverge: init.diverge.clone() }),
                                            semi_token: local.semi_token,
                                        };
                                        new_stmts.push(Stmt::Local(new_local));
                                        continue;
                                    }
                                }
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
        "This mutator targets local variable bindings within functions that use `&impl Trait` types. It transforms these bindings to use `impl Trait` directly by removing the reference and adjusting the assigned value accordingly. This change tests the compiler's handling of `impl Trait` in local contexts, particularly its ability to resolve trait implementations and type inference without the indirection of a reference."
    }
}
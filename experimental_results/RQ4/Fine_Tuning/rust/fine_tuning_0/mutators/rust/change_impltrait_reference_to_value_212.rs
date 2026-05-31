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

pub struct Change_ImplTrait_Reference_To_Value_212;

impl Mutator for Change_ImplTrait_Reference_To_Value_212 {
    fn name(&self) -> &str {
        "Change_ImplTrait_Reference_To_Value_212"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Pat::Type(PatType { ty, pat, colon_token, .. }) = &local.pat {
                                if let Type::ImplTrait(type_impl_trait) = ty.as_ref() {
                                    if let Expr::Reference(expr_ref) = &*init.expr {
                                        local.pat = Pat::Type(PatType {
                                            ty: Box::new(Type::ImplTrait(type_impl_trait.clone())),
                                            pat: pat.clone(),
                                            colon_token: *colon_token,
                                            attrs: Vec::new(),
                                        });
                                        local.init = Some(syn::LocalInit {
                                            eq_token: init.eq_token,
                                            expr: expr_ref.expr.clone(),
                                            diverge: init.diverge.clone(),
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
        "The mutation operator targets local variable bindings using `impl Trait` with reference types within function bodies. It changes the type from `&impl Trait` to `impl Trait`, ensuring the assignment is compatible with the new type. This transformation tests the compiler's handling of direct `impl Trait` usage without references, potentially exposing issues in type inference or trait implementation."
    }
}
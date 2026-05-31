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

pub struct Modify_Closure_Lifetime_429;

impl Mutator for Modify_Closure_Lifetime_429 {
    fn name(&self) -> &str {
        "Modify_Closure_Lifetime_429"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &mut local.init {
                            if let Expr::Closure(closure) = &mut **expr {
                                if let Some(ref mut lifetimes) = closure.lifetimes {
                                    lifetimes.lifetimes.clear();
                                }
                                for input in &mut closure.inputs {
                                    if let FnArg::Typed(pat_type) = input {
                                        if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                            type_ref.lifetime = None;
                                        }
                                    }
                                }
                                if let ReturnType::Type(_, ref mut ty) = closure.output {
                                    if let Type::Reference(type_ref) = &mut **ty {
                                        type_ref.lifetime = None;
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
        ""
    }
}
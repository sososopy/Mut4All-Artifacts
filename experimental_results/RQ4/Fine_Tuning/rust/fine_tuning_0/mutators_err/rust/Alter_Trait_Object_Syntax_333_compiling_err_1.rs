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

pub struct Alter_Trait_Object_Syntax_333;

impl Mutator for Alter_Trait_Object_Syntax_333 {
    fn name(&self) -> &str {
        "Alter_Trait_Object_Syntax_333"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let syn::Stmt::Local(local) = stmt {
                        if let Some((_, ty)) = &mut local.ty {
                            if let syn::Type::TraitObject(trait_object) = &mut **ty {
                                if trait_object.dyn_token.is_some() {
                                    trait_object.dyn_token = None;
                                } else {
                                    trait_object.dyn_token = Some(token::Dyn::default());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait object syntax within function scopes, toggling between `dyn Trait` and `dyn* Trait`. This transformation checks the compiler's handling of trait object syntax variations and ensures type conversions or castings are correctly adjusted."
    }
}
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

pub struct Modify_Const_Generic_Default_To_Self_280;

impl Mutator for Modify_Const_Generic_Default_To_Self_280 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Default_To_Self_280"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                for param in &mut trait_def.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(default_expr) = &mut const_param.default {
                            let param_name = &const_param.ident;
                            let new_expr: syn::Expr = parse_quote! { #param_name };
                            *default_expr = new_expr;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters in trait definitions that have default values. It replaces the default expression with the parameter's own identifier, creating a self-referential default. This transformation aims to expose bugs in the compiler's handling of const generic parameter defaults during HIR well-formedness checking, particularly around anonymous constants and their resolution. The mutation uses only elements already present in the seed program, preserving all other aspects of the trait definition."
    }
}
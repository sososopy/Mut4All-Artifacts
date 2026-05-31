use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, ItemTrait, ItemImpl, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Lifetime_Parameter_With_Static_192;

impl Mutator for Replace_Lifetime_Parameter_With_Static_192 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameter_With_Static_192"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        if lifetime_param.lifetime.ident != "static" {
                            lifetime_param.lifetime = Lifetime::new("'static", lifetime_param.lifetime.span());
                        }
                    }
                }
            }
            if let Item::Enum(item_enum) = item {
                for param in &mut item_enum.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        if lifetime_param.lifetime.ident != "static" {
                            lifetime_param.lifetime = Lifetime::new("'static", lifetime_param.lifetime.span());
                        }
                    }
                }
            }
            if let Item::Trait(item_trait) = item {
                for param in &mut item_trait.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        if lifetime_param.lifetime.ident != "static" {
                            lifetime_param.lifetime = Lifetime::new("'static", lifetime_param.lifetime.span());
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for param in &mut item_impl.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        if lifetime_param.lifetime.ident != "static" {
                            lifetime_param.lifetime = Lifetime::new("'static", lifetime_param.lifetime.span());
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic lifetime parameters in struct, enum, trait, and impl definitions with the reserved lifetime name 'static. This creates a conflict with Rust's lifetime naming rules, potentially causing inconsistencies in lifetime resolution and region error reporting. The mutation is applied only at declaration sites, leaving usage sites unchanged, which may lead to undeclared lifetime errors and expose compiler crashes in lifetime checking."
    }
}
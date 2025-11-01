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

pub struct Modify_Trait_And_Function_Bound_Syntax_409;

impl Mutator for Modify_Trait_And_Function_Bound_Syntax_409 {
    fn name(&self) -> &str {
        "Modify_Trait_And_Function_Bound_Syntax_409"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.generics.params.is_empty() {
                    let mut new_generics = item_fn.sig.generics.clone();
                    for param in &mut new_generics.params {
                        if let GenericParam::Type(type_param) = param {
                            if type_param.bounds.is_empty() {
                                type_param.bounds.push(parse_quote!('static));
                            }
                        }
                    }
                    item_fn.sig.generics = new_generics;

                    if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                        if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                            type_impl_trait.bounds.push(parse_quote!('static));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator focuses on modifying the trait and function bound syntax by introducing redundant lifetimes and bounds. It identifies functions with generic bounds and trait implementations, then adds unnecessary complexity by appending 'static lifetime to both generic parameters and return types. This mutation can potentially confuse the Rust compiler in resolving lifetimes and bindings."
    }
}
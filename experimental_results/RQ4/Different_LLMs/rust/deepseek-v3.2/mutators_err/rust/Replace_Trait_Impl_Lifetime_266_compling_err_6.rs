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

pub struct Replace_Trait_Impl_Lifetime_266;

impl Mutator for Replace_Trait_Impl_Lifetime_266 {
    fn name(&self) -> &str {
        "Replace_Trait_Impl_Lifetime_266"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.generics.params.len() > 0 {
                    let declared_lifetimes: Vec<String> = item_impl.generics.params.iter()
                        .filter_map(|param| {
                            if let GenericParam::Lifetime(lifetime_param) = param {
                                Some(lifetime_param.lifetime.ident.to_string())
                            } else {
                                None
                            }
                        })
                        .collect();
                    if declared_lifetimes.len() == 0 {
                        continue;
                    }
                    let target_lifetime = declared_lifetimes[0].clone();
                    let undeclared_lifetime_name = format!("{}_undeclared", target_lifetime);
                    let undeclared_lifetime = Lifetime::new(&undeclared_lifetime_name, Span::call_site());
                    if let Type::Path(type_path) = &mut *item_impl.self_ty {
                        for segment in &mut type_path.path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Lifetime(lifetime) = arg {
                                        if lifetime.ident.to_string() == target_lifetime {
                                            *lifetime = undeclared_lifetime.clone();
                                        }
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
        "The mutation operator targets trait implementations with lifetime parameters. It identifies impl blocks that declare at least one lifetime parameter and then modifies the type arguments in the self type, replacing a declared lifetime with an undeclared one (by appending '_undeclared' to an existing lifetime name). This creates a mismatch between declared and used lifetimes, potentially triggering region resolution errors during normalization and exposing internal compiler errors in the trait system's well-formedness checking."
    }
}
use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Opaque_Trait_Return_Mismatch_160;

impl Mutator for Opaque_Trait_Return_Mismatch_160 {
    fn name(&self) -> &str {
        "Opaque_Trait_Return_Mismatch_160"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        for bound in &mut type_impl_trait.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Fn") {
                                    if let Some(segment) = trait_bound.path.segments.get_mut(0) {
                                        if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                            for arg in &mut args.args {
                                                if let syn::GenericArgument::Constraint(constraint) = arg {
                                                    if constraint.ident == "Output" {
                                                        constraint.bounds = Punctuated::new();
                                                        let ty = syn::parse_quote!(i32);
                                                        if let syn::Type::Path(type_path) = ty {
                                                            let path = SynPath {
                                                                leading_colon: None,
                                                                segments: type_path.path.segments.clone(),
                                                            };
                                                            let new_bound = TraitBound {
                                                                modifier: TraitBoundModifier::None,
                                                                lifetimes: Some(BoundLifetimes::default()),
                                                                path,
                                                                paren_token: None,
                                                            };
                                                            constraint.bounds.push(TypeParamBound::Trait(new_bound));
                                                        } else {
                                                            panic!("Unexpected type for i32");
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
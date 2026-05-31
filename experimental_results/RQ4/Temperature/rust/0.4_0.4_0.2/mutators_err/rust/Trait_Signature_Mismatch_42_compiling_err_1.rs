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

pub struct Trait_Signature_Mismatch_42;

impl Mutator for Trait_Signature_Mismatch_42 {
    fn name(&self) -> &str {
        "Trait_Signature_Mismatch_42"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            let mut new_bounds = type_impl_trait.bounds.clone();
                            new_bounds.push(TypeParamBound::Lifetime(Lifetime::new("'a", Span::call_site())));
                            new_bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segments = Punctuated::new();
                                        segments.push(PathSegment {
                                            ident: Ident::new("AnotherTraitType", Span::call_site()),
                                            arguments: PathArguments::None,
                                        });
                                        segments
                                    },
                                },
                            }));
                            *pat_type.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: type_impl_trait.impl_token,
                                bounds: new_bounds,
                            }));
                        }
                    }
                }
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**ty {
                        let mut new_bounds = type_impl_trait.bounds.clone();
                        new_bounds.push(TypeParamBound::Lifetime(Lifetime::new("'static", Span::call_site())));
                        *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds: new_bounds,
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
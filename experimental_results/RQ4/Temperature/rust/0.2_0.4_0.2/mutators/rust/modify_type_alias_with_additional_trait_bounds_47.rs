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

pub struct Modify_Type_Alias_With_Additional_Trait_Bounds_47;

impl Mutator for Modify_Type_Alias_With_Additional_Trait_Bounds_47 {
    fn name(&self) -> &str {
        "Modify_Type_Alias_With_Additional_Trait_Bounds_47"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(type_item) = item {
                if let Type::ImplTrait(type_impl_trait) = &*type_item.ty {
                    let mut new_bounds = type_impl_trait.bounds.clone();
                    new_bounds.push(TypeParamBound::Trait(TraitBound {
                        paren_token: None,
                        modifier: TraitBoundModifier::None,
                        lifetimes: None,
                        path: syn::Path {
                            leading_colon: None,
                            segments: {
                                let mut segs = Punctuated::new();
                                segs.push(syn::PathSegment {
                                    ident: Ident::new("Clone", Span::call_site()),
                                    arguments: syn::PathArguments::None,
                                });
                                segs
                            },
                        },
                    }));
                    type_item.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                        impl_token: type_impl_trait.impl_token,
                        bounds: new_bounds,
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
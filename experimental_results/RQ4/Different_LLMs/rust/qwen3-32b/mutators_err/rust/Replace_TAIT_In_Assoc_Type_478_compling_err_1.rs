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

pub struct Replace_TAIT_In_Assoc_Type_478;

impl Mutator for Replace_TAIT_In_Assoc_Type_478 {
    fn name(&self) -> &str {
        "Replace_TAIT_In_Assoc_Type_478"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect TAITs (type T = impl Trait;)
        let mut tait_types = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let syn::Type::ImplTrait(type_impl_trait) = &*item_type.ty {
                    tait_types.push((item_type.ident.clone(), type_impl_trait.bounds.clone()));
                }
            }
        }

        // If there are TAITs, modify associated types in impls
        if !tait_types.is_empty() {
            let (tait_ident, _) = &tait_types[0];
            for item in &mut file.items {
                if let syn::Item::Impl(impl_item) = item {
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Type(assoc_type) = impl_item {
                            *assoc_type.ty = Box::new(syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = syn::punctuated::Punctuated::new();
                                        segs.push(syn::PathSegment {
                                            ident: tait_ident.clone(),
                                            arguments: syn::PathArguments::None,
                                        });
                                        segs
                                    },
                                },
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces associated types in trait implementations with opaque type aliases (TAIT). This transformation forces the compiler to resolve TAITs as trait-associated types, potentially exposing bugs in type inference, trait resolution, or monomorphization logic when opaque types are used in complex trait hierarchies."
    }
}
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

pub struct Modify_Inherent_Associated_Type_420;

impl Mutator for Modify_Inherent_Associated_Type_420 {
    fn name(&self) -> &str {
        "Modify_Inherent_Associated_Type_420"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut has_lifetime = false;
                if let Some((_, generics, _)) = &item_impl.trait_ {
                    for param in &generics.params {
                        if let syn::GenericParam::Lifetime(_) = param {
                            has_lifetime = true;
                            break;
                        }
                    }
                }
                if has_lifetime {
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Type(impl_type) = impl_item {
                            if let syn::Type::Reference(type_ref) = &mut *impl_type.ty {
                                if let Some(lifetime) = &mut type_ref.lifetime {
                                    lifetime.ident = syn::Ident::new("b", Span::call_site());
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
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

pub struct Transform_Return_Type_To_HigherRanked_ImplTrait_With_AssocType_196;

impl Mutator for Transform_Return_Type_To_HigherRanked_ImplTrait_With_AssocType_196 {
    fn name(&self) -> &str {
        "Transform_Return_Type_To_HigherRanked_ImplTrait_With_AssocType_196"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(_) = **ty {
                        *ty = Box::new(parse_quote! {
                            impl for<'a> Trait<'a, Assoc = impl Sized>
                        });
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let Type::Path(_) = **ty {
                                *ty = Box::new(parse_quote! {
                                    impl for<'a> Trait<'a, Assoc = impl Sized>
                                });
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
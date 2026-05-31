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

pub struct Replace_Concrete_Type_With_Associated_Type_297;

impl Mutator for Replace_Concrete_Type_With_Associated_Type_297 {
    fn name(&self) -> &str {
        "Replace_Concrete_Type_With_Associated_Type_297"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(_) = &*pat_type.ty {
                                *pat_type.ty = parse_quote!(<Self as UnknownTrait>::AssocType);
                            }
                        }
                    }
                    if let ReturnType::Type(_, ty) = &mut func.sig.output {
                        if let Type::Path(_) = &*ty {
                            *ty = parse_quote!(<Self as UnknownTrait>::AssocType);
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.constness.is_some() {
                            for input in &mut func.sig.inputs {
                                if let FnArg::Typed(pat_type) = input {
                                    if let Type::Path(_) = &*pat_type.ty {
                                        *pat_type.ty = parse_quote!(<Self as UnknownTrait>::AssocType);
                                    }
                                }
                            }
                            if let ReturnType::Type(_, ty) = &mut func.sig.output {
                                if let Type::Path(_) = &*ty {
                                    *ty = parse_quote!(<Self as UnknownTrait>::AssocType);
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
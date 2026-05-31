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

pub struct Create_Cyclic_Generic_Params_342;

impl Mutator for Create_Cyclic_Generic_Params_342 {
    fn name(&self) -> &str {
        "Create_Cyclic_Generic_Params_342"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                let has_type = generics.params.iter().any(|p| matches!(p, GenericParam::Type(_)));
                let has_const = generics.params.iter().any(|p| matches!(p, GenericParam::Const(_)));
                if has_type && has_const {
                    let mut type_idx = None;
                    let mut const_idx = None;
                    for (i, param) in generics.params.iter().enumerate() {
                        match param {
                            GenericParam::Type(_) => {
                                if type_idx.is_none() {
                                    type_idx = Some(i);
                                }
                            }
                            GenericParam::Const(_) => {
                                if const_idx.is_none() {
                                    const_idx = Some(i);
                                }
                            }
                            _ => {}
                        }
                    }
                    if let (Some(t), Some(c)) = (type_idx, const_idx) {
                        if let GenericParam::Type(ty) = &mut generics.params[t] {
                            ty.ident = Ident::new("T", Span::call_site());
                            ty.default = Some(parse_quote!(T));
                        }
                        if let GenericParam::Const(ct) = &mut generics.params[c] {
                            ct.ident = Ident::new("T", Span::call_site());
                            ct.ty = Some(Box::new(parse_quote!(T)));
                        }
                    }
                }
            } else if let syn::Item::Fn(item_fn) = item {
                let generics = &mut item_fn.sig.generics;
                let has_type = generics.params.iter().any(|p| matches!(p, GenericParam::Type(_)));
                let has_const = generics.params.iter().any(|p| matches!(p, GenericParam::Const(_)));
                if has_type && has_const {
                    let mut type_idx = None;
                    let mut const_idx = None;
                    for (i, param) in generics.params.iter().enumerate() {
                        match param {
                            GenericParam::Type(_) => {
                                if type_idx.is_none() {
                                    type_idx = Some(i);
                                }
                            }
                            GenericParam::Const(_) => {
                                if const_idx.is_none() {
                                    const_idx = Some(i);
                                }
                            }
                            _ => {}
                        }
                    }
                    if let (Some(t), Some(c)) = (type_idx, const_idx) {
                        if let GenericParam::Type(ty) = &mut generics.params[t] {
                            ty.ident = Ident::new("T", Span::call_site());
                            ty.default = Some(parse_quote!(T));
                        }
                        if let GenericParam::Const(ct) = &mut generics.params[c] {
                            ct.ident = Ident::new("T", Span::call_site());
                            ct.ty = Some(Box::new(parse_quote!(T)));
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
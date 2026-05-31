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

pub struct Duplicate_Const_Generic_Parameter_Name_82;

impl Mutator for Duplicate_Const_Generic_Parameter_Name_82 {
    fn name(&self) -> &str {
        "Duplicate_Const_Generic_Parameter_Name_82"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            let generics = match item {
                Item::Struct(s) => &mut s.generics,
                Item::Enum(e) => &mut e.generics,
                Item::Union(u) => &mut u.generics,
                Item::Fn(f) => &mut f.sig.generics,
                Item::Trait(t) => &mut t.generics,
                Item::Impl(i) => &mut i.generics,
                _ => continue,
            };
            if generics.params.is_empty() {
                continue;
            }
            let const_params = generics.params.iter().enumerate().filter_map(|(idx, param)| {
                if let GenericParam::Const(c) = param {
                    Some((idx, c.ident.clone()))
                } else {
                    None
                }
            }).collect::<Vec<_>>();
            if const_params.is_empty() {
                continue;
            }
            let (target_idx, target_ident) = const_params.choose(&mut rng).unwrap();
            let candidate_names = generics.params.iter().filter_map(|param| match param {
                GenericParam::Type(t) => Some(t.ident.clone()),
                GenericParam::Lifetime(l) => Some(l.lifetime.ident.clone()),
                GenericParam::Const(c) => Some(c.ident.clone()),
            }).collect::<Vec<_>>();
            let containing_name = match item {
                Item::Struct(s) => s.ident.clone(),
                Item::Enum(e) => e.ident.clone(),
                Item::Union(u) => u.ident.clone(),
                Item::Fn(f) => f.sig.ident.clone(),
                Item::Trait(t) => t.ident.clone(),
                Item::Impl(i) => i.trait_.as_ref().map(|(_, path, _)| path.segments.last().unwrap().ident.clone()).unwrap_or_else(|| {
                    if let Some(ty) = &i.self_ty {
                        if let Type::Path(tp) = ty {
                            tp.path.segments.last().unwrap().ident.clone()
                        } else {
                            Ident::new("Impl", Span::call_site())
                        }
                    } else {
                        Ident::new("Impl", Span::call_site())
                    }
                }),
                _ => continue,
            };
            candidate_names.push(containing_name);
            let new_name = candidate_names.choose(&mut rng).unwrap();
            if let GenericParam::Const(c) = &mut generics.params[*target_idx] {
                c.ident = new_name.clone();
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator selects a const generic parameter from struct, enum, union, function, trait, or impl definitions and changes its identifier to match either the containing item's name or another generic parameter already present in the same list. This creates duplicate generic parameter names, potentially causing shadowing, cycles, or out-of-range substitutions. It aims to stress the compiler's handling of generic parameter name resolution and const generic evaluation under ambiguous naming scenarios."
    }
}
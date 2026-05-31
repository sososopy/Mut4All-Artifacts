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

pub struct Add_Const_Generic_Assoc_Bound_133;

impl Mutator for Add_Const_Generic_Assoc_Bound_133 {
    fn name(&self) -> &str {
        "Add_Const_Generic_Assoc_Bound_133"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let new_assoc = TraitItem::Type(syn::TraitItemType {
                    attrs: Vec::new(),
                    ident: Ident::new("NewAssoc", Span::call_site()),
                    colon_token: Some(Default::default()),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        let x_bound = parse_quote! { X<Y<5i32> = ()> };
                        bounds.push(syn::TypeParamBound::Trait(x_bound));
                        bounds
                    },
                    generics: Default::default(),
                    default: None,
                    semi_token: Default::default(),
                });
                trait_item.items.push(new_assoc);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
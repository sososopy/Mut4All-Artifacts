use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path, PathArguments, 
    ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, parse_quote,
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

pub struct Apply_Conflicting_Enum_Struct_Repr_188;

impl Mutator for Apply_Conflicting_Enum_Struct_Repr_188 {
    fn name(&self) -> &str {
        "Apply_Conflicting_Enum_Struct_Repr_188"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut target_items: Vec<&mut Item> = file.items.iter_mut().collect();

        if let Some(item) = target_items.choose_mut(&mut rng) {
            if let syn::Item::Struct(s) = item {
                let repr_packed = parse_quote!(#[repr(packed)]);
                let repr_c = parse_quote!(#[repr(C)]);
                let repr_u32 = parse_quote!(#[repr(u32)]);

                s.attrs.push(repr_packed);
                s.attrs.push(repr_c);
                s.attrs.push(repr_u32);
            } else if let syn::Item::Enum(e) = item {
                let repr_packed = parse_quote!(#[repr(packed)]);
                let repr_c = parse_quote!(#[repr(C)]);
                let repr_u32 = parse_quote!(#[repr(u32)]);

                e.attrs.push(repr_packed);
                e.attrs.push(repr_c);
                e.attrs.push(repr_u32);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
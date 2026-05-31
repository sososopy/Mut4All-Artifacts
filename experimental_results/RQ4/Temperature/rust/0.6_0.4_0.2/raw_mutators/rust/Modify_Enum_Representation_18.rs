use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Modify_Enum_Representation_18;

impl Mutator for Modify_Enum_Representation_18 {
    fn name(&self) -> &str {
        "Modify_Enum_Representation_18"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_repr = false;
                for attr in &item_enum.attrs {
                    if attr.path.is_ident("repr") {
                        has_repr = true;
                        break;
                    }
                }
                if has_repr {
                    let new_repr: Attribute = parse_quote!(#[repr(u16)]);
                    item_enum.attrs.retain(|attr| !attr.path.is_ident("repr"));
                    item_enum.attrs.push(new_repr);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with a specified `#[repr(...)]` attribute. It changes the representation to `#[repr(u16)]`, potentially exposing issues in the handling of enum layouts, particularly in contexts where precise memory layout is critical."
    }
}
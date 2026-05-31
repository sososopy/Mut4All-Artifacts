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
    Attribute,
    Meta,
    MetaList,
    NestedMeta,
};

use crate::mutator::Mutator;

pub struct Modify_Enum_Representation_18;

impl Mutator for Modify_Enum_Representation_18 {
    fn name(&self) -> &str {
        "Modify_Enum_Representation_18"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                if let Some(attr) = item_enum.attrs.iter_mut().find(|a| a.path().is_ident("repr")) {
                    if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                        let mut rng = thread_rng();
                        let new_repr = match meta_list.nested.first() {
                            Some(NestedMeta::Meta(Meta::Path(path))) => {
                                if path.is_ident("u32") {
                                    "u16"
                                } else if path.is_ident("u16") {
                                    "u32"
                                } else {
                                    "C"
                                }
                            }
                            _ => "C",
                        };
                        *attr = parse_quote! { #[repr(#new_repr)] };
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enums with a `#[repr(...)]` attribute and modifies it to a different compatible representation. By changing the representation, it aims to uncover potential ABI and layout assumption bugs, especially when enums are used in conjunction with other data structures."
    }
}
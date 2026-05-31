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

pub struct Trait_Fn_Repr_Align_Invalid_27;

impl Mutator for Trait_Fn_Repr_Align_Invalid_27 {
    fn name(&self) -> &str {
        "Trait_Fn_Repr_Align_Invalid_27"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Fn(item_fn) = item {
                        let mut has_repr = false;
                        for attr in &mut item_fn.attrs {
                            if attr.path.is_ident("repr") {
                                has_repr = true;
                                if let Ok(meta) = attr.parse_meta() {
                                    if let Meta::List(meta_list) = meta {
                                        let mut has_align = false;
                                        let mut new_meta_list = meta_list.clone();
                                        new_meta_list.nested.clear();
                                        for nested in meta_list.nested {
                                            if let NestedMeta::Meta(Meta::Path(path)) = &nested {
                                                if path.is_ident("align") {
                                                    has_align = true;
                                                    new_meta_list
                                                        .nested
                                                        .push(parse_quote!(align));
                                                } else {
                                                    new_meta_list.nested.push(nested);
                                                }
                                            } else {
                                                new_meta_list.nested.push(nested);
                                            }
                                        }
                                        if !has_align {
                                            new_meta_list.nested.push(parse_quote!(align));
                                        }
                                        *attr = parse_quote!(#[#new_meta_list]);
                                    }
                                }
                            }
                        }
                        if !has_repr {
                            item_fn
                                .attrs
                                .push(parse_quote!(#[repr(align)]));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait functions by ensuring they have a `#[repr(align)]` attribute with an invalid or missing alignment value, which should trigger a compiler error related to attribute handling."
    }
}
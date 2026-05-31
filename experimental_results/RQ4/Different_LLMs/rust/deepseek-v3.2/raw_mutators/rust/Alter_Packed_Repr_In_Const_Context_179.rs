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

pub struct Alter_Packed_Repr_In_Const_Context_179;

impl Mutator for Alter_Packed_Repr_In_Const_Context_179 {
    fn name(&self) -> &str {
        "Alter_Packed_Repr_In_Const_Context_179"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_used_types = HashSet::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                if let Type::Path(type_path) = &item_const.ty {
                    const_used_types.insert(type_path.path.clone());
                }
            }
            if let Item::Static(item_static) = item {
                if let Type::Path(type_path) = &item_static.ty {
                    const_used_types.insert(type_path.path.clone());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if const_used_types.contains(&item_struct.ident.clone().into()) {
                    let mut repr_attr = None;
                    for attr in &item_struct.attrs {
                        if attr.path().is_ident("repr") {
                            repr_attr = Some(attr);
                        }
                    }

                    if let Some(attr) = repr_attr {
                        if let Ok(meta) = attr.parse_meta() {
                            if let Meta::List(list) = meta {
                                if list.path.is_ident("packed") {
                                    if let Some(nested) = list.nested.first() {
                                        if let NestedMeta::Meta(Meta::NameValue(name_value)) = nested {
                                            if let Lit::Int(lit_int) = &name_value.lit {
                                                let current_val = lit_int.base10_parse::<u8>().unwrap_or(1);
                                                let new_val = if current_val > 1 { current_val - 1 } else { 1 };
                                                let new_attr = Attribute {
                                    style: attr.style,
                                    path: attr.path.clone(),
                                    tokens: quote::quote_spanned!(attr.span() => (packed = #new_val)).into(),
                                };
                                let attr_index = item_struct.attrs.iter().position(|a| a.path().is_ident("repr")).unwrap();
                                item_struct.attrs[attr_index] = new_attr;
                            }
                        }
                    }
                }
            } else {
                let new_attr = Attribute {
                    style: syn::AttrStyle::Outer,
                    path: syn::Path::from(syn::Ident::new("repr", Span::call_site())),
                    tokens: quote::quote!(packed).into(),
                };
                item_struct.attrs.push(new_attr);
            }
        }
    }
            if let Item::Union(item_union) = item {
                if const_used_types.contains(&item_union.ident.clone().into()) {
                    let mut repr_attr = None;
                    for attr in &item_union.attrs {
                        if attr.path().is_ident("repr") {
                            repr_attr = Some(attr);
                        }
                    }

                    if let Some(attr) = repr_attr {
                        if let Ok(meta) = attr.parse_meta() {
                            if let Meta::List(list) = meta {
                                if list.path.is_ident("packed") {
                                    if let Some(nested) = list.nested.first() {
                                        if let NestedMeta::Meta(Meta::NameValue(name_value)) = nested {
                                            if let Lit::Int(lit_int) = &name_value.lit {
                                                let current_val = lit_int.base10_parse::<u8>().unwrap_or(1);
                                                let new_val = if current_val > 1 { current_val - 1 } else { 1 };
                                                let new_attr = Attribute {
                                    style: attr.style,
                                    path: attr.path.clone(),
                                    tokens: quote::quote_spanned!(attr.span() => (packed = #new_val)).into(),
                                };
                                let attr_index = item_union.attrs.iter().position(|a| a.path().is_ident("repr")).unwrap();
                                item_union.attrs[attr_index] = new_attr;
                            }
                        }
                    }
                }
            } else {
                let new_attr = Attribute {
                    style: syn::AttrStyle::Outer,
                    path: syn::Path::from(syn::Ident::new("repr", Span::call_site())),
                    tokens: quote::quote!(packed).into(),
                };
                item_union.attrs.push(new_attr);
            }
        }
    }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct and union definitions that are used in const or static contexts. It alters their representation attributes to introduce or modify packed alignments. If the type already has a #[repr(packed(N))] attribute, it decreases N by 1 (minimum 1). If no packed representation exists, it adds #[repr(packed)]. This creates mismatches between ABI alignment and layout alignment in constant evaluation, potentially triggering layout computation bugs in the compiler similar to the reported ICE."
    }
}
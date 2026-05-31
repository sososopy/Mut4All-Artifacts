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
use syn::Meta::NestedMeta;

use crate::mutator::Mutator;

pub struct Add_Layout_Default_Variant_180;

impl Mutator for Add_Layout_Default_Variant_180 {
    fn name(&self) -> &str {
        "Add_Layout_Default_Variant_180"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut unit_structs = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let syn::Fields::Unit = struct_item.fields {
                    let attr = parse_quote!(# [rustc_layout_scalar_valid_range_start(1)]);
                    struct_item.attrs.push(attr);
                    unit_structs.push(struct_item.ident.clone());
                }
            }
        }

        if !unit_structs.is_empty() {
            for item in &mut file.items {
                if let syn::Item::Enum(enum_item) = item {
                    let mut has_default = false;
                    for attr in &enum_item.attrs {
                        if attr.path().is_ident("derive") {
                            if let syn::Meta::List(list) = &attr.meta {
                                for nested in list.nested() {
                                    if let NestedMeta::Meta(meta) = nested {
                                        if let syn::Meta::Path(path) = meta {
                                            if path.is_ident("Default") {
                                                has_default = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if has_default {
                            break;
                        }
                    }
                    if has_default {
                        for variant in &mut enum_item.variants {
                            if let syn::Fields::Unit = variant.fields {
                                let struct_name = unit_structs.get(0).unwrap();
                                let struct_type = syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(struct_name.clone()),
                                });
                                variant.fields = syn::Fields::Unnamed(syn::FieldsUnnamed {
                                    paren_token: syn::token::Paren::default(),
                                    unnamed: syn::punctuated::Punctuated::from_iter(vec![syn::Field {
                                        attrs: Vec::new(),
                                        ident: None,
                                        colon_token: None,
                                        ty: struct_type,
                                        vis: syn::Visibility::Inherited,
                                        mutability: None,
                                    }]),
                                });
                                variant.attrs.push(parse_quote!(# [default]));
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies unit structs and adds a layout_scalar_valid_range attribute to them. It then modifies enums that derive Default by converting the first unit variant into a tuple variant containing the unit struct and marking it as the default. This transformation stresses the compiler's handling of layout constraints and default enum construction, potentially exposing ICEs when invalid ranges conflict with the enum's default initialization."
    }
}
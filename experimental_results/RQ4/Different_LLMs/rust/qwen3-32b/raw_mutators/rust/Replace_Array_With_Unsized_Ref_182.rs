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

pub struct Replace_Array_With_Unsized_Ref_182;

impl Mutator for Replace_Array_With_Unsized_Ref_182 {
    fn name(&self) -> &str {
        "Replace_Array_With_Unsized_Ref_182"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                let struct_name = &struct_item.ident;

                // Check if there's a const fn that returns this struct
                let has_const_fn = file.items.iter().any(|i| {
                    if let syn::Item::Fn(fn_item) = i {
                        if fn_item.sig.constness.is_some() {
                            if let syn::ReturnType::Type(_, ty) = &fn_item.sig.output {
                                if let syn::Type::Path(type_path) = &**ty {
                                    if let Some(ident) = type_path.path.get_ident() {
                                        if ident == struct_name {
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                        false
                    } else {
                        false
                    }
                });

                // Check if there's a static mut declaration of this struct
                let has_static_mut = file.items.iter().any(|i| {
                    if let syn::Item::Static(static_item) = i {
                        if static_item.mutability.is_some() {
                            if let syn::Type::Path(type_path) = &*static_item.ty {
                                if let Some(ident) = type_path.path.get_ident() {
                                    if ident == struct_name {
                                        return true;
                                    }
                                }
                            }
                        }
                        false
                    } else {
                        false
                    }
                });

                if has_const_fn && has_static_mut {
                    for field in &mut struct_item.fields {
                        if let syn::Type::Array(array) = &mut field.ty {
                            // Create a reference to the element type
                            let ref_type = syn::Type::Reference(Box::new(syn::TypeReference {
                                and_token: syn::Token![&](proc_macro2::Span::call_site()),
                                mutability: None,
                                elem: array.elem.clone(),
                            }));
                            array.elem = Box::new(ref_type);
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
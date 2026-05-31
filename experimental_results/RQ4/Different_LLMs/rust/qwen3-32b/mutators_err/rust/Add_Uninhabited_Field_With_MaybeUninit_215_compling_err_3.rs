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

pub struct Add_Uninhabited_Field_With_MaybeUninit_215;

impl Mutator for Add_Uninhabited_Field_With_MaybeUninit_215 {
    fn name(&self) -> &str {
        "Add_Uninhabited_Field_With_MaybeUninit_215"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_structs = HashSet::new();

        // First pass: process structs
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                match &mut s.fields {
                    syn::Fields::Named(named) => {
                        // Add a new field with type !
                        let new_field = syn::Field {
                            attrs: vec![],
                            vis: syn::Visibility::Inherited,
                            ident: Some(Ident::new("b", Span::call_site())),
                            colon_token: Some(Default::default()),
                            ty: syn::Type::Never(syn::TypeNever {
                                bang_token: syn::token::Bang::default(),
                            }),
                        };
                        named.named.push(new_field);
                        modified_structs.insert(s.ident.to_string());
                    }
                    syn::Fields::Unnamed(unnamed) => {
                        // Add a new field with type !
                        let new_field = syn::Field {
                            attrs: vec![],
                            vis: syn::Visibility::Inherited,
                            ident: None,
                            colon_token: None,
                            ty: syn::Type::Never(syn::TypeNever {
                                bang_token: syn::token::Bang::default(),
                            }),
                        };
                        unnamed.unnamed.push(new_field);
                        modified_structs.insert(s.ident.to_string());
                    }
                    _ => {}
                }
            }
        }

        // Second pass: process functions and methods
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let syn::Type::Path(type_path) = &**return_type {
                        if let Some(ident) = type_path.path.get_ident() {
                            if modified_structs.contains(&ident.to_string()) {
                                let struct_type = return_type.clone();
                                func.block = parse_quote! {
                                    unsafe {
                                        std::mem::MaybeUninit::<#struct_type>::uninit().assume_init()
                                    }
                                };
                            }
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, return_type) = &method.sig.output {
                            if let syn::Type::Path(type_path) = &**return_type {
                                if let Some(ident) = type_path.path.get_ident() {
                                    if modified_structs.contains(&ident.to_string()) {
                                        let struct_type = return_type.clone();
                                        method.block = parse_quote! {
                                            unsafe {
                                                std::mem::MaybeUninit::<#struct_type>::uninit().assume_init()
                                            }
                                        };
                                    }
                                }
                            }
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
use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashMap, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemStatic, ItemConst, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Static_Slice_Initializer_With_Unsafe_Ref_62 {
    count: usize,
}

impl Mutator for Replace_Static_Slice_Initializer_With_Unsafe_Ref_62 {
    fn name(&self) -> &str {
        "Replace_Static_Slice_Initializer_With_Unsafe_Ref_62"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut structs = HashMap::new();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                structs.insert(item_struct.ident.clone(), item_struct.clone());
            }
        }

        let mut new_statics = Vec::new();
        let mut items_to_process = std::mem::take(&mut file.items);
        file.items.clear();

        for item in items_to_process {
            match item {
                Item::Static(mut item_static) => {
                    if let Type::Reference(type_ref) = &*item_static.ty {
                        if let Type::Slice(type_slice) = &*type_ref.elem {
                            if let Type::Path(type_path) = &*type_slice.elem {
                                if let Some(ident) = type_path.path.get_ident() {
                                    if let Some(struct_def) = structs.get(ident) {
                                        let has_mut_static_ref = struct_def.fields.iter().any(|field| {
                                            if let Some(field_ident) = field.ident.as_ref() {
                                                if let Type::Reference(type_ref) = &*field.ty {
                                                    if type_ref.mutability.is_some() {
                                                        if let Some(lifetime) = &type_ref.lifetime {
                                                            if lifetime.ident == "static" {
                                                                return true;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            false
                                        });

                                        if has_mut_static_ref {
                                            let new_static_name = format!("GLOBAL_STATIC_{}", self.count + 1);
                                            let new_static_ident = Ident::new(&new_static_name, Span::call_site());
                                            let mut referenced_type = parse_quote!(i32);
                                            for field in &struct_def.fields {
                                                if let Some(field_ident) = field.ident.as_ref() {
                                                    if let Type::Reference(type_ref) = &*field.ty {
                                                        if type_ref.mutability.is_some() {
                                                            if let Some(lifetime) = &type_ref.lifetime {
                                                                if lifetime.ident == "static" {
                                                                    referenced_type = *type_ref.elem.clone();
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }

                                            let new_static = ItemStatic {
                                                attrs: vec![],
                                                vis: parse_quote!(pub),
                                                static_token: parse_quote!(static),
                                                mutability: Some(syn::StaticMutability::Mut(Default::default())),
                                                ident: new_static_ident.clone(),
                                                colon_token: parse_quote!(:),
                                                ty: Box::new(referenced_type),
                                                eq_token: parse_quote!(=),
                                                expr: Box::new(parse_quote!(0)),
                                                semi_token: Default::default(),
                                            };
                                            new_statics.push(new_static);

                                            let struct_name = struct_def.ident.clone();
                                            let field_name = struct_def.fields.iter().find(|field| {
                                                if let Some(field_ident) = field.ident.as_ref() {
                                                    if let Type::Reference(type_ref) = &*field.ty {
                                                        if type_ref.mutability.is_some() {
                                                            if let Some(lifetime) = &type_ref.lifetime {
                                                                if lifetime.ident == "static" {
                                                                    return true;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                false
                                            }).and_then(|field| field.ident.clone());

                                            if let Some(field_name) = field_name {
                                                let expr = parse_quote! {
                                                    #struct_name { #field_name: unsafe { &mut #new_static_ident } }
                                                };
                                                let array_expr = parse_quote! {
                                                    &[#expr]
                                                };
                                                item_static.expr = Box::new(array_expr);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    file.items.push(Item::Static(item_static));
                }
                Item::Const(mut item_const) => {
                    if let Type::Reference(type_ref) = &*item_const.ty {
                        if let Type::Slice(type_slice) = &*type_ref.elem {
                            if let Type::Path(type_path) = &*type_slice.elem {
                                if let Some(ident) = type_path.path.get_ident() {
                                    if let Some(struct_def) = structs.get(ident) {
                                        let has_mut_static_ref = struct_def.fields.iter().any(|field| {
                                            if let Some(field_ident) = field.ident.as_ref() {
                                                if let Type::Reference(type_ref) = &*field.ty {
                                                    if type_ref.mutability.is_some() {
                                                        if let Some(lifetime) = &type_ref.lifetime {
                                                            if lifetime.ident == "static" {
                                                                return true;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            false
                                        });

                                        if has_mut_static_ref {
                                            let new_static_name = format!("GLOBAL_STATIC_{}", self.count + 1);
                                            let new_static_ident = Ident::new(&new_static_name, Span::call_site());
                                            let mut referenced_type = parse_quote!(i32);
                                            for field in &struct_def.fields {
                                                if let Some(field_ident) = field.ident.as_ref() {
                                                    if let Type::Reference(type_ref) = &*field.ty {
                                                        if type_ref.mutability.is_some() {
                                                            if let Some(lifetime) = &type_ref.lifetime {
                                                                if lifetime.ident == "static" {
                                                                    referenced_type = *type_ref.elem.clone();
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }

                                            let new_static = ItemStatic {
                                                attrs: vec![],
                                                vis: parse_quote!(pub),
                                                static_token: parse_quote!(static),
                                                mutability: Some(syn::StaticMutability::Mut(Default::default())),
                                                ident: new_static_ident.clone(),
                                                colon_token: parse_quote!(:),
                                                ty: Box::new(referenced_type),
                                                eq_token: parse_quote!(=),
                                                expr: Box::new(parse_quote!(0)),
                                                semi_token: Default::default(),
                                            };
                                            new_statics.push(new_static);

                                            let struct_name = struct_def.ident.clone();
                                            let field_name = struct_def.fields.iter().find(|field| {
                                                if let Some(field_ident) = field.ident.as_ref() {
                                                    if let Type::Reference(type_ref) = &*field.ty {
                                                        if type_ref.mutability.is_some() {
                                                            if let Some(lifetime) = &type_ref.lifetime {
                                                                if lifetime.ident == "static" {
                                                                    return true;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                false
                                            }).and_then(|field| field.ident.clone());

                                            if let Some(field_name) = field_name {
                                                let expr = parse_quote! {
                                                    #struct_name { #field_name: unsafe { &mut #new_static_ident } }
                                                };
                                                let array_expr = parse_quote! {
                                                    &[#expr]
                                                };
                                                item_const.expr = Box::new(array_expr);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    file.items.push(Item::Const(item_const));
                }
                _ => {
                    file.items.push(item);
                }
            }
        }

        for new_static in new_statics {
            file.items.insert(0, Item::Static(new_static));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static or const items with a type of &'static [T], where T is a struct containing a mutable static reference field. It replaces the initializer with a slice containing an instance of T with an unsafe reference to a newly introduced mutable static variable. This creates nested static dependencies that may trigger invalid scalar layout evaluation during const-checking, testing the compiler's ability to handle complex static reference chains."
    }
}
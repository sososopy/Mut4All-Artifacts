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

pub struct Introduce_Union_With_Trait_Assoc_Type_276;

impl Mutator for Introduce_Union_With_Trait_Assoc_Type_276 {
    fn name(&self) -> &str {
        "Introduce_Union_With_Trait_Assoc_Type_276"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, ref trait_path, _)) = impl_item.trait_ {
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            for stmt in &mut method.block.stmts {
                                if let syn::Stmt::Item(item) = stmt {
                                    if let syn::Item::Struct(_) = item {
                                        let union_name = syn::Ident::new("U", proc_macro2::Span::call_site());
                                        let field_name = syn::Ident::new("a", proc_macro2::Span::call_site());
                                        let field_type = syn::parse_quote!(<Self as #trait_path>::Item);
                                        let new_union = syn::ItemUnion {
                                            attrs: Vec::new(),
                                            vis: syn::Visibility::Inherited,
                                            union_token: syn::token::Union(proc_macro2::Span::call_site()),
                                            ident: union_name,
                                            generics: syn::Generics::default(),
                                            fields: syn::Fields::Named(syn::Named {
                                                brace_token: syn::token::Brace(proc_macro2::Span::call_site()),
                                                named: syn::punctuated::Punctuated::from_iter(vec![syn::Field {
                                                    attrs: Vec::new(),
                                                    vis: syn::Visibility::Inherited,
                                                    ident: Some(field_name),
                                                    colon_token: Some(syn::token::Colon(proc_macro2::Span::call_site())),
                                                    mutability: syn::FieldMutability::None,
                                                    ty: field_type,
                                                }]),
                                            }),
                                        };
                                        *item = syn::Item::Union(new_union);
                                    } else if let syn::Item::Enum(_) = item {
                                        let union_name = syn::Ident::new("U", proc_macro2::Span::call_site());
                                        let field_name = syn::Ident::new("a", proc_macro2::Span::call_site());
                                        let field_type = syn::parse_quote!(<Self as #trait_path>::Item);
                                        let new_union = syn::ItemUnion {
                                            attrs: Vec::new(),
                                            vis: syn::Visibility::Inherited,
                                            union_token: syn::token::Union(proc_macro2::Span::call_site()),
                                            ident: union_name,
                                            generics: syn::Generics::default(),
                                            fields: syn::Fields::Named(syn::Named {
                                                brace_token: syn::token::Brace(proc_macro2::Span::call_site()),
                                                named: syn::punctuated::Punctuated::from_iter(vec![syn::Field {
                                                    attrs: Vec::new(),
                                                    vis: syn::Visibility::Inherited,
                                                    ident: Some(field_name),
                                                    colon_token: Some(syn::token::Colon(proc_macro2::Span::call_site())),
                                                    mutability: syn::FieldMutability::None,
                                                    ty: field_type,
                                                }]),
                                            }),
                                        };
                                        *item = syn::Item::Union(new_union);
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
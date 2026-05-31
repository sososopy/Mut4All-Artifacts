use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashMap,
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    *,
};
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

pub struct Replace_Const_Param_With_AssocType_288;

impl Mutator for Replace_Const_Param_With_AssocType_288 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_AssocType_288"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_types = HashSet::new();

        for item in &file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &func.sig.generics.params {
                    if let GenericParam::Const(c) = param {
                        let ty_str = c.ty.to_token_stream().to_string();
                        const_types.insert(ty_str);
                    }
                }
            }
        }

        let mut trait_map = HashMap::new();
        let mut counter = 0;
        for ty_str in &const_types {
            let trait_name = Ident::new(&format!("Helper_288_{}", counter), Span::call_site());
            counter += 1;

            let ty: Type = parse_quote!(#ty_str);
            let new_trait = parse_quote! {
                trait #trait_name {
                    type Assoc;
                }
            };
            file.items.push(Item::Trait(new_trait));

            let new_impl = parse_quote! {
                impl #trait_name for #ty {
                    type Assoc = #ty;
                }
            };
            file.items.push(Item::Impl(new_impl));

            trait_map.insert(ty_str.clone(), trait_name);
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(c) = param {
                        let original_type_str = c.ty.to_token_stream().to_string();
                        let trait_name = trait_map.get(&original_type_str).unwrap().clone();

                        let original_type: Type = parse_quote!(#original_type_str);
                        let assoc_type_path = parse_quote!(
                            <#original_type as #trait_name>::Assoc
                        );
                        c.ty = assoc_type_path;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const parameter types with associated types from newly created traits. It introduces nested trait references to stress the compiler's normalization logic, potentially exposing issues in handling const parameters with ADT types through trait indirection."
    }
}
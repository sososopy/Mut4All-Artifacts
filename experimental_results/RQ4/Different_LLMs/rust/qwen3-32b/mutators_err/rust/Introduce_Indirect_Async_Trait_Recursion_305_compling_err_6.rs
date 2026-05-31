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

pub struct Introduce_Indirect_Async_Trait_Recursion_305;

impl Mutator for Introduce_Indirect_Async_Trait_Recursion_305 {
    fn name(&self) -> &str {
        "Introduce_Indirect_Async_Trait_Recursion_305"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modifications = Vec::new();
        let mut new_items = Vec::new();
        
        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                let original_trait_name = &trait_def.ident;
                for trait_item in &mut trait_def.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if method.sig.asyncness.is_some() {
                            if let syn::ReturnType::Type(_, return_type) = &method.sig.output {
                                if let syn::Type::Path(type_path) = *return_type {
                                    if let Some(qself) = &type_path.qself {
                                        let method_name = &method.sig.ident;
                                        let assoc_type_name = type_path.path.segments.last().unwrap().ident.to_string();
                                        modifications.push( (original_trait_name.to_string(), method_name, assoc_type_name) );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        for (original_trait_name, method_name, assoc_type_name) in modifications {
            for item in &mut file.items {
                if let syn::Item::Trait(trait_def) = item {
                    if trait_def.ident.to_string() == original_trait_name {
                        for trait_item in &mut trait_def.items {
                            if let syn::TraitItem::Type(trait_type) = trait_item {
                                if trait_type.ident.to_string() == assoc_type_name {
                                    let trait_name = &trait_type.ident;
                                    let new_bound = parse_quote!(#trait_name);
                                    trait_type.bounds.push(new_bound);
                                    let new_trait = parse_quote! {
                                        trait #trait_name {
                                            async fn second(self) -> Self;
                                        }
                                    };
                                    new_items.push(syn::Item::Trait(new_trait));
                                    let new_impl = parse_quote! {
                                        impl<T: #trait_name + #original_trait_name> T {
                                            async fn second(self) -> Self {
                                                self.#method_name().await
                                            }
                                        }
                                    };
                                    new_items.push(syn::Item::Impl(new_impl));
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
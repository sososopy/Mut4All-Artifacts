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
        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                let original_trait_name = &trait_def.ident;
                for trait_item in &mut trait_def.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        if method.sig.asyncness.is_some() {
                            if let syn::ReturnType::Type(_, return_type) = &method.sig.output {
                                if let syn::Type::Path(type_path) = &**return_type {
                                    if let Some(qself) = &type_path.qself {
                                        let method_name = &method.sig.ident;
                                        let assoc_type_name = type_path.path.segments.last().unwrap().ident.to_string();
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
                                                    file.items.push(syn::Item::Trait(Box::new(new_trait)));
                                                    let new_impl = parse_quote! {
                                                        impl<T: #trait_name + #original_trait_name> T {
                                                            async fn second(self) -> Self {
                                                                self.#method_name().await
                                                            }
                                                        }
                                                    };
                                                    file.items.push(syn::Item::Impl(Box::new(new_impl)));
                                                    break;
                                                }
                                            }
                                        }
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
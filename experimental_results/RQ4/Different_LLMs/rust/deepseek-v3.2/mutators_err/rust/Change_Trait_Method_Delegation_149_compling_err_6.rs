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

pub struct Change_Trait_Method_Delegation_149;

impl Mutator for Change_Trait_Method_Delegation_149 {
    fn name(&self) -> &str {
        "Change_Trait_Method_Delegation_149"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut trait_methods = HashSet::new();
        let mut inherent_methods = HashSet::new();
        let mut delegation_items = Vec::new();

        // First pass: collect all trait and inherent methods
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        trait_methods.insert((trait_item.ident.clone(), method.sig.ident.clone()));
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        inherent_methods.insert((impl_item.self_ty.clone(), method.sig.ident.clone()));
                    }
                }
            }
        }

        // Second pass: find delegation reuse items
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Verbatim(verbatim) = impl_item {
                        let content = verbatim.to_string();
                        if content.contains("reuse") && content.contains("delegate") {
                            delegation_items.push((impl_item, verbatim.clone()));
                        }
                    }
                }
            }
        }

        // Mutate delegation items
        for (impl_item, verbatim) in delegation_items {
            let original_content = verbatim.to_string();
            let re = Regex::new(r"reuse\s*<([^>]+)>\s*::\s*\{([^}]+)\}\s*\{([^}]+)\}").unwrap();
            if let Some(caps) = re.captures(&original_content) {
                let type_trait_part = caps.get(1).unwrap().as_str();
                let method_name =2.unwrap().as_str();
                let delegate_expr = caps.get(3).unwrap().as_str();

                // Parse type and trait from the reuse syntax
                let type_trait_re = Regex::new(r"([^ ]+)\s+as\s+([^ ]+)").unwrap();
                if let Some(type_trait_caps) = type_trait_re.captures(type_trait_part) {
                    let type_name = type_trait_caps.get(1).unwrap().as_str();
                    let trait_name = type_trait_caps.get(2).unwrap().as_str();

                    // Find alternative method
                    let mut candidates = Vec::new();
                    for (trait_ident, method_ident) in &trait_methods {
                        if trait_ident != trait_name && method_ident == method_name {
                            candidates.push((trait_ident.clone(), method_ident.clone()));
                        }
                    }
                    for (self_ty, method_ident) in &inherent_methods {
                        if method_ident == method_name {
                            candidates.push((Ident::new("Self", Span::call_site()), method_ident.clone()));
                        }
                    }

                    if candidates.is_empty() {
                        // Create new trait and implement it
                        let new_trait_name = Ident::new("AlternativeTrait", Span::call_site());
                        let new_trait = Item::Trait(parse_quote! {
                            trait AlternativeTrait {
                                fn #method_name() -> ();
                            }
                        });
                        file.items.push(new_trait);

                        let type_ident = Ident::new(type_name, Span::call_site());
                        let new_impl = Item::Impl(parse_quote! {
                            impl AlternativeTrait for #type_ident {
                                fn #method_name() -> () {
                                    ()
                                }
                            }
                        });
                        file.items.push(new_impl);

                        candidates.push((new_trait_name, Ident::new(method_name, Span::call_site())));
                    }

                    if let Some((new_trait, new_method)) = candidates.choose(&mut rng) {
                        let new_content = format!("reuse <{} as {}>::{{{}}} {{{}}}", type_name, new_trait, new_method, delegate_expr);
                        *impl_item = syn::ImplItem::Verbatim(parse_quote! { #new_content });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets delegation reuse syntax in Rust's fn_delegation feature. It changes the trait method being delegated to a different trait method with a compatible signature, either from another trait or inherent impl. If no compatible alternative exists, it introduces a new trait with a matching method signature and implements it for the relevant type. This transformation stresses the compiler's delegation resolution logic, potentially triggering path resolution edge cases and exposing bugs in delegation analysis."
    }
}
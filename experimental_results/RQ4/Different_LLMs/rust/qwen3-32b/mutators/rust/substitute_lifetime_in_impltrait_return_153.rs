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

pub struct Substitute_Lifetime_In_ImplTrait_Return_153;

impl Mutator for Substitute_Lifetime_In_ImplTrait_Return_153 {
    fn name(&self) -> &str {
        "Substitute_Lifetime_In_ImplTrait_Return_153"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let existing_lifetimes = self.collect_lifetimes(&func.sig.generics);
                if existing_lifetimes.is_empty() {
                    continue;
                }
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        for bound in &mut type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(ref mut bound_lifetimes) = trait_bound.lifetimes {
                                    for param in &mut bound_lifetimes.lifetimes {
                                        if let GenericParam::Lifetime(lt_param) = param {
                                            if existing_lifetimes.contains(&lt_param.lifetime.ident) {
                                                let new_lt = self.generate_new_lifetime(&existing_lifetimes);
                                                lt_param.lifetime.ident = new_lt.ident;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        let existing_lifetimes = self.collect_lifetimes(&method.sig.generics);
                        if existing_lifetimes.is_empty() {
                            continue;
                        }
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                                for bound in &mut type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(ref mut bound_lifetimes) = trait_bound.lifetimes {
                                            for param in &mut bound_lifetimes.lifetimes {
                                                if let GenericParam::Lifetime(lt_param) = param {
                                                    if existing_lifetimes.contains(&lt_param.lifetime.ident) {
                                                        let new_lt = self.generate_new_lifetime(&existing_lifetimes);
                                                        lt_param.lifetime.ident = new_lt.ident;
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a lifetime parameter in the `impl Trait` return type with a new, undeclared lifetime. This introduces a region parameter inconsistency during substitution, potentially triggering ICEs by accessing an out-of-bounds index in the type substitution process. It targets functions with `impl Trait` returns and generic lifetimes, altering the return type to reference an undefined lifetime."
    }
}

impl Substitute_Lifetime_In_ImplTrait_Return_153 {
    fn collect_lifetimes(&self, generics: &syn::Generics) -> HashSet<Ident> {
        let mut lifetimes = HashSet::new();
        for param in &generics.params {
            if let GenericParam::Lifetime(lt) = param {
                lifetimes.insert(lt.lifetime.ident.clone());
            }
        }
        lifetimes
    }

    fn generate_new_lifetime(&self, existing_lifetimes: &HashSet<Ident>) -> Lifetime {
        let mut rng = thread_rng();
        let mut new_lt = Ident::new("c", Span::call_site());
        while existing_lifetimes.contains(&new_lt) {
            let c = new_lt.to_string().chars().next().unwrap();
            let next_char = (c as u8 + 1) as char;
            new_lt = Ident::new(&next_char.to_string(), Span::call_site());
        }
        Lifetime::new(&new_lt.to_string(), Span::call_site())
    }
}
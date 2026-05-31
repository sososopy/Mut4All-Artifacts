use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Generator_Usage_In_Trait_And_Impl_165;

impl Mutator for Modify_Generator_Usage_In_Trait_And_Impl_165 {
    fn name(&self) -> &str {
        "Modify_Generator_Usage_In_Trait_And_Impl_165"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        if let Some(bound) = type_item.bounds.first_mut() {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Generator") {
                                    trait_bound.path.segments.last_mut().unwrap().arguments = syn::PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: {
                                                let mut args = Punctuated::new();
                                                args.push(GenericArgument::Constraint(syn::Constraint {
                                                    ident: syn::Ident::new("Yield", Span::call_site()),
                                                    colon_token: token::Colon::default(),
                                                    bounds: Punctuated::new(),
                                                    generics: None,
                                                }));
                                                args.push(GenericArgument::Constraint(syn::Constraint {
                                                    ident: syn::Ident::new("Return", Span::call_site()),
                                                    colon_token: token::Colon::default(),
                                                    bounds: Punctuated::new(),
                                                    generics: None,
                                                }));
                                                args
                                            },
                                            gt_token: token::Gt::default(),
                                        }
                                    );
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(type_item) = impl_item {
                        if let syn::Type::ImplTrait(type_impl_trait) = &type_item.ty {
                            for bound in &type_impl_trait.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("Generator") {
                                        type_item.ty = syn::parse_quote! {
                                            impl Generator<Yield = u32, Return = String>
                                        };
                                    }
                                }
                            }
                        }
                    }
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "run" {
                            func.block = syn::parse_quote!({
                                move || {
                                    yield 1;
                                    return "done".to_string();
                                }
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies traits and implementations that use the `Generator` trait by changing the `Yield` and `Return` types to `u32` and `String`, respectively. It then updates the corresponding implementation to match the new associated type but introduces a mismatch in runtime behavior. This transformation aims to test the compiler's handling of type inference and generator logic under altered type expectations."
    }
}
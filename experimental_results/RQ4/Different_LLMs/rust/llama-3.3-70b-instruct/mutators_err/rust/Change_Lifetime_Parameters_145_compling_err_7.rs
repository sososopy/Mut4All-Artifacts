use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
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

pub struct Change_Lifetime_Parameters_145;

impl Mutator for Change_Lifetime_Parameters_145 {
    fn name(&self) -> &str {
        "Change_Lifetime_Parameters_145"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    let generics_clone = generics.clone();
                    self.mutate_lifetimes(generics_clone, generics);
                }
            } else if let syn::Item::Struct(struct_) = item {
                if let Some(generics) = &mut struct_.generics {
                    let generics_clone = generics.clone();
                    self.mutate_lifetimes(generics_clone, generics);
                }
            } else if let syn::Item::Trait(trait_) = item {
                if let Some(generics) = &mut trait_.generics {
                    let generics_clone = generics.clone();
                    self.mutate_lifetimes(generics_clone, generics);
                }
            } else if let syn::Item::Impl(impl_) = item {
                if let Some(generics) = &mut impl_.generics {
                    let generics_clone = generics.clone();
                    self.mutate_lifetimes(generics_clone, generics);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets Rust code constructs that involve lifetime parameters. It applies to functions, structs, and traits that have lifetime parameters specified. The operator works by modifying the lifetime parameters in the following ways: changing the name of a lifetime parameter, adding a new lifetime parameter, removing an existing lifetime parameter, and swapping the order of lifetime parameters."
    }
}

impl Change_Lifetime_Parameters_145 {
    fn mutate_lifetimes(
        &self,
        generics_clone: syn::Generics,
        generics: &mut syn::Generics,
    ) {
        let mut rng = thread_rng();
        let params: Vec<_> = generics_clone
            .params
            .iter()
            .filter_map(|param| {
                if let GenericParam::Lifetime(LifetimeParam { lifetime, .. }) = param {
                    Some(lifetime)
                } else {
                    None
                }
            })
            .collect();

        if params.is_empty() {
            return;
        }

        let action: u8 = rng.gen_range(0..4);
        match action {
            0 => {
                let params_clone = params.clone();
                self.rename_lifetime(params_clone, generics);
            }
            1 => self.add_new_lifetime(generics),
            2 => {
                let params_clone = params.clone();
                self.remove_lifetime(params_clone, generics);
            }
            3 => {
                let params_clone = params.clone();
                self.swap_lifetimes(params_clone, generics);
            }
            _ => unreachable!(),
        }
    }

    fn rename_lifetime(
        &self,
        params: Vec<&syn::Lifetime>,
        generics: &mut syn::Generics,
    ) {
        let param = params.choose(&mut thread_rng()).unwrap();
        let new_name = Ident::new(
            &format!("{}_new", param.ident),
            Span::call_site(),
        );
        for param_mut in &mut generics.params {
            if let GenericParam::Lifetime(LifetimeParam {
                lifetime: param_lifetime,
                ..
            }) = param_mut
            {
                if *param_lifetime == **param {
                    param_lifetime.ident = new_name.clone();
                }
            }
        }
    }

    fn add_new_lifetime(&self, generics: &mut syn::Generics) {
        let new_name = Ident::new(
            &format!("new_{}", thread_rng().r#gen::<u32>()),
            Span::call_site(),
        );
        generics.params.push(GenericParam::Lifetime(LifetimeParam {
            attrs: vec![],
            lifetime: Lifetime {
                apostrophe: token::Apostrophe(Span::call_site()),
                ident: new_name,
            },
            colon_token: None,
            bounds: Punctuated::new(),
        }));
    }

    fn remove_lifetime(
        &self,
        params: Vec<&syn::Lifetime>,
        generics: &mut syn::Generics,
    ) {
        if let Some(param) = params.choose(&mut thread_rng()) {
            generics.params = generics
                .params
                .clone()
                .into_iter()
                .filter(|param_| {
                    if let GenericParam::Lifetime(LifetimeParam {
                        lifetime: param_lifetime,
                        ..
                    }) = param_
                    {
                        param_lifetime != *param
                    } else {
                        true
                    }
                })
                .collect();
        }
    }

    fn swap_lifetimes(
        &self,
        params: Vec<&syn::Lifetime>,
        generics: &mut syn::Generics,
    ) {
        if params.len() < 2 {
            return;
        }
        let (first, second) = {
            let mut indices: Vec<_> = (0..params.len()).collect();
            indices.shuffle(&mut thread_rng());
            let first = indices.pop().unwrap();
            let second = indices.pop().unwrap();
            (first, second)
        };
        let (first_param, second_param) = (params[first], params[second]);
        for param in &mut generics.params {
            if let GenericParam::Lifetime(LifetimeParam {
                lifetime: param_lifetime,
                ..
            }) = param
            {
                if *param_lifetime == *first_param {
                    param_lifetime.ident = second_param.ident.clone();
                } else if *param_lifetime == *second_param {
                    param_lifetime.ident = first_param.ident.clone();
                }
            }
        }
    }
}
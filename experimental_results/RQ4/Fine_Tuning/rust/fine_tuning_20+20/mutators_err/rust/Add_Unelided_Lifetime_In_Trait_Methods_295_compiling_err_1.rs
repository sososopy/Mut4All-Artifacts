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

pub struct Add_Unelided_Lifetime_In_Trait_Methods_295;

fn find_unused_lifetime<'a>(existing: &HashSet<&'a str>) -> &'a str {
    let candidates = [
        "'a", "'b", "'c", "'d", "'e", "'f", "'g", "'h", "'i", "'j", "'k", "'l", "'m", "'n", "'o",
        "'p", "'q", "'r", "'s", "'t", "'u", "'v", "'w", "'x", "'y", "'z", "'aa", "'bb", "'cc",
        "'dd", "'ee", "'ff", "'gg", "'hh", "'ii", "'jj", "'kk", "'ll", "'mm", "'nn", "'oo", "'pp",
        "'qq", "'rr", "'ss", "'tt", "'uu", "'vv", "'ww", "'xx", "'yy", "'zz",
    ];
    for &candidate in &candidates {
        if !existing.contains(candidate) {
            return candidate;
        }
    }
    "'z"
}

impl Mutator for Add_Unelided_Lifetime_In_Trait_Methods_295 {
    fn name(&self) -> &str {
        "Add_Unelided_Lifetime_In_Trait_Methods_295"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut has_impl_trait = false;
                let mut associated_type_idents = HashSet::new();
                for item in &trait_item.items {
                    if let TraitItem::Type(assoc_type) = item {
                        associated_type_idents.insert(assoc_type.ident.to_string());
                    }
                }
                for item in &trait_item.items {
                    if let TraitItem::Method(method) = item {
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                has_impl_trait = true;
                                for bound in &type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(last_segment) =
                                            trait_bound.path.segments.last()
                                        {
                                            if associated_type_idents
                                                .contains(&last_segment.ident.to_string())
                                            {
                                                let mut existing_lifetimes = HashSet::new();
                                                for param in &method.sig.generics.params {
                                                    if let GenericParam::Lifetime(lifetime) = param
                                                    {
                                                        existing_lifetimes
                                                            .insert(lifetime.lifetime.ident.to_string());
                                                    }
                                                }
                                                let new_lifetime =
                                                    find_unused_lifetime(&existing_lifetimes);
                                                let new_lifetime_ident = Ident::new(
                                                    new_lifetime,
                                                    Span::call_site(),
                                                );
                                                let new_lifetime_token = Lifetime::new(
                                                    new_lifetime,
                                                    Span::call_site(),
                                                );
                                                let new_lifetime_param = LifetimeParam {
                                                    attrs: Vec::new(),
                                                    lifetime: new_lifetime_token.clone(),
                                                    colon_token: None,
                                                    bounds: Punctuated::new(),
                                                };
                                                method.sig.generics.params.push(
                                                    GenericParam::Lifetime(new_lifetime_param),
                                                );
                                                let new_bound: TypeParamBound =
                                                    parse_quote!(#last_segment : #new_lifetime_ident);
                                                let mut new_bounds = Punctuated::new();
                                                for bound in &type_impl_trait.bounds {
                                                    new_bounds.push(bound.clone());
                                                }
                                                new_bounds.push(new_bound);
                                                let new_impl_trait = TypeImplTrait {
                                                    impl_token: type_impl_trait.impl_token,
                                                    bounds: new_bounds,
                                                };
                                                let new_return_type =
                                                    Type::ImplTrait(new_impl_trait);
                                                method.sig.output =
                                                    ReturnType::Type(Default::default(), Box::new(new_return_type));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if has_impl_trait {
                    let mut existing_lifetimes = HashSet::new();
                    for param in &trait_item.generics.params {
                        if let GenericParam::Lifetime(lifetime) = param {
                            existing_lifetimes.insert(lifetime.lifetime.ident.to_string());
                        }
                    }
                    let new_lifetime = find_unused_lifetime(&existing_lifetimes);
                    let new_lifetime_ident = Ident::new(new_lifetime, Span::call_site());
                    let new_lifetime_token = Lifetime::new(new_lifetime, Span::call_site());
                    let new_lifetime_param = LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: new_lifetime_token.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    };
                    trait_item
                        .generics
                        .params
                        .push(GenericParam::Lifetime(new_lifetime_param));
                    for item in &mut trait_item.items {
                        if let TraitItem::Method(method) = item {
                            if let ReturnType::Type(_, return_type) = &method.sig.output {
                                if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                    let mut new_bounds = Punctuated::new();
                                    for bound in &type_impl_trait.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(last_segment) =
                                                trait_bound.path.segments.last()
                                            {
                                                if associated_type_idents
                                                    .contains(&last_segment.ident.to_string())
                                                {
                                                    let new_bound: TypeParamBound = parse_quote!(
                                                        #last_segment : #new_lifetime_ident
                                                    );
                                                    new_bounds.push(new_bound);
                                                }
                                            }
                                        }
                                        new_bounds.push(bound.clone());
                                    }
                                    let new_impl_trait = TypeImplTrait {
                                        impl_token: type_impl_trait.impl_token,
                                        bounds: new_bounds,
                                    };
                                    let new_return_type = Type::ImplTrait(new_impl_trait);
                                    method.sig.output = ReturnType::Type(
                                        Default::default(),
                                        Box::new(new_return_type),
                                    );
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait methods that return `impl Trait` with associated type constraints. It introduces an unelided lifetime in the associated type bound within the `impl Trait` return, using a lifetime parameter not declared in the method or trait. This stresses the compiler's handling of lifetimes and opaque types, aiming to expose bugs related to lifetime resolution and trait system interactions."
    }
}
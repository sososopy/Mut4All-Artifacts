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

pub struct Impl_Trait_And_Lifetime_Transformation_42;

impl Mutator for Impl_Trait_And_Lifetime_Transformation_42 {
    fn name(&self) -> &str {
        "Impl_Trait_And_Lifetime_Transformation_42"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut rng = thread_rng();
                let new_lifetime_name = Ident::new("'b", Span::call_site());
                let has_lifetimes = func.sig.generics.lifetimes().next().is_some();
                
                if !has_lifetimes {
                    func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: Lifetime::new(&new_lifetime_name.to_string(), Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                }
                
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            let bounds = &mut type_impl_trait.bounds;
                            for bound in bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(lifetimes) = &mut trait_bound.lifetimes {
                                        for lifetime_param in &mut lifetimes.lifetimes {
                                            if let GenericParam::Lifetime(lifetime) = lifetime_param {
                                                let new_lifetime = if rng.gen_bool(0.5) {
                                                    Lifetime::new("'_", Span::call_site())
                                                } else {
                                                    Lifetime::new(&new_lifetime_name.to_string(), Span::call_site())
                                                };
                                                lifetime.lifetime = new_lifetime;
                                            }
                                        }
                                    } else {
                                        trait_bound.lifetimes = Some(BoundLifetimes {
                                            for_token: token::For::default(),
                                            lt_token: token::Lt::default(),
                                            lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                                                attrs: Vec::new(),
                                                lifetime: Lifetime::new("'_", Span::call_site()),
                                                colon_token: None,
                                                bounds: Punctuated::new(),
                                            })]),
                                            gt_token: token::Gt::default(),
                                        });
                                    }
                                }
                            }
                        }
                    }
                }

                if rng.gen_bool(0.5) {
                    let new_param: FnArg = parse_quote!(another_param: impl OtherTrait<'a> + '_);
                    func.sig.inputs.push(new_param);
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
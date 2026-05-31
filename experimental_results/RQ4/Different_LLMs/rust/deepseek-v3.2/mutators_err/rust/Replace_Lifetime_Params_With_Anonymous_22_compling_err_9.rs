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

pub struct Replace_Lifetime_Params_With_Anonymous_22;

impl Mutator for Replace_Lifetime_Params_With_Anonymous_22 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Params_With_Anonymous_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    let lifetime_params = func.sig.generics.params.iter().filter(|p| matches!(p, GenericParam::Lifetime(_))).count();
                    if lifetime_params >= 2 {
                        let mut new_params = Punctuated::new();
                        for param in &func.sig.generics.params {
                            if !matches!(param, GenericParam::Lifetime(_)) {
                                new_params.push(param.clone());
                            }
                        }
                        if new_params.len() < func.sig.generics.params.len() {
                            new_params.push(GenericParam::Lifetime(LifetimeParam {
                                attrs: Vec::new(),
                                colon_token: None,
                                lifetime: Lifetime {
                                    apostrophe: Span::call_site(),
                                    ident: Ident::new("_", Span::call_site()),
                                },
                                bounds: Punctuated::new(),
                            }));
                        }
                        func.sig.generics.params = new_params;

                        let mut visitor = LifetimeReplacementVisitor;
                        syn::visit_mut::visit_item_fn_mut(&mut visitor, func);
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item_fn in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item_fn {
                        if func.sig.asyncness.is_some() {
                            let lifetime_params = func.sig.generics.params.iter().filter(|p| matches!(p, GenericParam::Lifetime(_))).count();
                            if lifetime_params >= 2 {
                                let mut new_params = Punctuated::new();
                                for param in &func.sig.generics.params {
                                    if !matches!(param, GenericParam::Lifetime(_)) {
                                        new_params.push(param.clone());
                                    }
                                }
                                if new_params.len() < func.sig.generics.params.len() {
                                    new_params.push(GenericParam::Lifetime(LifetimeParam {
                                        attrs: Vec::new(),
                                        colon_token: None,
                                        lifetime: Lifetime {
                                            apostrophe: Span::call_site(),
                                            ident: Ident::new("_", Span::call_site()),
                                        },
                                        bounds: Punctuated::new(),
                                    }));
                                }
                                func.sig.generics.params = new_params;

                                let mut visitor = LifetimeReplacementVisitor;
                                syn::visit_mut::visit_impl_item_fn_mut(&mut visitor, func);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async function signatures with two or more named lifetime parameters. It replaces all named lifetime parameters with a single anonymous lifetime '_', modifying the generic parameter list and updating lifetime references in the signature. This transformation aims to trigger region-related ICEs by introducing lifetime elision mismatches and challenging the compiler's lifetime inference in async contexts."
    }
}

struct LifetimeReplacementVisitor;

impl VisitMut for LifetimeReplacementVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Reference(type_ref) = ty {
            if let Some(lifetime) = &type_ref.lifetime {
                if lifetime.ident != "_" {
                    type_ref.lifetime = Some(Lifetime {
                        apostrophe: Span::call_site(),
                        ident: Ident::new("_", Span::call_site()),
                    });
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_path_mut(&mut self, path: &mut SynPath) {
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Lifetime(lifetime) = arg {
                        if lifetime.ident != "_" {
                            *lifetime = Lifetime {
                                apostrophe: Span::call_site(),
                                ident: Ident::new("_", Span::call_site()),
                            };
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_path_mut(self, path);
    }
}
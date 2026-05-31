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

pub struct Replace_Async_Trait_Method_Self_With_Impl_Deref_304;

impl Mutator for Replace_Async_Trait_Method_Self_With_Impl_Deref_304 {
    fn name(&self) -> &str {
        "Replace_Async_Trait_Method_Self_With_Impl_Deref_304"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut async_trait_methods = Vec::new();
        let mut deref_import_needed = false;

        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if method.sig.asyncness.is_some() {
                            async_trait_methods.push((trait_item, method));
                        }
                    }
                }
            }
        }

        if async_trait_methods.is_empty() {
            return;
        }

        let (trait_item_ref, method) = async_trait_methods.choose(&mut rng).unwrap();
        let method = unsafe { &mut *(*(method as *const &mut syn::TraitItemFn) as *mut syn::TraitItemFn) };

        let new_self_type = {
            let deref_bound = TraitBound {
                paren_token: None,
                modifier: TraitBoundModifier::None,
                lifetimes: None,
                path: parse_quote!(std::ops::Deref<Target = Self>),
            };
            let clone_bound = TraitBound {
                paren_token: None,
                modifier: TraitBoundModifier::None,
                lifetimes: None,
                path: parse_quote!(Clone),
            };
            let send_bound = TraitBound {
                paren_token: None,
                modifier: TraitBoundModifier::None,
                lifetimes: None,
                path: parse_quote!(Send),
            };

            let mut bounds = Punctuated::new();
            bounds.push(TypeParamBound::Trait(deref_bound));
            bounds.push(TypeParamBound::Trait(clone_bound));
            bounds.push(TypeParamBound::Trait(send_bound));
            bounds.push(TypeParamBound::Lifetime(Lifetime::new("'static", Span::call_site())));

            Type::ImplTrait(TypeImplTrait {
                impl_token: token::Impl { span: Span::call_site() },
                bounds,
            })
        };

        let new_self_arg = FnArg::Typed(PatType {
            attrs: Vec::new(),
            pat: Box::new(Pat::Ident(PatIdent {
                attrs: Vec::new(),
                by_ref: None,
                mutability: None,
                ident: Ident::new("self", Span::call_site()),
                subpat: None,
            })),
            colon_token: token::Colon::default(),
            ty: Box::new(Type::Reference(TypeReference {
                and_token: token::And::default(),
                lifetime: None,
                mutability: None,
                elem: Box::new(new_self_type),
            })),
        });

        method.sig.inputs = Punctuated::from_iter(vec![new_self_arg]);

        for item in &mut file.items {
            if let Item::Mod(module) = item {
                if let Some((_, ref mut content)) = module.content {
                    let mut found_deref = false;
                    for item in content.iter() {
                        if let Item::Use(use_item) = item {
                            if let syn::UseTree::Path(path) = &*use_item.tree {
                                if path.ident == "std" {
                                    if let syn::UseTree::Path(inner_path) = &*path.tree {
                                        if inner_path.ident == "ops" {
                                            if let syn::UseTree::Name(name) = &*inner_path.tree {
                                                if name.ident == "Deref" {
                                                    found_deref = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if !found_deref {
                        let new_use = parse_quote!(use std::ops::Deref;);
                        content.push(new_use);
                    }
                }
            }
        }
    }
}
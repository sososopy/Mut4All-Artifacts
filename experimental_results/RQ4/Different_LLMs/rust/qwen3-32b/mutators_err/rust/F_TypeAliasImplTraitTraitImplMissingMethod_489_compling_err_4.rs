use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct F_TypeAliasImplTraitTraitImplMissingMethod_489;

impl Mutator for F_TypeAliasImplTraitTraitImplMissingMethod_489 {
    fn name(&self) -> &str {
        "F_TypeAliasImplTraitTraitImplMissingMethod_489"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_traits = Vec::new();
        for item in &file.items {
            if let syn::Item::Trait(trait_def) = item {
                let has_assoc_type = trait_def.items.iter().any(|item| {
                    matches!(item, syn::TraitItem::Type(_))
                });
                let has_method = trait_def.items.iter().any(|item| {
                    matches!(item, syn::TraitItem::Fn(_))
                });
                if has_assoc_type && has_method {
                    let trait_path = syn::Path::from(trait_def.ident.clone());
                    let method_name = trait_def.items.iter()
                        .find_map(|item| {
                            if let syn::TraitItem::Fn(fn_item) = item {
                                Some(fn_item.sig.ident.clone())
                            } else {
                                None
                            }
                        })
                        .unwrap();
                    target_traits.push((trait_path, method_name));
                }
            }
        }

        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some(trait_bound) = &impl_item.trait_ {
                    let trait_path = &trait_bound.1;
                    for (target_trait_path, target_method) in &target_traits {
                        if path_eq(trait_path, target_trait_path) {
                            if let syn::Type::Reference(_) = &*impl_item.self_ty {
                                let method_missing = impl_item.items.iter().all(|i| {
                                    if let syn::ImplItem::Fn(fn_item) = i {
                                        fn_item.sig.ident != *target_method
                                    } else {
                                        true
                                    }
                                });
                                if method_missing {
                                    new_items.push(add_function(target_trait_path, target_method));
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

fn path_eq(a: &syn::Path, b: &syn::Path) -> bool {
    a.segments.last().map(|seg| seg.ident == b.segments.last().unwrap().ident)
        == Some(true)
}

fn add_function(trait_path: &syn::Path, method_name: &syn::Ident) -> syn::Item {
    let function_name = syn::Ident::new("test", Span::call_site());
    let lifetime = syn::Lifetime::new("'a", Span::call_site());
    let return_type = syn::ReturnType::Type(
        Span::call_site().into(),
        Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
            impl_token: Default::default(),
            bounds: {
                let mut bounds = syn::punctuated::Punctuated::new();
                bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                    paren_token: None,
                    modifier: syn::TraitBoundModifier::None,
                    lifetimes: None,
                    path: syn::Path::from(syn::Ident::new("Sized", Span::call_site())),
                }));
                bounds
            },
        })),
    );

    let tokens = quote! {
        <&'a () as #trait_path>::#method_name()
    };
    let expr = syn::parse2(tokens).unwrap();

    let new_fn = syn::ItemFn {
        attrs: vec![],
        vis: syn::Visibility::Inherited,
        sig: syn::Signature {
            constness: None,
            asyncness: None,
            unsafety: None,
            abi: None,
            fn_token: Default::default(),
            ident: function_name,
            generics: syn::Generics {
                lt_token: Default::default(),
                params: {
                    let mut params = syn::punctuated::Punctuated::new();
                    params.push(syn::GenericParam::Lifetime(syn::LifetimeParam {
                        attrs: vec![],
                        lifetime: lifetime.clone(),
                        colon_token: None,
                        bounds: syn::punctuated::Punctuated::new(),
                    }));
                    params
                },
                gt_token: Default::default(),
                where_clause: None,
            },
            paren_token: Default::default(),
            inputs: syn::punctuated::Punctuated::new(),
            output: return_type,
            variadic: None,
        },
        block: Box::new(syn::Block {
            brace_token: Default::default(),
            stmts: {
                let mut stmts = syn::punctuated::Punctuated::new();
                stmts.push(syn::Stmt::Expr(expr, None));
                stmts.into_iter().collect()
            },
        }),
    };

    syn::Item::Fn(new_fn)
}
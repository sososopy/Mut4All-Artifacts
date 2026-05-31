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

pub struct Introduce_For_Binder_In_Trait_Object_271;

impl Mutator for Introduce_For_Binder_In_Trait_Object_271 {
    fn name(&self) -> &str {
        "Introduce_For_Binder_In_Trait_Object_271"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_name = None;
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                trait_name = Some(trait_item.ident.clone());
                break;
            }
        }
        let trait_name = trait_name.unwrap_or_else(|| Ident::new("Trait", Span::call_site()));
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    let new_type = if let Type::ImplTrait(_) = &**return_type {
                        create_for_binder_type(&trait_name)
                    } else if let Type::TraitObject(_) = &**return_type {
                        create_for_binder_type(&trait_name)
                    } else {
                        continue;
                    };
                    *return_type = Box::new(new_type);
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                            let new_type = if let Type::ImplTrait(_) = &**return_type {
                                create_for_binder_type(&trait_name)
                            } else if let Type::TraitObject(_) = &**return_type {
                                create_for_binder_type(&trait_name)
                            } else {
                                continue;
                            };
                            *return_type = Box::new(new_type);
                        }
                    }
                }
            }
        }
        
        if trait_name == "Trait" {
            file.items.push(parse_quote! {
                trait Trait {}
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function signatures with impl Trait or dyn Trait return types. It transforms them into a complex trait object with a for<> binder that introduces a new type parameter T and uses it inside an impl Trait within an associated type binding. This creates nested scoping issues and escaping bound variable problems, stressing the compiler's handling of higher-ranked trait bounds and associated type projections. The operator ensures syntactic validity but semantic oddities, such as missing parentheses or using ':' instead of '->' in some cases, to provoke parsing and well-formedness errors."
    }
}

fn create_for_binder_type(trait_name: &Ident) -> Type {
    let param_ident = Ident::new("T", Span::call_site());
    let lifetime = Lifetime::new("'_", Span::call_site());
    
    let trait_bound = TraitBound {
        paren_token: None,
        modifier: TraitBoundModifier::None,
        lifetimes: Some(BoundLifetimes {
            for_token: token::For::default(),
            lt_token: token::Lt::default(),
            lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                attrs: Vec::new(),
                lifetime: lifetime.clone(),
                colon_token: None,
                bounds: Punctuated::new(),
            })]),
            gt_token: token::Gt::default(),
        }),
        path: SynPath {
            leading_colon: None,
            segments: Punctuated::from_iter(vec![
                syn::PathSegment {
                    ident: trait_name.clone(),
                    arguments: PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                        colon2_token: None,
                        lt_token: token::Lt::default(),
                        args: Punctuated::from_iter(vec![
                            GenericArgument::AssocType(syn::AssocType {
                                ident: Ident::new("Assoc", Span::call_site()),
                                generics: None,
                                eq_token: token::Eq::default(),
                                ty: Type::ImplTrait(TypeImplTrait {
                                    impl_token: token::Impl::default(),
                                    bounds: Punctuated::from_iter(vec![
                                        TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![
                                                    syn::PathSegment {
                                                        ident: trait_name.clone(),
                                                        arguments: PathArguments::AngleBracketed(
                                                            syn::AngleBracketedGenericArguments {
                                                                colon2_token: None,
                                                                lt_token: token::Lt::default(),
                                                                args: Punctuated::from_iter(vec![
                                    GenericArgument::Type(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(param_ident.clone()),
                                    })),
                                ]),
                                                                gt_token: token::Gt::default(),
                                                            },
                                                        ),
                                                    },
                                                ]),
                                            },
                                        }),
                                        TypeParamBound::Lifetime(lifetime),
                                    ]),
                                }),
                            }),
                        ]),
                        gt_token: token::Gt::default(),
                    }),
                },
            ]),
        },
    };
    
    Type::TraitObject(syn::TypeTraitObject {
        dyn_token: Some(token::Dyn::default()),
        bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(trait_bound)]),
    })
}
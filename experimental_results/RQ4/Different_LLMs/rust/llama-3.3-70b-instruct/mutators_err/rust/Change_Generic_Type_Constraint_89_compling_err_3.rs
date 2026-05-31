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

pub struct Change_Generic_Type_Constraint_89;

impl Mutator for Change_Generic_Type_Constraint_89 {
    fn name(&self) -> &str {
        "Change_Generic_Type_Constraint_89"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            for bound in &mut type_param.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let new_trait_bound = TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new(
                                                    &format!("{}Bar", trait_bound.path.segments[0].ident),
                                                    Span::call_site(),
                                                ),
                                                arguments: PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: Punctuated::from_iter(vec![
                                                            GenericArgument::Type(Type::Path(TypePath {
                                                                qself: None,
                                                                path: SynPath {
                                                                    leading_colon: None,
                                                                    segments: Punctuated::from_iter(
                                                                        vec![PathSegment {
                                                                            ident: Ident::new(
                                                                                "usize",
                                                                                Span::call_site(),
                                                                            ),
                                                                            arguments:
                                                                                PathArguments::None,
                                                                        }],
                                                                    ),
                                                                },
                                                            })),
                                                        ]),
                                                        gt_token: token::Gt::default(),
                                                    },
                                                ),
                                            },
                                        ]),
                                    };
                                    *bound = TypeParamBound::Trait(new_trait_bound);
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            for bound in &mut type_param.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let new_trait_bound = TraitBound {
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new(
                                                    &format!("{}Bar", trait_bound.path.segments[0].ident),
                                                    Span::call_site(),
                                                ),
                                                arguments: PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: Punctuated::from_iter(vec![
                                                            GenericArgument::Type(Type::Path(TypePath {
                                                                qself: None,
                                                                path: SynPath {
                                                                    leading_colon: None,
                                                                    segments: Punctuated::from_iter(
                                                                        vec![PathSegment {
                                                                            ident: Ident::new(
                                                                                "usize",
                                                                                Span::call_site(),
                                                                            ),
                                                                            arguments:
                                                                                PathArguments::None,
                                                                        }],
                                                                    ),
                                                                },
                                                            })),
                                                        ]),
                                                        gt_token: token::Gt::default(),
                                                    },
                                                ),
                                            },
                                        ]),
                                    };
                                    *bound = TypeParamBound::Trait(new_trait_bound);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the generic type constraint of a struct or impl by modifying the trait bound to include a new trait. This transformation aims to test the compiler's ability to resolve trait bounds and may lead to ICEs or inference failures in the trait resolution system."
    }
}
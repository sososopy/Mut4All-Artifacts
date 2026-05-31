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

pub struct Replace_Type_Alias_With_Generic_Type_477;

impl Mutator for Replace_Type_Alias_With_Generic_Type_477 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Generic_Type_477"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                if let Type::Alias(type_alias) = &item_type.ty {
                    if let Type::ImplTrait(type_impl_trait) = &*type_alias.ty {
                        let generic_param = GenericParam::Type(Ident::new("T", Span::call_site()));
                        let mut bounds = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            bounds.push(TypeParamBound::Trait(bound.clone()));
                        }
                        let generic_type = Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new("T", Span::call_site()),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        });
                        let new_type = Type::ImplTrait(TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds,
                        });
                        let new_item = Item::Type(ItemType {
                            attrs: item_type.attrs.clone(),
                            vis: item_type.vis.clone(),
                            ident: item_type.ident.clone(),
                            generics: Generics {
                                lt_token: Some(token::Lt::default()),
                                params: Punctuated::from_iter(vec![generic_param]),
                                gt_token: Some(token::Gt::default()),
                                where_clause: Some(WhereClause {
                                    where_token: token::Where::default(),
                                    predicates: Punctuated::from_iter(vec![WherePredicate::Type(
                                        PredicateType {
                                            lifetimes: None,
                                            bounded_ty: generic_type,
                                            colon_token: token::Colon::default(),
                                            bounds: bounds,
                                        },
                                    )]),
                                }),
                            },
                            ty: Box::new(new_type),
                            semi_token: item_type.semi_token.clone(),
                        });
                        *item = new_item;
                    }
                }
            }
            if let Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if let Some(type_alias) = file.items.iter().find(|item| {
                                    if let Item::Type(item_type) = item {
                                        if item_type.ident == segment.ident {
                                            true
                                        } else {
                                            false
                                        }
                                    } else {
                                        false
                                    }
                                }) {
                                    if let Item::Type(item_type) = type_alias {
                                        if let Type::Alias(type_alias) = &item_type.ty {
                                            if let Type::ImplTrait(type_impl_trait) = &*type_alias.ty {
                                                let generic_param = GenericParam::Type(Ident::new("T", Span::call_site()));
                                                let mut bounds = Punctuated::new();
                                                for bound in &type_impl_trait.bounds {
                                                    bounds.push(TypeParamBound::Trait(bound.clone()));
                                                }
                                                let generic_type = Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath {
                                                        leading_colon: None,
                                                        segments: Punctuated::from_iter(vec![PathSegment {
                                                            ident: Ident::new("T", Span::call_site()),
                                                            arguments: PathArguments::None,
                                                        }]),
                                                    },
                                                });
                                                pat_type.ty = Box::new(Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath {
                                                        leading_colon: None,
                                                        segments: Punctuated::from_iter(vec![PathSegment {
                                                            ident: Ident::new("T", Span::call_site()),
                                                            arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                                colon2_token: None,
                                                                lt_token: token::Lt::default(),
                                                                args: Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
                                                                    qself: None,
                                                                    path: SynPath {
                                                                        leading_colon: None,
                                                                        segments: Punctuated::from_iter(vec![PathSegment {
                                                                            ident: Ident::new("i32", Span::call_site()),
                                                                            arguments: PathArguments::None,
                                                                        }]),
                                                                    },
                                                                }))]),
                                                                gt_token: token::Gt::default(),
                                                            }),
                                                        }]),
                                                    },
                                                }));
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
        "The mutation operator replaces type aliases with generic types, using the same trait bounds as the original type alias. This transformation tests the compiler's handling of type inference and trait bounds, and can potentially expose bugs related to the use of type aliases with the impl trait feature."
    }
}
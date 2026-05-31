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

pub struct Replace_Associated_Type_With_Impl_Alias_471;

impl Mutator for Replace_Associated_Type_With_Impl_Alias_471 {
    fn name(&self) -> &str {
        "Replace_Associated_Type_With_Impl_Alias_471"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let has_associated_type = item_impl.items.iter().any(|i| {
                    matches!(i, syn::ImplItem::Type(_))
                });

                if has_associated_type {
                    // Generate DummyT<T> = impl SomeTrait
                    let dummy_t_ident = Ident::new("DummyT", Span::call_site());
                    let dummy_t_generics = parse_quote!(<T>);
                    let dummy_t_type = Type::ImplTrait(TypeImplTrait {
                        impl_token: token::Impl::default(),
                        bounds: {
                            let mut bounds = Punctuated::new();
                            let trait_path = parse_quote!(SomeTrait);
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: trait_path,
                            }));
                            bounds
                        },
                    });

                    let dummy_t_item = syn::Item::Type(syn::ItemType {
                        attrs: Vec::new(),
                        vis: syn::Visibility::Inherited,
                        ident: dummy_t_ident.clone(),
                        generics: dummy_t_generics,
                        ty: Box::new(dummy_t_type),
                    });

                    // Insert DummyT at beginning of file
                    file.items.insert(0, dummy_t_item);

                    // Modify impl block's generics
                    let param_name = item_impl.generics.params.iter()
                        .find_map(|param| {
                            if let syn::GenericParam::Type(type_param) = param {
                                Some(type_param.ident.clone())
                            } else {
                                None
                            }
                        });

                    if let Some(param) = param_name {
                        // Create DummyT<T> type path
                        let dummy_t_type_path = TypePath {
                            qself: None,
                            path: Path {
                                leading_colon: None,
                                segments: {
                                    let mut segs = Punctuated::new();
                                    segs.push(PathSegment {
                                        ident: dummy_t_ident.clone(),
                                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: {
                                                let mut args = Punctuated::new();
                                                args.push(GenericArgument::Type(Type::Ident(TypeIdent {
                                                    ident: param.clone(),
                                                })));
                                                args
                                            },
                                            gt_token: token::Gt::default(),
                                        }),
                                    });
                                    segs
                                },
                            },
                        };

                        // Create SomeTrait<DummyT<T>> trait bound
                        let trait_path = Path {
                            leading_colon: None,
                            segments: {
                                let mut segs = Punctuated::new();
                                segs.push(PathSegment {
                                    ident: Ident::new("SomeTrait", Span::call_site()),
                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt::default(),
                                        args: {
                                            let mut args = Punctuated::new();
                                            args.push(GenericArgument::Type(dummy_t_type_path));
                                            args
                                        },
                                        gt_token: token::Gt::default(),
                                    }),
                                });
                                segs
                            },
                        };

                        let trait_bound = TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: trait_path,
                        };

                        // Create where predicate: T: SomeTrait<DummyT<T>>
                        let bounded_ty = Type::Ident(TypeIdent {
                            ident: param.clone(),
                        });

                        let predicate = WherePredicate::Type(PredicateType {
                            lifetimes: None,
                            bounded_ty,
                            colon_token: token::Colon::default(),
                            bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(trait_bound)]),
                        });

                        // Add to where clause
                        if let Some(where_clause) = &mut item_impl.generics.where_clause {
                            where_clause.predicates.push(predicate);
                        } else {
                            let mut where_clause = WhereClause {
                                where_token: token::Where::default(),
                                predicates: Punctuated::from_iter(vec![predicate]),
                            };
                            item_impl.generics.where_clause = Some(where_clause);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
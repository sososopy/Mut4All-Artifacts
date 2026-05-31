use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBinary, ExprCall, ExprLit, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Generic_Const_Trait_Bound_408;

impl Mutator for Add_Generic_Const_Trait_Bound_408 {
    fn name(&self) -> &str {
        "Add_Generic_Const_Trait_Bound_408"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Check for struct `a` and trait `AnotherTrait`
        let has_a = file.items.iter().any(|item| {
            if let Item::Struct(s) = item {
                s.ident == "a"
            } else {
                false
            }
        });
        let has_trait = file.items.iter().any(|item| {
            if let Item::Trait(t) = item {
                t.ident == "AnotherTrait"
            } else {
                false
            }
        });

        if !has_a {
            let new_struct = parse_quote! {
                struct a<const B: bool>;
            };
            file.items.insert(0, Item::Struct(new_struct));
        }
        if !has_trait {
            let new_trait = parse_quote! {
                trait AnotherTrait {}
            };
            file.items.insert(0, Item::Trait(new_trait));
        }

        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                let has_const = impl_item.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Const(_))
                });

                if has_const {
                    let const_params: Vec<&syn::ConstParam> = impl_item
                        .generics
                        .params
                        .iter()
                        .filter_map(|param| match param {
                            GenericParam::Const(c) => Some(c),
                            _ => None,
                        })
                        .collect();

                    if let Some(const_param) = const_params.first() {
                        // Build expression: N > 5
                        let expr = ExprBinary {
                            attrs: vec![],
                            left: Box::new(Expr::Path(ExprPath {
                                attrs: vec![],
                                qself: None,
                                path: Path::from(const_param.ident.clone()),
                            })),
                            op: token::Gt::default(),
                            right: Box::new(Expr::Lit(ExprLit {
                                attrs: vec![],
                                lit: Lit::Int(syn::LitInt::new("5", Span::call_site())),
                            })),
                        };

                        // Build bounded_ty: a<{ N > 5 }>
                        let generic_args = AngleBracketedGenericArguments {
                            colon2_token: None,
                            lt_token: token::Lt::default(),
                            args: Punctuated::from_iter(vec![GenericArgument::Expr(expr)]),
                            gt_token: token::Gt::default(),
                        };
                        let path = Path {
                            leading_colon: None,
                            segments: Punctuated::from_iter(vec![PathSegment {
                                ident: Ident::new("a", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(generic_args),
                            }]),
                        };
                        let bounded_ty = Type::Path(TypePath {
                            qself: None,
                            path,
                        });

                        // Build trait bound: AnotherTrait
                        let trait_bound = TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: Path::from(Ident::new("AnotherTrait", Span::call_site())),
                        };

                        // Build predicate
                        let predicate = WherePredicate::Type(PredicateType {
                            lifetimes: None,
                            bounded_ty: Box::new(bounded_ty),
                            colon_token: token::Colon::default(),
                            bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(trait_bound)]),
                        });

                        // Add to where clause
                        if let Some(where_clause) = &mut impl_item.generics.where_clause {
                            where_clause.predicates.push(predicate);
                        } else {
                            let mut where_clause = WhereClause {
                                where_token: token::Where::default(),
                                predicates: Punctuated::from_iter(vec![predicate]),
                            };
                            impl_item.generics.where_clause = Some(where_clause);
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
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

pub struct Replace_Lifetime_Binder_With_Const_Binder_438;

impl Mutator for Replace_Lifetime_Binder_With_Const_Binder_438 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Binder_With_Const_Binder_438"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    let mut new_predicates = Punctuated::new();
                    let mut found = false;
                    for pred in where_clause.predicates.iter_mut() {
                        if let syn::WherePredicate::Lifetime(_) = pred {
                            if !found {
                                let bound_lifetimes = BoundLifetimes {
                                    for_token: token::For::default(),
                                    lt_token: token::Lt::default(),
                                    lifetimes: {
                                        let mut params = Punctuated::new();
                                        let const_param = syn::ConstParam {
                                            attrs: Vec::new(),
                                            const_token: token::Const::default(),
                                            ident: Ident::new("C", Span::call_site()),
                                            colon_token: token::Colon::default(),
                                            default: None,
                                            eq_token: None,
                                            ty: parse_quote!(u8),
                                        };
                                        params.push(GenericParam::Const(const_param));
                                        params
                                    },
                                    gt_token: token::Gt::default(),
                                };
                                let bounded_ty = Type::Array(syn::TypeArray {
                                    bracket_token: token::Bracket::default(),
                                    elem: Box::new(Type::Tuple(syn::TypeTuple {
                                        paren_token: token::Paren::default(),
                                        elems: Punctuated::new(),
                                    })),
                                    len: Expr::Path(syn::ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: SynPath::from(Ident::new("C", Span::call_site())),
                                    }),
                                    semi_token: token::Semi::default(),
                                });
                                let colon_token = token::Colon::default();
                                let mut bounds = Punctuated::new();
                                bounds.push(TypeParamBound::Trait(TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath::from(Ident::new("Sized", Span::call_site())),
                                }));
                                let new_pred = syn::WherePredicate::Type(syn::PredicateType {
                                    lifetimes: Some(bound_lifetimes),
                                    bounded_ty,
                                    colon_token,
                                    bounds,
                                });
                                new_predicates.push(new_pred);
                                found = true;
                            } else {
                                new_predicates.push(pred.clone());
                            }
                        } else {
                            new_predicates.push(pred.clone());
                        }
                    }
                    where_clause.predicates = new_predicates;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
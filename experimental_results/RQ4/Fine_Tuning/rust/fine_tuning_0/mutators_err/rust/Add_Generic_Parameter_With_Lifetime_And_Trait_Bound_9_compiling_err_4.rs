use proc_macro2::{Span};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Add_Generic_Parameter_With_Lifetime_And_Trait_Bound_9;

impl Mutator for Add_Generic_Parameter_With_Lifetime_And_Trait_Bound_9 {
    fn name(&self) -> &str {
        "Add_Generic_Parameter_With_Lifetime_And_Trait_Bound_9"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.generics.params.is_empty() {
                    let lifetime = Lifetime::new("'a", Span::call_site());
                    let generic_param: GenericParam = parse_quote!(T_1);
                    let lifetime_param: LifetimeParam = parse_quote!('a);
                    func.sig.generics.params.push(generic_param);
                    func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                    
                    let trait_bound: TraitBound = parse_quote!(for<'a> T_1: 'a + Sized);
                    func.sig.generics.where_clause.get_or_insert_with(|| parse_quote!(where))
                        .predicates.push(syn::WherePredicate::Type(syn::PredicateType {
                            lifetimes: Some(BoundLifetimes {
                                for_token: token::For {
                                    span: Span::call_site(),
                                },
                                lt_token: token::Lt {
                                    spans: [Span::call_site()],
                                },
                                lifetimes: Punctuated::from_iter(vec![lifetime_param.clone()]),
                                gt_token: token::Gt {
                                    spans: [Span::call_site()],
                                },
                            }),
                            bounded_ty: parse_quote!(T_1),
                            colon_token: token::Colon {
                                spans: [Span::call_site()],
                            },
                            bounds: Punctuated::from_iter(vec![
                                TypeParamBound::Lifetime(lifetime),
                                TypeParamBound::Trait(trait_bound),
                            ]),
                        }));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.generics.params.is_empty() {
                            let lifetime = Lifetime::new("'a", Span::call_site());
                            let generic_param: GenericParam = parse_quote!(T_1);
                            let lifetime_param: LifetimeParam = parse_quote!('a);
                            func.sig.generics.params.push(generic_param);
                            func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                            
                            let trait_bound: TraitBound = parse_quote!(for<'a> T_1: 'a + Sized);
                            func.sig.generics.where_clause.get_or_insert_with(|| parse_quote!(where))
                                .predicates.push(syn::WherePredicate::Type(syn::PredicateType {
                                    lifetimes: Some(BoundLifetimes {
                                        for_token: token::For {
                                            span: Span::call_site(),
                                        },
                                        lt_token: token::Lt {
                                            spans: [Span::call_site()],
                                        },
                                        lifetimes: Punctuated::from_iter(vec![lifetime_param.clone()]),
                                        gt_token: token::Gt {
                                            spans: [Span::call_site()],
                                        },
                                    }),
                                    bounded_ty: parse_quote!(T_1),
                                    colon_token: token::Colon {
                                        spans: [Span::call_site()],
                                    },
                                    bounds: Punctuated::from_iter(vec![
                                        TypeParamBound::Lifetime(lifetime),
                                        TypeParamBound::Trait(trait_bound),
                                    ]),
                                }));
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
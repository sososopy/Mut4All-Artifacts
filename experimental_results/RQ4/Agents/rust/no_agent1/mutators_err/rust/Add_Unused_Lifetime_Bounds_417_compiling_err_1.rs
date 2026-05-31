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

pub struct Add_Unused_Lifetime_Bounds_417;

impl Mutator for Add_Unused_Lifetime_Bounds_417 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Bounds_417"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_ident = Ident::new("'unused", Span::call_site());
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'unused", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates.push(parse_quote! { for<'unused> T: 'unused });
                } else {
                    func.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(parse_quote! { for<'unused> T: 'unused });
                            predicates
                        },
                    });
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let lifetime_ident = Ident::new("'unused", Span::call_site());
                        let lifetime_param = LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'unused", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        };
                        method.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                        
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            where_clause.predicates.push(parse_quote! { for<'unused> T: 'unused });
                        } else {
                            method.sig.generics.where_clause = Some(WhereClause {
                                where_token: Default::default(),
                                predicates: {
                                    let mut predicates = Punctuated::new();
                                    predicates.push(parse_quote! { for<'unused> T: 'unused });
                                    predicates
                                },
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces unused lifetime parameters and bounds into function and method signatures. By adding these redundant constraints, it aims to test the compiler's ability to handle extraneous lifetime information, potentially leading to ICEs or issues in the lifetime resolution system."
    }
}
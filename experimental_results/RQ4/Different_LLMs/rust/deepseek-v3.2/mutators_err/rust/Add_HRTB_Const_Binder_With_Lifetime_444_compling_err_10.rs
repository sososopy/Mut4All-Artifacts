use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_HRTB_Const_Binder_With_Lifetime_444;

impl Mutator for Add_HRTB_Const_Binder_With_Lifetime_444 {
    fn name(&self) -> &str {
        "Add_HRTB_Const_Binder_With_Lifetime_444"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                let lifetime_params: Vec<&LifetimeParam> = item_fn.sig.generics.params.iter()
                    .filter_map(|param| {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            Some(lifetime_param)
                        } else {
                            None
                        }
                    })
                    .collect();
                if lifetime_params.is_empty() {
                    continue;
                }
                let chosen_lifetime = &lifetime_params[0].lifetime;
                let new_predicate: WherePredicate = parse_quote! {
                    for<const C: usize> [(); C]: #chosen_lifetime
                };
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    where_clause.predicates.push(new_predicate);
                } else {
                    item_fn.sig.generics.where_clause = Some(parse_quote! {
                        where for<const C: usize> [(); C]: #chosen_lifetime
                    });
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        let lifetime_params: Vec<&LifetimeParam> = method.sig.generics.params.iter()
                            .filter_map(|param| {
                                if let GenericParam::Lifetime(lifetime_param) = param {
                                    Some(lifetime_param)
                                } else {
                                    None
                                }
                            })
                            .collect();
                        if lifetime_params.is_empty() {
                            continue;
                        }
                        let chosen_lifetime = &lifetime_params[0].lifetime;
                        let new_predicate: WherePredicate = parse_quote! {
                            for<const C: usize> [(); C]: #chosen_lifetime
                        };
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            where_clause.predicates.push(new_predicate);
                        } else {
                            method.sig.generics.where_clause = Some(parse_quote! {
                                where for<const C: usize> [(); C]: #chosen_lifetime
                            });
                        }
                    }
                }
            }
        }
        let mut found_feature = false;
        for attr in &file.attrs {
            if let Meta::NameValue(name_value) = &attr.meta {
                if let Expr::Lit(expr_lit) = &name_value.value {
                    if let Lit::Str(lit_str) = &expr_lit.lit {
                        if name_value.path.is_ident("feature") && lit_str.value() == "non_lifetime_binders" {
                            found_feature = true;
                            break;
                        }
                    }
                }
            }
        }
        if !found_feature {
            let feature_attr: Attribute = parse_quote! {
                #![feature(non_lifetime_binders)]
            };
            file.attrs.insert(0, feature_attr);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions and methods with lifetime parameters. It adds a where-clause predicate containing a higher-ranked const binder: `for<const C: usize> [(); C]: 'a`, where 'a is an existing lifetime parameter. This transformation directly triggers the incomplete feature `non_lifetime_binders` and may cause an internal compiler error (ICE) during well-formedness checking, as bound const variables escape their scope. The mutation also ensures the crate-level feature attribute is present."
    }
}
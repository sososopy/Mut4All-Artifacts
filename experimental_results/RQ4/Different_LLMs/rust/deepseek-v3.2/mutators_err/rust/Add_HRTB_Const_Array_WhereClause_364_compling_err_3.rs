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

pub struct Add_HRTB_Const_Array_WhereClause_364;

impl Mutator for Add_HRTB_Const_Array_WhereClause_364 {
    fn name(&self) -> &str {
        "Add_HRTB_Const_Array_WhereClause_364"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut used_lifetimes = HashSet::new();
        let mut array_types = Vec::new();

        let mut visitor = CollectLifetimesAndArrayTypes {
            lifetimes: &mut used_lifetimes,
            array_types: &mut array_types,
        };
        visitor.visit_file_mut(file);

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }

                let fresh_lifetime = generate_fresh_lifetime(&used_lifetimes, &mut rng);
                used_lifetimes.insert(fresh_lifetime.to_string());

                let array_type = if array_types.is_empty() {
                    parse_quote!([(); 0])
                } else {
                    array_types.choose(&mut rng).unwrap().clone()
                };

                let new_predicate: WherePredicate = parse_quote! {
                    for<#fresh_lifetime> #array_type: 
                };

                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    where_clause.predicates.push(new_predicate);
                } else {
                    item_fn.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(new_predicate);
                            predicates
                        },
                    });
                }
            }
        }

        let mut found_feature = false;
        for attr in &file.attrs {
            if let Meta::List(list) = &attr.meta {
                if list.path.is_ident("feature") {
                    for nested in list.tokens.clone().into_iter() {
                        if let proc_macro2::TokenTree::Ident(ident) = nested {
                            if ident.to_string() == "generic_const_exprs" {
                                found_feature = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if !found_feature {
            let feature_attr: Attribute = parse_quote! {
                #![feature(generic_const_exprs)]
            };
            file.attrs.insert(0, feature_attr);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds or extends where clauses with a higher-ranked trait bound (HRTB) involving a const generic array type. It introduces a fresh lifetime 'b within a for<'b> quantifier and uses it inside a const expression block that declares a local variable x: &'b (). The array length is set to 0. This transformation stresses the compiler's handling of HRTBs combined with generic const expressions, lifetime interactions within const contexts, and where clause predicate resolution, potentially triggering edge cases in const evaluation and lifetime validation."
    }
}

struct CollectLifetimesAndArrayTypes<'a> {
    lifetimes: &'a mut HashSet<String>,
    array_types: &'a mut Vec<Type>,
}

impl<'a> VisitMut for CollectLifetimesAndArrayTypes<'a> {
    fn visit_lifetime_mut(&mut self, lifetime: &mut Lifetime) {
        self.lifetimes.insert(lifetime.ident.to_string());
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(type_array) = ty {
            self.array_types.push(Type::Array(type_array.clone()));
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}

fn generate_fresh_lifetime(used: &HashSet<String>, rng: &mut impl Rng) -> Ident {
    let mut candidate = String::new();
    loop {
        candidate.clear();
        candidate.push('\'');
        candidate.push((b'a' + rng.gen_range(0..26)) as char);
        if rng.gen_bool(0.5) {
            candidate.push((b'a' + rng.gen_range(0..26)) as char);
        }
        if !used.contains(&candidate) {
            return Ident::new(&candidate[1..], Span::call_site());
        }
    }
}
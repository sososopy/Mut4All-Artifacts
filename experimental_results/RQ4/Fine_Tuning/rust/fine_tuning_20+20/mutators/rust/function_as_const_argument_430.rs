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

pub struct Function_As_Const_Argument_430;

impl Mutator for Function_As_Const_Argument_430 {
    fn name(&self) -> &str {
        "Function_As_Const_Argument_430"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut function_names = Vec::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                function_names.push(item_fn.sig.ident.clone());
            }
        }
        let mut rng = rand::thread_rng();
        let selected_function = function_names.choose(&mut rng);
        if selected_function.is_none() {
            return;
        }
        let selected_function = selected_function.unwrap();
        let mut generic_targets = Vec::new();
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Struct(item_struct) = item {
                if item_struct.generics.params.is_empty() {
                    continue;
                }
                generic_targets.push(index);
            }
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.generics.params.is_empty() {
                    continue;
                }
                generic_targets.push(index);
            }
        }
        let selected_target = generic_targets.choose(&mut rng);
        if selected_target.is_none() {
            return;
        }
        let selected_target = *selected_target.unwrap();
        if let Item::Struct(item_struct) = &mut file.items[selected_target] {
            let constraint: WherePredicate = parse_quote! {
                [(); #selected_function()]:
            };
            if let Some(where_clause) = &mut item_struct.generics.where_clause {
                where_clause.predicates.push(constraint);
            } else {
                item_struct.generics.where_clause = Some(WhereClause {
                    where_token: Default::default(),
                    predicates: {
                        let mut predicates = Punctuated::new();
                        predicates.push(constraint);
                        predicates
                    },
                });
            }
        }
        if let Item::Fn(item_fn) = &mut file.items[selected_target] {
            let constraint: WherePredicate = parse_quote! {
                [(); #selected_function()]:
            };
            if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                where_clause.predicates.push(constraint);
            } else {
                item_fn.sig.generics.where_clause = Some(WhereClause {
                    where_token: Default::default(),
                    predicates: {
                        let mut predicates = Punctuated::new();
                        predicates.push(constraint);
                        predicates
                    },
                });
            }
        }
        let mut has_feature = false;
        for attr in &file.attrs {
            if attr.path().is_ident("feature") {
                let _ = attr.parse_nested_meta(|meta| {
                    if meta.path.to_token_stream().to_string().contains("min_generic_const_args") {
                        has_feature = true;
                    }
                    Ok(())
                });
            }
        }
        if !has_feature {
            let feature_attr: Attribute = parse_quote! {
                #![feature(min_generic_const_args)]
            };
            file.attrs.insert(0, feature_attr);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions and structs with generic parameters, injecting a constraint that uses a randomly selected function as a const argument. It ensures the presence of the `min_generic_const_args` feature, aiming to expose compiler bugs related to const evaluation and generic parameter constraints."
    }
}
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

pub struct Add_Non_Lifetime_Binders_To_Closure_Functions_440;

impl Mutator for Add_Non_Lifetime_Binders_To_Closure_Functions_440 {
    fn name(&self) -> &str {
        "Add_Non_Lifetime_Binders_To_Closure_Functions_440"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_feature = false;
        for attr in &file.attrs {
            if let syn::AttrStyle::Inner = attr.style {
                if let syn::Meta::List(list) = &attr.meta {
                    if list.path.is_ident("feature") {
                        if let Ok(nested) = list.parse_args::<syn::Meta>() {
                            if nested.path().is_ident("non_lifetime_binders") {
                                has_feature = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        
        if !has_feature {
            let feature_attr = parse_quote! {
                #![feature(non_lifetime_binders)]
            };
            file.attrs.insert(0, feature_attr);
        }

        let mut closure_functions = Vec::new();
        let mut visitor = ClosureFunctionVisitor {
            functions: &mut closure_functions,
        };
        visitor.visit_file(file);

        let mut rng = thread_rng();
        for func_idx in closure_functions {
            if let Item::Fn(func) = &mut file.items[func_idx] {
                if func.sig.ident == "main" {
                    continue;
                }

                let mut has_closure_param = false;
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                                    has_closure_param = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                if !has_closure_param {
                    continue;
                }

                let existing_params: HashSet<String> = func.sig.generics.params.iter()
                    .filter_map(|param| {
                        if let GenericParam::Type(type_param) = param {
                            Some(type_param.ident.to_string())
                        } else {
                            None
                        }
                    })
                    .collect();

                let mut new_param_name = String::new();
                for c in b'A'..=b'Z' {
                    let candidate = format!("T{}", c as char);
                    if !existing_params.contains(&candidate) {
                        new_param_name = candidate;
                        break;
                    }
                }
                if new_param_name.is_empty() {
                    for i in 0..100 {
                        let candidate = format!("T{}", i);
                        if !existing_params.contains(&candidate) {
                            new_param_name = candidate;
                            break;
                        }
                    }
                }
                if new_param_name.is_empty() {
                    continue;
                }

                let new_param_ident = Ident::new(&new_param_name, Span::call_site());
                
                let mut new_where_clause = func.sig.generics.where_clause.take().map(|wc| wc.predicates).unwrap_or_else(Punctuated::new);
                
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                                    let hrtb_predicate: WherePredicate = parse_quote! {
                                        for<#new_param_ident> #pat_type: #type_path
                                    };
                                    new_where_clause.push(hrtb_predicate);
                                }
                            }
                        }
                    }
                }

                if !new_where_clause.is_empty() {
                    func.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: new_where_clause,
                    });
                }
            }
        }

        let mut new_functions = Vec::new();
        let mut visitor = GenericClosureVisitor {
            closures: &mut new_functions,
        };
        visitor.visit_file(file);

        if new_functions.is_empty() {
            let generic_closure_func: ItemFn = parse_quote! {
                fn generic_closure<T>(x: T) -> T {
                    let closure = |x: T| x;
                    closure(x)
                }
            };
            file.items.push(Item::Fn(generic_closure_func));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds higher-ranked trait bounds with non-lifetime parameters (for<T>) to functions that accept closure arguments. It ensures the non_lifetime_binders feature is enabled, identifies functions with Fn/FnMut/FnOnce parameters, and modifies their where clauses to include HRTB constraints. If no suitable closures exist, it creates a generic closure function to provide test material. This transformation targets the compiler's handling of early-bound vs late-bound parameters in HRTB contexts, potentially triggering ICEs related to F-non_lifetime_binders bugs."
    }
}

struct ClosureFunctionVisitor<'a> {
    functions: &'a mut Vec<usize>,
}

impl<'a> Visit<'a> for ClosureFunctionVisitor<'a> {
    fn visit_item_fn(&mut self, i: &'a ItemFn) {
        for (idx, input) in i.sig.inputs.iter().enumerate() {
            if let FnArg::Typed(pat_type) = input {
                if let Type::Path(type_path) = &*pat_type.ty {
                    if let Some(segment) = type_path.path.segments.last() {
                        if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                            self.functions.push(idx);
                        }
                    }
                }
            }
        }
    }
}

struct GenericClosureVisitor<'a> {
    closures: &'a mut Vec<ItemFn>,
}

impl<'a> Visit<'a> for GenericClosureVisitor<'a> {
    fn visit_expr_closure(&mut self, i: &'a ExprClosure) {
        if i.inputs.is_empty() {
            let func: ItemFn = parse_quote! {
                fn dummy() {
                    let _ = || {};
                }
            };
            self.closures.push(func);
        }
    }
}
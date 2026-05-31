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

pub struct Replace_Const_Generic_Expression_With_Complex_Arithmetic_47;

impl Mutator for Replace_Const_Generic_Expression_With_Complex_Arithmetic_47 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Expression_With_Complex_Arithmetic_47"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_generic_const_exprs = false;
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            if let Type::Array(arr) = &pred_type.bounded_ty {
                                if let Expr::Path(expr_path) = &arr.len {
                                    let new_expr: Expr = parse_quote! { ( { { #expr_path } } ) };
                                    arr.len = new_expr;
                                }
                            }
                        }
                    }
                }
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            if let Expr::Path(expr_path) = &*default {
                                let new_expr: Expr = parse_1quote! { ( #expr_path - 1 ) + 1 + #expr_path };
                                *default = Box::new(new_expr);
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(pred_type) = predicate {
                                    if let Type::Array(arr) = &pred_type.bounded_ty {
                                        if let Expr::Path(expr_path) = &arr.len {
                                            let new_expr: Expr = parse_quote! { ( { { #expr_path } } ) };
                                            arr.len = new_expr;
                                        }
                                    }
                                }
                            }
                        }
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if let Some(default) = &mut const_param.default {
                                    if let Expr::Path(expr_path) = &*default {
                                        let new_expr: Expr = parse_quote! { ( #expr_path - 1 ) + 1 + #expr_path };
                                        *default = new_expr;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Struct(item_struct) = item {
                if let Some(where_clause) = &mut item_struct.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            if let Type::Array(arr) = &pred_type.bounded_ty {
                                if let Expr::Path(expr_path) = &arr.len {
                                    let new_expr: Expr = parse_quote! { ( { { #expr_path } } ) };
                                    arr.len = Box::new(new_expr);
                                }
                            }
                        }
                    }
                }
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            if let Expr::Path(expr_path) = &*default {
                                let new_expr: Expr = parse_quote! { ( #expr_path - 1 ) + 1 + #expr_path };
                                *default = new_expr;
                            }
                        }
                    }
                }
            }
        }
        if !has_generic_const_exprs {
            let mut visitor = FeatureGateVisitor { found: false };
            visitor.visit_file(&file);
            if visitor.found {
                has_generic_const_exprs = true;
            }
        }
        if !has_generic_const_exprs {
            file.items.insert(0, parse_quote! {
                #![feature(generic_const_exprs)]
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant generic expressions in const generic arguments and where-clause bounds with more complex expressions that introduce redundant arithmetic operations and nested blocks. It targets expressions involving const generic parameters, transforming them into forms like `( { { N } } )` or `( ( N - 1 ) + 1 + N )`. This stresses the const evaluator with additional syntactic nesting and redundant operations, potentially exposing ICEs in const evaluation during trait selection. If the seed doesn't already use the generic_const_exprs feature, the operator adds the feature gate to the crate root."
    }
}

struct FeatureGateVisitor {
    found: bool,
}

impl Visit<'_> for FeatureGateVisitor {
    fn visit_attribute(&mut self, attr: &syn::Attribute) {
        if attr.path().is_ident("feature") {
            if let Ok(meta) = attr.parse_args::<syn::Meta>() {
                if let syn::Meta::List(list) = meta {
                    for nested in &list.nested {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested {
                            if path.is_ident("generic_const_exprs") {
                                self.found = true;
                            }
                        }
                    }
                }
            }
        }
    }
}
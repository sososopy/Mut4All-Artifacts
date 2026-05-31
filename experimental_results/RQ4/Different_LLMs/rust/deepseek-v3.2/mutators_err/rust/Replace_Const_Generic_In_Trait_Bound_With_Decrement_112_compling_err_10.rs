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

pub struct Replace_Const_Generic_In_Trait_Bound_With_Decrement_112;

impl Mutator for Replace_Const_Generic_In_Trait_Bound_With_Decrement_112 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_In_Trait_Bound_With_Decrement_112"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstGenericVisitor<'a> {
            const_params: &'a HashSet<String>,
        }

        impl<'a> VisitMut for ConstGenericVisitor<'a> {
            fn visit_type_mut(&mut self, node: &mut Type) {
                if let Type::Array(arr) = node {
                    if let Type::Path(path) = &*arr.elem {
                        if path.path.segments.len() == 1 {
                            let ident = &path.path.segments[0].ident;
                            if self.const_params.contains(&ident.to_string()) {
                                if let Expr::Lit(expr_lit) = &arr.len {
                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                        let current_val = lit_int.base10_parse::<u64>().unwrap_or(0);
                                        let new_val = current_val.saturating_sub(1);
                                        arr.len = syn::Expr::Lit(syn::ExprLit {
                                            attrs: Vec::new(),
                                            lit: syn::Lit::Int(syn::LitInt::new(&new_val.to_string(), lit_int.span())),
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_type_mut(self, node);
            }

            fn visit_where_predicate_mut(&mut self, node: &mut syn::WherePredicate) {
                if let syn::WherePredicate::Type(pred_type) = node {
                    if let Type::Array(arr) = &mut pred_type.bounded_ty {
                        if let Type::Path(path) = &*arr.elem {
                            if path.path.segments.len() == 1 {
                                let ident = &path.path.segments[0].ident;
                                if self.const_params.contains(&ident.to_string()) {
                                    if let Expr::Lit(expr_lit) = &arr.len {
                                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                            let current_val = lit_int.base10_parse::<u64>().unwrap_or(0);
                                            let new_val = current_val.saturating_sub(1);
                                            arr.len = syn::Expr::Lit(syn::ExprLit {
                                                attrs: Vec::new(),
                                                lit: syn::Lit::Int(syn::LitInt::new(&new_val.to_string(), lit_int.span())),
                                            });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_where_predicate_mut(self, node);
            }
        }

        let mut const_params = HashSet::new();
        for item in &mut file.items {
            match item {
                Item::Fn(func) => {
                    for param in &func.sig.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            const_params.insert(const_param.ident.to_string());
                        }
                    }
                }
                Item::Impl(impl_item) => {
                    for param in &impl_item.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            const_params.insert(const_param.ident.to_string());
                        }
                    }
                }
                Item::Struct(struct_item) => {
                    for param in &struct_item.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            const_params.insert(const_param.ident.to_string());
                        }
                    }
                }
                _ => {}
            }
        }

        let mut visitor = ConstGenericVisitor {
            const_params: &const_params,
        };
        syn::visit_mut::visit_file_mut(&mut visitor, file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters in trait bounds and type constraints, replacing their usage with decremented expressions (N - 1). It specifically modifies array length expressions in where clauses and type contexts, aiming to trigger underflow during const evaluation in trait resolution. This transformation tests the compiler's overflow handling and const evaluation robustness in generic contexts."
    }
}
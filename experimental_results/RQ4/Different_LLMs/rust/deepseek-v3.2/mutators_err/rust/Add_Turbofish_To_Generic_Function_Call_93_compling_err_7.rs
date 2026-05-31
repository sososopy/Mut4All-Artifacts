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

pub struct Add_Turbofish_To_Generic_Function_Call_93;

impl Mutator for Add_Turbofish_To_Generic_Function_Call_93 {
    fn name(&self) -> &str {
        "Add_Turbofish_To_Generic_Function_Call_93"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct Visitor<'a> {
            generics: Vec<&'a syn::Generics>,
        }
        impl<'a> Visit<'a> for Visitor<'a> {
            fn visit_item_fn(&mut self, i: &'a ItemFn) {
                self.generics.push(&i.sig.generics);
                syn::visit::visit_item_fn(self, i);
                self.generics.pop();
            }
            fn visit_item_impl(&mut self, i: &'a syn::ItemImpl) {
                self.generics.push(&i.generics);
                syn::visit::visit_item_impl(self, i);
                self.generics.pop();
            }
            fn visit_impl_item_fn(&mut self, i: &'a syn::ImplItemFn) {
                self.generics.push(&i.sig.generics);
                syn::visit::visit_impl_item_fn(self, i);
                self.generics.pop();
            }
        }
        let mut visitor = Visitor { generics: Vec::new() };
        visitor.visit_file(file);
        let mut visitor_mut = TurbofishVisitor {
            generics_stack: Vec::new(),
        };
        visitor_mut.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic function calls within generic contexts where a where clause bound involves an associated constant expression like [(); T::CONST]: Sized. It replaces implicit generic argument inference with explicit turbofish syntax (e.g., function_name::<T>()). This transformation alters the inference context and may expose bugs in const evaluation during trait selection, particularly when the turbofish forces explicit type substitution that interacts with associated constant bounds."
    }
}

struct TurbofishVisitor {
    generics_stack: Vec<syn::Generics>,
}

impl VisitMut for TurbofishVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        self.generics_stack.push(i.sig.generics.clone());
        syn::visit_mut::visit_item_fn_mut(self, i);
        self.generics_stack.pop();
    }
    fn visit_item_impl_mut(&mut self, i: &mut syn::ItemImpl) {
        self.generics_stack.push(i.generics.clone());
        syn::visit_mut::visit_item_impl_mut(self, i);
        self.generics_stack.pop();
    }
    fn visit_impl_item_fn_mut(&mut self, i: &mut syn::ImplItemFn) {
        self.generics_stack.push(i.sig.generics.clone());
        syn::visit_mut::visit_impl_item_fn_mut(self, i);
        self.generics_stack.pop();
    }
    fn visit_expr_call_mut(&mut self, expr: &mut syn::ExprCall) {
        if let syn::Expr::Path(path_expr) = &*expr.func {
            if let Some(current_generics) = self.generics_stack.last() {
                if !current_generics.params.is_empty() {
                    let where_clause = current_generics.where_clause.as_ref();
                    let mut target_param = None;
                    for param in &current_generics.params {
                        if let syn::GenericParam::Type(type_param) = param {
                            if let Some(where_clause) = where_clause {
                                for predicate in &where_clause.predicates {
                                    if let syn::WherePredicate::Type(type_predicate) = predicate {
                                        if let syn::Type::Array(array_type) = &type_predicate.bounded_ty {
                                            if let syn::Type::Path(type_path) = &*array_type.elem {
                                                if let Some(segment) = type_path.path.segments.last() {
                                                    if segment.ident == "Sized" {
                                                        if let syn::Expr::Const(const_expr) = &array_type.len {
                                                            if let syn::Expr::Path(const_path) = &const_expr.expr {
                                                                if const_path.path.segments.len() > 1 {
                                    let last_segment = const_path.path.segments.last().unwrap();
                                    if last_segment.ident == "CONST" {
                                        let first_segment = const_path.path.segments.first().unwrap();
                                        if first_segment.ident == type_param.ident {
                                            target_param = Some(type_param.ident.clone());
                                        }
                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if let Some(param_ident) = target_param {
                        let path_segments = &path_expr.path.segments;
                        if path_segments.len() == 1 {
                            let segment = &path_segments[0];
                            if segment.arguments.is_none() {
                                let new_segment = syn::PathSegment {
                                    ident: segment.ident.clone(),
                                    arguments: syn::PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: syn::token::Lt::default(),
                                            args: syn::punctuated::Punctuated::from_iter(vec![
                                                syn::GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                                    qself: None,
                                                    path: syn::Path::from(param_ident),
                                                })),
                                            ]),
                                            gt_token: syn::token::Gt::default(),
                                        },
                                    ),
                                };
                                let new_path = syn::ExprPath {
                                    attrs: path_expr.attrs.clone(),
                                    qself: None,
                                   1. path: syn::Path {
                                        leading_colon: None,
                                        segments: syn::punctuated::Punctuated::from_iter(vec![new_segment]),
                                    },
                                };
                                *expr.func = syn::Expr::Path(new_path);
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, expr);
    }
}
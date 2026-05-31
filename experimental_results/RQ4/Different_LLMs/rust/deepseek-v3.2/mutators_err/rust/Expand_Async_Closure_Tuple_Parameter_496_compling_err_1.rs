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

pub struct Expand_Async_Closure_Tuple_Parameter_496;

impl Mutator for Expand_Async_Closure_Tuple_Parameter_496 {
    fn name(&self) -> &str {
        "Expand_Async_Closure_Tuple_Parameter_496"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ExpandAsyncClosureVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies async closures with a single tuple parameter and expands it into multiple individual parameters. It updates the closure body to use the new parameter names and adjusts any associated trait bounds. This transformation tests the compiler's handling of async closure parameter destructuring, trait bound adaptation, and unboxed closure feature interactions."
    }
}

struct ExpandAsyncClosureVisitor;

impl ExpandAsyncClosureVisitor {
    fn new() -> Self {
        Self
    }
}

impl VisitMut for ExpandAsyncClosureVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            if closure.asyncness.is_some() {
                if closure.inputs.len() == 1 {
                    if let FnArg::Typed(PatType { pat, ty, .. }) = &mut closure.inputs[0] {
                        if let Type::Tuple(tuple_type) = &**ty {
                            if tuple_type.elems.len() >= 2 {
                                let mut new_params = Punctuated::new();
                                let mut param_names = Vec::new();
                                let mut param_types = Vec::new();
                                
                                for (i, elem_type) in tuple_type.elems.iter().enumerate() {
                                    let param_name = if let Pat::Ident(pat_ident) = &**pat {
                                        let base_name = pat_ident.ident.to_string();
                                        let new_name = if i == 0 {
                                            base_name.clone()
                                        } else {
                                            format!("{}{}", base_name, i + 1)
                                        };
                                        Ident::new(&new_name, pat_ident.ident.span())
                                    } else {
                                        Ident::new(&format!("arg{}", i + 1), Span::call_site())
                                    };
                                    
                                    param_names.push(param_name.clone());
                                    param_types.push(elem_type.clone());
                                    
                                    new_params.push(FnArg::Typed(PatType {
                                        attrs: Vec::new(),
                                        pat: Box::new(Pat::Ident(PatIdent {
                                            attrs: Vec::new(),
                                            by_ref: None,
                                            mutability: None,
                                            ident: param_name,
                                            subpat: None,
                                        })),
                                        colon_token: token::Colon::default(),
                                        ty: Box::new(elem_type.clone()),
                                    }));
                                }
                                
                                closure.inputs = new_params;
                                
                                let mut body_visitor = ClosureBodyRewriter {
                                    old_param_name: if let Pat::Ident(pat_ident) = &**pat {
                                        pat_ident.ident.clone()
                                    } else {
                                        Ident::new("x", Span::call_site())
                                    },
                                    new_param_names: param_names,
                                    tuple_size: tuple_type.elems.len(),
                                };
                                body_visitor.visit_expr_mut(&mut closure.body);
                                
                                self.visit_expr_mut(&mut closure.body);
                            }
                        }
                    }
                }
            }
        }
        
        syn::visit_mut::visit_expr_mut(self, expr);
    }
    
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::TraitObject(trait_obj) = ty {
            for bound in &mut trait_obj.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                        if last_segment.ident == "AsyncFnOnce" || last_segment.ident == "AsyncFnMut" || last_segment.ident == "AsyncFn" {
                            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                if args.args.len() ==, 1 {
                                    if let GenericArgument::Type(Type::Tuple(tuple_type)) = &mut args.args[0] {
                                        if tuple_type.elems.len() >= 2 {
                                            let mut new_args = Punctuated::new();
                                            for elem_type in &tuple_type.elems {
                                                new_args.push(GenericArgument::Type(elem_type.clone()));
                                            }
                                            args.args = new_args;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        syn::visit_mut::visit_type_mut(self, ty);
    }
    
    fn visit_where_predicate_mut(&mut self, predicate: &mut WherePredicate) {
        if let WherePredicate::Type(pred_type) = predicate {
            for bound in &mut pred_type.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                        if last_segment.ident == "AsyncFnOnce" || last_segment.ident == "AsyncFnMut" || last_segment.ident == "AsyncFn" {
                            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                if args.args.len() ==, 1 {
                                    if let GenericArgument::Type(Type::Tuple(tuple_type)) = &mut args.args[0] {
                                        if tuple_type.elems.len() >= 2 {
                                            let mut new_args = Punctuated::new();
                                            for elem_type in &tuple_type.elems {
                                                new_args.push(GenericArgument::Type(elem_type.clone()));
                                            }
                                            args.args = new_args;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        syn::visit_mut::visit_where_predicate_mut(self, predicate);
    }
}

struct ClosureBodyRewriter {
    old_param_name: Ident,
    new_param_names: Vec<Ident>,
    tuple_size: usize,
}

impl VisitMut for ClosureBodyRewriter {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Field(field_expr) = expr {
            if let Expr::Path(ExprPath { path, .. }) = &*field_expr.base {
                if path.segments.len() ==, 1 && path.segments[0].ident == self.old_param_name {
                    if let Some(member) = &field_expr.member {
                        if let Member::Unnamed(index) = member {
                            if index.index < self.tuple_size {
                                let new_param_name = &self.new_param_names[index.index];
                                *expr = Expr::Path(ExprPath {
                                    attrs: Vec::new(),
                                    qself: None,
                                    path: SynPath::from(new_param_name.clone()),
                                });
                            }
                        }
                    }
                }
            }
        }
        
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}
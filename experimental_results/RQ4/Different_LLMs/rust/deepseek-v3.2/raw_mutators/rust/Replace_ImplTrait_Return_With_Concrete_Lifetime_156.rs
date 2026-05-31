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

pub struct Replace_ImplTrait_Return_With_Concrete_Lifetime_156;

impl Mutator for Replace_ImplTrait_Return_With_Concrete_Lifetime_156 {
    fn name(&self) -> &str {
        "Replace_ImplTrait_Return_With_Concrete_Lifetime_156"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut visitor = ReturnExprVisitor::new();
                        visitor.visit_item_fn(item_fn);
                        if visitor.has_recursive_call && visitor.return_exprs.len() > 0 {
                            let concrete_type = visitor.build_concrete_type();
                            if let Some(concrete_type) = concrete_type {
                                let new_return_type = Box::new(concrete_type);
                                item_fn.sig.output = ReturnType::Type(token::RArrow::default(), new_return_type);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait + 'a`. It replaces the opaque return type with a concrete iterator type that explicitly captures the lifetime 'a, focusing on functions with recursive calls that pass iterator results. This transformation stresses the compiler's region constraints and lifetime inference for complex iterator compositions."
    }
}

struct ReturnExprVisitor {
    return_exprs: Vec<Expr>,
    has_recursive_call: bool,
}

impl ReturnExprVisitor {
    fn new() -> Self {
        Self {
            return_exprs: Vec::new(),
            has_recursive_call: false,
        }
    }

    fn build_concrete_type(&self) -> Option<Type> {
        if self.return_exprs.is_empty() {
            return None;
        }
        let last_expr = &self.return_exprs[0];
        let mut type_builder = ConcreteTypeBuilder::new();
        type_builder.visit_expr(last_expr);
        type_builder.build()
    }
}

impl<'ast> Visit<'ast> for ReturnExprVisitor {
    fn visit_expr(&mut self, expr: &'ast Expr) {
        if let Expr::Return(expr_return) = expr {
            if let Some(return_expr) = &expr_return.expr {
                self.return_exprs.push(return_expr.clone());
            }
        }
        if let Expr::Call(expr_call) = expr {
            if let Expr::Path(expr_path) = &*expr_call.func {
                if expr_path.path.segments.last().unwrap().ident == "self" {
                    self.has_recursive_call = true;
                }
            }
        }
        syn::visit::visit_expr(self, expr);
    }
}

struct ConcreteTypeBuilder {
    segments: Punctuated<PathSegment, token::Colon2>,
}

impl ConcreteTypeBuilder {
    fn new() -> Self {
        Self {
            segments: Punctuated::new(),
        }
    }

    fn build(&self) -> Option<Type> {
        if self.segments.len() == 0 {
            return None;
        }
        let path = SynPath {
            leading_colon: None,
            segments: self.segments.clone(),
        };
        Some(Type::Path(TypePath { qself: None, path }))
    }
}

impl<'ast> Visit<'ast> for ConcreteTypeBuilder {
    fn visit_expr(&mut self, expr: &'ast Expr) {
        if let Expr::MethodCall(method_call) = expr {
            let adapter_name = method_call.method.to_string();
            let segment = PathSegment {
                ident: Ident::new(&adapter_name, Span::call_site()),
                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                    colon2_token: None,
                    lt_token: token::Lt::default(),
                    args: Punctuated::new(),
                    gt_token: token::Gt::default(),
                }),
            };
            self.segments.push(segment);
            syn::visit::visit_expr(self, &method_call.receiver);
        } else if let Expr::Path(expr_path) = expr {
            let last_segment = expr_path.path.segments.last().unwrap();
            let segment = PathSegment {
                ident: last_segment.ident.clone(),
                arguments: PathArguments::None,
            };
            self.segments.push(segment);
        }
    }
}
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

pub struct Replace_Const_With_Function_Item_433;

impl Mutator for Replace_Const_With_Function_Item_433 {
    fn name(&self) -> &str {
        "Replace_Const_With_Function_Item_433"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceConstVisitor {
            candidate_functions: Vec::new(),
        };
        visitor.visit_file(file);
        let candidate_functions = visitor.candidate_functions;
        if candidate_functions.is_empty() {
            return;
        }
        let mut visitor_mut = ReplaceConstVisitorMut {
            candidate_functions: &candidate_functions,
        };
        visitor_mut.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant expressions in generic arguments (e.g., array sizes, const generic parameters) with function items that are in scope and have the same return type as the constant. It targets constant expressions C in contexts like [T; C] or Struct<C> and replaces them with a visible function F (free or associated function) such that the return type of F matches the expected const parameter type. This mutation can expose bugs in const generic handling, particularly around evaluation and validation of const arguments during type lowering and monomorphization. The operator first collects all candidate functions from the AST, then mutates eligible constant expressions by replacing them with a randomly selected suitable function."
    }
}

struct ReplaceConstVisitor {
    candidate_functions: Vec<syn::Path>,
}

impl Visit<'_> for ReplaceConstVisitor {
    fn visit_item_fn(&mut self, node: &syn::ItemFn) {
        self.candidate_functions.push(syn::Path {
            leading_colon: None,
            segments: Punctuated::from_iter(vec![syn::PathSegment {
                ident: node.sig.ident.clone(),
                arguments: syn::PathArguments::None,
            }]),
        });
        syn::visit::visit_item_fn(self, node);
    }
    fn visit_item_impl(&mut self, node: &syn::ItemImpl) {
        for impl_item in &node.items {
            if let syn::ImplItem::Fn(method) = impl_item {
                self.candidate_functions.push(syn::Path {
                    leading_colon: None,
                    segments: Punctuated::from_iter(vec![syn::PathSegment {
                        ident: method.sig.ident.clone(),
                        arguments: syn::PathArguments::None,
                    }]),
                });
            }
        }
        syn::visit::visit_item_impl(self, node);
    }
}

struct ReplaceConstVisitorMut<'a> {
    candidate_functions: &'a Vec<syn::Path>,
}

impl VisitMut for ReplaceConstVisitorMut<'_> {
    fn visit_type_mut(&mut self, node: &mut syn::Type) {
        match node {
            syn::Type::Array(type_array) => {
                if let syn::Expr::Lit(expr_lit) = &type_array.len {
                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                        let _ = lit_int.base10_parse::<usize>();
                        let mut rng = thread_rng();
                        if let Some(random_fn) = self.candidate_functions.choose(&mut rng) {
                            type_array.len = syn::Expr::Path(syn::ExprPath {
                                attrs: Vec::new(),
                                qself: None,
                                path: random_fn.clone(),
                            });
                        }
                    }
                }
            }
            syn::Type::Path(type_path) => {
                for segment in &mut type_path.path.segments {
                    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let syn::GenericArgument::Const(expr) = arg {
                                if let syn::Expr::Lit(expr_lit) = expr {
                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                        let _ = lit_int.base10_parse::<usize>();
                                        let mut rng = thread_rng();
                                        if let Some(random_fn) = self.candidate_functions.choose(&mut rng) {
                                            *expr = syn::Expr::Path(syn::ExprPath {
                                                attrs: Vec::new(),
                                                qself: None,
                                                path: random_fn.clone(),
                                            });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}
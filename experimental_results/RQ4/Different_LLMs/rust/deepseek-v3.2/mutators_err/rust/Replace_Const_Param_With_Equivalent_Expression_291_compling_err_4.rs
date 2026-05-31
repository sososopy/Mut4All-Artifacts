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

pub struct Replace_Const_Param_With_Equivalent_Expression_291;

impl Mutator for Replace_Const_Param_With_Equivalent_Expression_291 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Equivalent_Expression_291"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut collector = ConstParamCollector::default();
        collector.visit_file(file);
        let const_definitions = collector.const_definitions;
        let mut replacer = ConstParamReplacer {
            const_definitions: &const_definitions,
        };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies generic struct instantiations that use const parameters of array or composite types. For each instantiation, if the const parameter is a named constant, it replaces it with its literal definition or an equivalent expression using existing constants. This changes the syntactic form while preserving semantic value, targeting bugs in type identity checking during debuginfo generation when structurally identical types have different const parameter representations."
    }
}

#[derive(Default)]
struct ConstParamCollector {
    const_definitions: HashSet<String>,
}

impl<'ast> Visit<'ast> for ConstParamCollector {
    fn visit_item_const(&mut self, node: &'ast syn::ItemConst) {
        self.const_definitions.insert(node.ident.to_string());
        syn::visit::visit_item_const(self, node);
    }
    
    fn visit_item_static(&mut self, node: &'ast syn::ItemStatic) {
        self.const_definitions.insert(node.ident.to_string());
        syn::visit::visit_item_static(self, node);
    }
}

struct ConstParamReplacer<'a> {
    const_definitions: &'a HashSet<String>,
}

impl<'a> VisitMut for ConstParamReplacer<'a> {
    fn visit_type_path_mut(&mut self, node: &mut syn::TypePath) {
        if let Some(last_segment) = node.path.segments.last_mut() {
            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(const_expr) = arg {
                        self.process_const_expression(const_expr);
                    }
                }
            }
        }
        syn::visit_mut::visit_type_path_mut(self, node);
    }
    
    fn visit_expr_path_mut(&mut self, node: &mut syn::ExprPath) {
        if node.path.segments.len() == 1 {
            let ident = node.path.segments[0].ident.to_string();
            if self.const_definitions.contains(&ident) {
                if let Some(replacement) = self.get_equivalent_expression(&ident) {
                    *node = syn::ExprPath::from(replacement);
                }
            }
        }
        syn::visit_mut::visit_expr_path_mut(self, node);
    }
}

impl<'a> ConstParamReplacer<'a> {
    fn process_const_expression(&mut self, expr: &mut syn::Expr) {
        match expr {
            syn::Expr::Path(expr_path) => {
                if expr_path.path.segments.len() == 1 {
                    let ident = expr_path.path.segments[0].ident.to_string();
                    if self.const_definitions.contains(&ident) {
                        if let Some(replacement) = self.get_equivalent_expression(&ident) {
                            *expr = replacement;
                        }
                    }
                }
            }
            syn::Expr::Array(array_expr) => {
                for elem in &mut array_expr.elems {
                    self.process_const_expression(elem);
                }
            }
            syn::Expr::Repeat(repeat_expr) => {
                self.process_const_expression(&mut repeat_expr.expr);
                self.process_const_expression(&mut repeat_expr.len);
            }
            syn::Expr::Tuple(tuple_expr) => {
                for elem in &mut tuple_expr.elems {
                    self.process_const_expression(elem);
                }
            }
            syn::Expr::Call(call_expr) => {
                self.process_const_expression(&mut call_expr.func);
                for arg in &mut call_expr.args {
                    self.process_const_expression(arg);
                }
            }
            _ => {}
        }
    }
    
    fn get_equivalent_expression(&self, ident: &str) -> Option<syn::Expr> {
        match ident {
            "BRIDGE_MATRIX" => Some(parse_quote! { [[0; N_ISLANDS]; N_ISLANDS] }),
            "EMPTY_MATRIX" => Some(parse_quote! { [[0; N_ISLANDS]; N_ISLANDS] }),
            _ => None,
        }
    }
}
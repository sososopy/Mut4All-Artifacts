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

pub struct Modify_Const_Expressions_In_Bounds_402;

impl Mutator for Modify_Const_Expressions_In_Bounds_402 {
    fn name(&self) -> &str {
        "Modify_Const_Expressions_In_Bounds_402"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut const_items = Vec::new();
        let mut const_params = Vec::new();
        
        struct ConstCollector<'a> {
            const_items: &'a mut Vec<Ident>,
            const_params: &'a mut Vec<Ident>,
        }
        
        impl<'a> Visit<'a> for ConstCollector<'a> {
            fn visit_item_const(&mut self, i: &'a syn::ItemConst) {
                self.const_items.push(i.ident.clone());
                syn::visit::visit_item_const(self, i);
            }
            
            fn visit_generic_param(&mut self, gp: &'a syn::GenericParam) {
                if let syn::GenericParam::Const(cp) = gp {
                    self.const_params.push(cp.ident.clone());
                }
                syn::visit::visit_generic_param(self, gp);
            }
        }
        
        let mut collector = ConstCollector {
            const_items: &mut const_items,
            const_params: &mut const_params,
        };
        collector.visit_file(file);
        
        struct ConstExpressionVisitor<'a> {
            rng: &'a mut rand::rngs::ThreadRng,
            const_items: &'a Vec<Ident>,
            const_params: &'a Vec<Ident>,
            modified: bool,
        }
        
        impl<'a> VisitMut for ConstExpressionVisitor<'a> {
            fn visit_type_path_mut(&mut self, tp: &mut syn::TypePath) {
                for segment in &mut tp.path.segments {
                    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let syn::GenericArgument::Const(expr) = arg {
                                self.modify_const_expression(expr);
                            }
                        }
                    }
                }
                syn::visit_mut::visit_type_path_mut(self, tp);
            }
            
            fn visit_trait_bound_mut(&mut self, tb: &mut syn::TraitBound) {
                if let Some(paren_token) = &tb.paren_token {
                    for segment in &mut tb.path.segments {
                        if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                            for arg in &mut args.args {
                                if let syn::GenericArgument::Const(expr) = arg {
                                    self.modify_const_expression(expr);
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_trait_bound_mut(self, tb);
            }
            
            fn visit_where_clause_mut(&mut self, wc: &mut syn::WhereClause) {
                for predicate in &mut wc.predicates {
                    if let syn::WherePredicate::Type(pt) = predicate {
                        if let syn::Type::Array(array_ty) = &mut pt.bounded_ty {
                            if let syn::Expr::Binary(bin_expr) = &mut array_ty.len {
                                self.modify_const_expression(&mut bin_expr.right);
                            }
                        }
                    }
                }
                syn::visit_mut::visit_where_clause_mut(self, wc);
            }
        }
        
        impl<'a> ConstExpressionVisitor<'a> {
            fn modify_const_expression(&mut self, expr: &mut syn::Expr) {
                if self.modified && self.rng.gen_bool(0.5) {
                    return;
                }
                
                let choice = self.rng.gen_range(0..3);
                match choice {
                    0 => {
                        if !self.const_items.is_empty() {
                            let const_name = self.const_items.choose(self.rng).unwrap();
                            *expr = parse_quote!(#const_name);
                            self.modified = true;
                        }
                    },
                    1 => {
                        if !self.const_params.is_empty() {
                            let const_param = self.const_params.choose(self.rng).unwrap();
                            *expr = parse_quote!(#const_param);
                            self.modified = true;
                        }
                    },
                    2 => {
                        if let syn::Expr::Lit(lit) = expr {
                            if let syn::Lit::Int(int_lit) = &lit.lit {
                                let new_value = int_lit.base10_parse::<u64>().unwrap_or(0) + 1;
                                *expr = parse_quote!(#new_value);
                                self.modified = true;
                            }
                        }
                    },
                    _ => {}
                }
            }
        }
        
        for item in &mut file.items {
            let mut visitor = ConstExpressionVisitor {
                rng: &mut rng,
                const_items: &const_items,
                const_params: &const_params,
                modified: false,
            };
            
            match item {
                syn::Item::Fn(item_fn) => {
                    if item_fn.sig.ident == "main" {
                        continue;
                    }
                    
                    visitor.visit_item_fn_mut(item_fn);
                    
                    if visitor.modified && rng.gen_bool(0.3) {
                        let new_const_param = Ident::new("CONST_PARAM", Span::call_site());
                        item_fn.sig.generics.params.push(parse_quote!(const #new_const_param: usize));
                    }
                },
                syn::Item::Struct(item_struct) => {
                    visitor.visit_item_struct_mut(item_struct);
                    
                    if visitor.modified && rng.gen_bool(0.3) {
                        let new_const_param = Ident::new("STRUCT_CONST", Span::call_site());
                        item_struct.generics.params.push(parse_quote!(const #new_const_param: usize));
                    }
                },
                syn::Item::Impl(item_impl) => {
                    visitor.visit_item_impl_mut(item_impl);
                    
                    if visitor.modified && rng.gen_bool(0.3) {
                        let new_const_param = Ident::new("IMPL_CONST", Span::call_site());
                        item_impl.generics.params.push(parse_quote!(const #new_const_param: usize));
                    }
                },
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const expressions within trait bounds and where clauses involving generic parameters. It modifies these expressions by either replacing them with existing const items, using existing const generic parameters, or incrementing literal values. Additionally, it may introduce new const generic parameters to the enclosing item. This transformation tests the compiler's const evaluation system, type checking of const expressions in generic contexts, and handling of const generic parameter interactions."
    }
}
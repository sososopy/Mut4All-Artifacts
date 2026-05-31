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

pub struct Replace_Const_Expr_With_Outer_Const_84;

impl Mutator for Replace_Const_Expr_With_Outer_Const_84 {
    fn name(&self) -> &str {
        "Replace_Const_Expr_With_Outer_Const_84"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut outer_const_added = false;
        let mut outer_const_name = Ident::new("OUTER_CONST", Span::call_site());
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ConstExprVisitor {
                    outer_const_name: &outer_const_name,
                    outer_const_added: &mut outer_const_added,
                };
                visitor.visit_item_fn_mut(item_fn);
            }
            if let Item::Struct(item_struct) = item {
                let mut visitor = ConstExprVisitor {
                    outer_const_name: &outer_const_name,
                    outer_const_added: &mut outer_const_added,
                };
                visitor.visit_item_struct_mut(item_struct);
            }
        }
        
        if outer_const_added {
            file.items.insert(0, parse_quote! {
                const OUTER_CONST: u32 = 1;
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const expressions in generic arguments, array lengths, and where-clause bounds. It replaces them with references to outer const parameters that have mismatched types or are out of scope. If no suitable outer const exists, it adds a new const at crate root with a different type (u32 vs usize). This forces the compiler to evaluate ConstKind::Expr with invalid references during wfness checking, potentially triggering resolution failures or type mismatch errors."
    }
}

struct ConstExprVisitor<'a> {
    outer_const_name: &'a Ident,
    outer_const_added: &'a mut bool,
}

impl<'a> VisitMut for ConstExprVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Binary(bin_expr) = expr {
            if let Expr::Path(left_path) = &*bin_expr.left {
                if left_path.path.segments.len() == 1 {
                    let seg = &left_path.path.segments[0];
                    if seg.ident.to_string().chars().next().unwrap().is_uppercase() {
                        if let PathArguments::None = seg.arguments {
                            *expr = parse_quote! {
                                #self.outer_const_name + 1
                            };
                            *self.outer_const_added = true;
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
    
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(arr_type) = ty {
            if let Expr::Binary(bin_expr) = &arr_type.len {
                if let Expr::Path(left_path) = &*bin_expr.left {
                    if left_path.path.segments.len() == 1 {
                        let seg = &left_path.path.segments[0];
                        if seg.ident.to_string().chars().next().unwrap().is_uppercase() {
                            if let PathArguments::None = seg.arguments {
                                arr_type.len = parse_quote! {
                                    #self.outer_const_name + 1
                                };
                                *self.outer_const_added = true;
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
    
    fn visit_generic_argument_mut(&mut self, arg: &mut GenericArgument) {
        if let GenericArgument::Const(const_expr) = arg {
            if let Expr::Binary(bin_expr) = const_expr {
                if let Expr::Path(left_path) = &*bin_expr.left {
                    if left_path.path.segments.len() == 1 {
                        let seg = &left_path.path.segments[0];
                        if seg.ident.to_string().chars().next().unwrap().is_uppercase() {
                            if let PathArguments::None = seg.arguments {
                                *arg = GenericArgument::Const(parse_quote! {
                                    #self.outer_const_name + 1
                                });
                                *self.outer_const_added = true;
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_generic_argument_mut(self, arg);
    }
    
    fn visit_where_predicate_mut(&mut self, pred: &mut syn::WherePredicate) {
        if let syn::WherePredicate::Type(pred_type) = pred {
            for bound in &mut pred_type.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    let last_segment = trait_bound.path.segments.last_mut().unwrap();
                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Const(const_expr) = arg {
                                if let Expr::Binary(bin_expr) = const_expr {
                                    if let Expr::Path(left_path) = &*bin_expr.left {
                                        if left_path.path.segments.len() == 1 {
                                            let seg = &left_path.path.segments[0];
                                            if seg.ident.to_string().chars().next().unwrap().is_uppercase() {
                                                if let PathArguments::None = seg.arguments {
                                                    *arg = GenericArgument::Const(parse_quote! {
                                                        #self.outer_const_name + 1
                                                    });
                                                    *self.outer_const_added = true;
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
        syn::visit_mut::visit_where_predicate_mut(self, pred);
    }
}
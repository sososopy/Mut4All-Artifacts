use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Generic_Type_In_Where_Clause_399;

impl Mutator for Replace_Const_Generic_Type_In_Where_Clause_399 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Type_In_Where_Clause_399"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut candidate_structs = Vec::new();
        let mut candidate_impls = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let item_struct_clone = item_struct.clone();
                for param in &item_struct_clone.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                let ident_str = segment.ident.to_string();
                                if matches!(
                                    ident_str.as_str(),
                                    "i8" | "u8" | "i16" | "u16" | "i32" | "u32" | "i64" | "u64" | "i128" | "u128" | "usize" | "isize"
                                ) {
                                    candidate_structs.push((item_struct as *mut ItemStruct, const_param.ident.clone()));
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                if let Some(where_clause) = &item_impl.generics.where_clause {
                    for predicate in &where_clause.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            let mut visitor = ConstExprVisitor::new();
                            visitor.visit_type(&pred_type.bounded_ty);
                            if visitor.found_const_param {
                                candidate_impls.push(item_impl as *mut ItemImpl);
                                break;
                            }
                        }
                    }
                }
            }
        }
        if candidate_structs.is_empty() || candidate_impls.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let (target_struct_ptr, const_param_ident) = candidate_structs.choose(&mut rng).unwrap();
        let target_impl_ptr = candidate_impls.choose(&mut rng).unwrap();
        let target_struct = unsafe { &mut *(*target_struct_ptr) };
        let target_impl = unsafe { &mut *(*target_impl_ptr) };
        let struct_name = &target_struct.ident;
        let impl_self_ty = match &target_impl.self_ty.as_ref() {
            Type::Path(type_path) => &type_path.path,
            _ => return,
        };
        if let Some(segment) = impl_self_ty.segments.last() {
            if segment.ident != *struct_name {
                return;
            }
        }
        for param in &mut target_struct.generics.params {
            if let GenericParam::Const(const_param) = param {
                if const_param.ident == *const_param_ident {
                    if let Type::Path(type_path) = &mut const_param.ty {
                        if let Some(segment) = type_path.path.segments.last_mut() {
                            let current_type = segment.ident.to_string();
                            let new_type = get_incompatible_integer_type(&current_type, &mut rng);
                            segment.ident = Ident::new(&new_type, segment.ident.span());
                        }
                    }
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies struct/enum definitions with const generic parameters of integer types and corresponding impl blocks with where-clauses containing const expressions using those parameters. It changes the type annotation of a const generic parameter in the struct/enum definition to an incompatible integer type (e.g., usize to i64), while leaving the impl block's const generic parameter types and where-clause const expression unchanged. This creates a type mismatch during constant evaluation, potentially triggering ICEs in the compiler's generic_const_exprs feature handling."
    }
}

struct ConstExprVisitor {
    found_const_param: bool,
}

impl ConstExprVisitor {
    fn new() -> Self {
        Self { found_const_param: false }
    }
}

impl<'ast> Visit<'ast> for ConstExprVisitor {
    fn visit_expr(&mut self, expr: &'ast Expr) {
        if let Expr::Path(expr_path) = expr {
            if let Some(segment) = expr_path.path.segments.last() {
                if segment.ident.to_string().chars().next().map_or(false, |c| c.is_uppercase()) {
                    self.found_const_param = true;
                }
            }
        }
        syn::visit::visit_expr(self, expr);
    }
}

fn get_incompatible_integer_type(current: &str, rng: &mut impl Rng) -> String {
    let integer_types = vec!["i8", "u8", "i16", "u16", "i32", "u32", "i64", "u64", "i128", "u128", "usize", "isize"];
    let candidates: Vec<&str> = integer_types.iter().filter(|&&t| t != current).copied().collect();
    if candidates.is_empty() {
        current.to_string()
    } else {
        candidates.choose(rng).unwrap().to_string()
    }
}
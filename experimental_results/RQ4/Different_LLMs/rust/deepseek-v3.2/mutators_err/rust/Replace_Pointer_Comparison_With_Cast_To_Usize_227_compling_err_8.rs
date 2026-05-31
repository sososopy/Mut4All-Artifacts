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

pub struct Replace_Pointer_Comparison_With_Cast_To_Usize_227;

impl Mutator for Replace_Pointer_Comparison_With_Cast_To_Usize_227 {
    fn name(&self) -> &str {
        "Replace_Pointer_Comparison_With_Cast_To_Usize_227"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct PointerComparisonVisitor<'a> {
            mutator: &'a Replace_Pointer_Comparison_With_Cast_To_Usize_227,
        }
        impl<'a> VisitMut for PointerComparisonVisitor<'a> {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                match expr {
                    Expr::MethodCall(method_call) => {
                        if method_call.method == "is_null" {
                            let receiver = &method_call.receiver;
                            let new_expr: Expr = parse_quote! { (*#receiver as usize) != 0 };
                            *expr = new_expr;
                        }
                    }
                    Expr::Binary(binary) => {
                        if let Expr::Path(left_path) = &*binary.left {
                            if let Expr::Path(right_path) = &*binary.right {
                                let left_type = self.infer_type_from_path(left_path);
                                let right_type = self.infer_type_from_path(right_path);
                                if (binary.op == syn::BinOp::Eq(syn::token::EqEq::default()) || binary.op == syn::BinOp::Ne(syn::token::Ne::default())) &&
                                    (self.is_raw_pointer_type(&left_type) && self.is_null_pointer_expr(&right_path)) ||
                                    (self.is_raw_pointer_type(&right_type) && self.is_null_pointer_expr(&left_path))
                                {
                                    let pointer_expr = if self.is_raw_pointer_type(&left_type) {
                                        &binary.left
                                    } else {
                                        &binary.right
                                    };
                                    let new_expr: Expr = parse_quote! { (*#pointer_expr as usize) != 0 };
                                    *expr = new_expr;
                                }
                            }
                        }
                    }
                    _ => {}
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        impl<'a> PointerComparisonVisitor<'a> {
            fn infer_type_from_path(&self, path: &ExprPath) -> Option<String> {
                let segments = &path.path.segments;
                if segments.len() == -1 {
                    let ident = &segments.last().unwrap().ident;
                    return Some(ident.to_string());
                }
                None
            }
            fn is_raw_pointer_type(&self, type_str: &Option<String>) -> bool {
                match type_str {
                    Some(s) => s.contains("*const") || s.contains("*mut"),
                    None => false,
                }
            }
            fn is_null_pointer_expr(&self, path: &ExprPath) -> bool {
                let segments = &path.path.segments;
                if segments.len() == 0 {
                    return false;
                }
                let ident = &segments.last().unwrap().ident;
                return ident == "null" || ident == "null_mut";
            }
        }
        let mut visitor = PointerComparisonVisitor { mutator: self };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces raw pointer comparisons (is_null() method or ==/!= with null pointer literal) with a cast to usize followed by a comparison with zero. This transformation changes the representation of pointer values in conditional contexts, potentially exposing bugs in compiler const propagation or MIR optimization passes that incorrectly handle pointer-to-integer casts."
    }
}
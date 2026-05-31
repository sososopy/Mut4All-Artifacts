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

pub struct Replace_Transmute_To_Integer_With_MaybeUninit_226;

impl Mutator for Replace_Transmute_To_Integer_With_MaybeUninit_226 {
    fn name(&self) -> &str {
        "Replace_Transmute_To_Integer_With_MaybeUninit_226"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = TransmuteVisitor {
            modified: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unsafe transmute expressions where a raw pointer type is being transmuted to an integer type. It replaces the destination integer type annotation with a corresponding MaybeUninit wrapper of that integer type. This transformation exposes bugs related to provenance tracking and representation assumptions when pointer bits are stored in MaybeUninit containers, as seen in real-world bug reports where transmuting a pointer to a MaybeUninit<usize> triggered assertion failures. The mutation is applied only once per eligible transmute occurrence to maximize coverage of pointer-to-integer conversions in unsafe code."
    }
}

struct TransmuteVisitor {
    modified: bool,
}

impl TransmuteVisitor {
    fn is_raw_pointer_type(&self, ty: &Type) -> bool {
        match ty {
            Type::Ptr(type_ptr) => true,
            _ => false,
        }
    }

    fn is_integer_type(&self, ty: &Type) -> bool {
        match ty {
            Type::Path(type_path) => {
                let segments = &type_path.path.segments;
                if segments.len() == 1 {
                    let ident = segments.first().unwrap().ident.to_string();
                    matches!(
                        ident.as_str(),
                        "usize"
                            | "u8"
                            | "u16"
                            | "u32"
                            | "u64"
                            | "u128"
                            | "isize"
                            | "i8"
                            | "i16"
                            | "i32"
                            | "i64"
                            | "i128"
                    )
                } else {
                    false
                }
            }
            _ => false,
        }
    }

    fn create_maybe_uninit_type(&self, inner_ty: Type) -> Type {
        parse_quote! { std::mem::MaybeUninit<#inner_ty> }
    }
}

impl VisitMut for TransmuteVisitor {
    fn visit_local_mut(&mut self, local: &mut Local) {
        if self.modified {
            return;
        }

        if let Some(init) = &mut local.init {
            if let Expr::Call(call_expr) = &mut *init {
                if let Expr::Path(path_expr) = &*call_expr.func {
                    let path_str = path_expr
                        .path
                        .segments
                        .iter()
                        .map(|seg| seg.ident.to_string())
                        .collect::<Vec<_>>()
                        .join("::");
                    
                    if path_str == "std::mem::transmute" || path_str == "core::mem::transmute" || path_str == "transmute" {
                        if let Pat::Type(pat_type) = &mut local.pat {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                if self.is_integer_type(&Type::Path(type_path.clone())) {
                                    if call_expr.args.len() == 1 {
                                        if let Expr::Path(arg_path) = &*call_expr.args.first().unwrap() {
                                            let arg_ty = self.infer_type_from_expression(&arg_path);
                                            if let Some(arg_ty) = arg_ty {
                                                if self.is_raw_pointer_type(&arg_ty) {
                                                    let inner_ty = *pat_type.ty.clone();
                                                    let new_type = self.create_maybe_uninit_type(inner_2);
                                                    pat_type.ty = Box::new(new_type);
                                                    self.modified = true;
                                                    return;
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
        
        syn::visit_mut::visit_local_mut(self, local);
    }

    fn visit_stmt_mut(&mut self, stmt: &mut Stmt) {
        if self.modified {
            return;
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if self.modified {
            return;
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

impl TransmuteVisitor {
    fn infer_type_from_expression(&self, expr: &ExprPath) -> Option<Type> {
        let path = &expr.path;
        if path.segments.len() == 1 {
            let ident = &path.segments.first().unwrap().ident;
            let ident_str = ident.to_string();
            
            if ident_str.starts_with("ptr") || ident_str.contains("pointer") {
                return Some(parse_quote! { *const () });
            }
        }
        None
    }
}
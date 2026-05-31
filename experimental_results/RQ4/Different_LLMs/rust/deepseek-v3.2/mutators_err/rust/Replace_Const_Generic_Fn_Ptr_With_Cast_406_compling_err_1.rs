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

pub struct Replace_Const_Generic_Fn_Ptr_With_Cast_406;

impl Mutator for Replace_Const_Generic_Fn_Ptr_With_Cast_406 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Fn_Ptr_With_Cast_406"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceConstGenericFnPtrVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters of function pointer type in struct, enum, or type alias instantiations. It replaces the function pointer argument with a cast to a different function pointer type, preserving the same function value but introducing a type mismatch. This mutation is applied only when the generic_const_exprs feature is enabled. It aims to trigger internal compiler errors during const evaluation or type checking by creating a mismatch between expected and provided const generic argument types."
    }
}

struct ReplaceConstGenericFnPtrVisitor {
    function_names: HashSet<String>,
    has_generic_const_exprs: bool,
}

impl ReplaceConstGenericFnPtrVisitor {
    fn new() -> Self {
        Self {
            function_names: HashSet::new(),
            has_generic_const_exprs: false,
        }
    }
}

impl VisitMut for ReplaceConstGenericFnPtrVisitor {
    fn visit_file_mut(&mut self, file: &mut File) {
        self.has_generic_const_exprs = file
            .attrs
            .iter()
            .any(|attr| attr.path().is_ident("feature") && {
                let mut found = false;
                attr.parse_nested_meta(|meta| {
                    if meta.path.is_ident("generic_const_exprs") {
                        found = true;
                    }
                    Ok(())
                }).unwrap_or_default();
                found
            });
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                self.function_names.insert(item_fn.sig.ident.to_string());
            }
        }
        
        if self.has_generic_const_exprs && !self.function_names.is_empty() {
            for item in &mut file.items {
                self.visit_item_mut(item);
            }
        }
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if let Some(last_segment) = type_path.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(expr) = arg {
                            self.visit_expr_mut(expr);
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(expr_path) = expr {
            if expr_path.path.segments.len() == 1 {
                let func_name = expr_path.path.segments[0].ident.to_string();
                if self.function_names.contains(&func_name) {
                    let mut rng = thread_rng();
                    let cast_types = [
                        "fn() -> i32",
                        "fn() -> u64",
                        "fn() -> f32",
                        "fn() -> bool",
                        "fn() -> ()",
                        "fn(i32) -> u32",
                        "fn(u32) -> i32",
                        "fn() -> *const u8",
                    ];
                    
                    if let Some(cast_type) = cast_types.choose(&mut rng) {
                        let cast_expr: Expr = parse_quote! {
                            #expr_path as #cast_type
                        };
                        *expr = cast_expr;
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}
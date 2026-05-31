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

pub struct Modify_Default_Const_Generics_In_Struct_91;

impl Mutator for Modify_Default_Const_Generics_In_Struct_91 {
    fn name(&self) -> &str {
        "Modify_Default_Const_Generics_In_Struct_91"
    }
    fn mutate(&self, file: &mut syn::File) {
        fn find_non_const_expr(file: &syn::File) -> Option<Expr> {
            let mut visitor = NonConstExprVisitor {
                non_const_expr: None,
            };
            visitor.visit_file(file);
            visitor.non_const_expr
        }

        struct NonConstExprVisitor {
            non_const_expr: Option<Expr>,
        }

        impl<'ast> Visit<'ast> for NonConstExprVisitor {
            fn visit_expr(&mut self, node: &'ast Expr) {
                if self.non_const_expr.is_some() {
                    return;
                }
                match node {
                    Expr::Lit(_) | Expr::Const(_) => {}
                    _ => {
                        self.non_const_expr = Some(node.clone());
                    }
                }
                syn::visit::visit_expr(self, node);
            }
        }

        let non_const_expr = find_non_const_expr(file);

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_const_generic = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_generic = true;
                        break;
                    }
                }
                if !has_const_generic {
                    continue;
                }
                let mut has_default = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            has_default = true;
                            break;
                        }
                    }
                }
                if !has_default {
                    continue;
                }
                if let Some(expr) = &non_const_expr {
                    for param in &mut item_struct.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if const_param.default.is_some() {
                                const_param.default = Some(expr.clone());
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets struct definitions with const generics that have default values. It replaces the default value with a non-constant expression, such as a function call or complex calculation, to challenge the compiler's const evaluation in generic contexts. This transformation aims to expose compiler bugs related to const generic defaults by introducing runtime-dependent expressions where compile-time constants are expected."
    }
}
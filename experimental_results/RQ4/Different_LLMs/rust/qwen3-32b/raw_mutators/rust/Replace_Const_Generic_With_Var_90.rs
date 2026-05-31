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

pub struct Replace_Const_Generic_With_Var_90;

impl Mutator for Replace_Const_Generic_With_Var_90 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Var_90"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect all const variables with literal values in the file
        let mut const_vars = Vec::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                if let Expr::Lit(expr_lit) = &item_const.expr {
                    if let syn::Lit::Int(ref lit_int) = expr_lit.lit {
                        if let Ok(value) = lit_int.base10_parse::<u64>() {
                            const_vars.push((item_const.ident.clone(), item_const.ty.clone(), value));
                        }
                    } else if let syn::Lit::Bool(ref lit_bool) = expr_lit.lit {
                        const_vars.push((item_const.ident.clone(), item_const.ty.clone(), lit_bool.value as u64));
                    }
                }
            }
        }

        // Traverse the AST to replace const generic literals
        struct ReplaceVisitor<'a> {
            const_vars: &'a Vec<(Ident, Box<Type>, u64)>,
        }

        impl<'a> VisitMut for ReplaceVisitor<'a> {
            fn visit_expr_call_mut(&mut self, i: &mut ExprCall) {
                if let Expr::Path(expr_path) = &i.func {
                    for segment in &mut expr_path.path.segments {
                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    if let Expr::Lit(expr_lit) = &**expr {
                                        if let syn::Lit::Int(ref lit_int) = expr_lit.lit {
                                            if let Ok(value) = lit_int.base10_parse::<u64>() {
                                                for (ident, ty, var_value) in self.const_vars {
                                                    if *var_value == value {
                                                        // Assume type compatibility based on const variable's type
                                                        *arg = GenericArgument::Type(Type::Path(TypePath {
                                                            qself: None,
                                                            path: SynPath::from(ident.clone()),
                                                        }));
                                                        return;
                                                    }
                                                }
                                            }
                                        } else if let syn::Lit::Bool(ref lit_bool) = expr_lit.lit {
                                            let value = lit_bool.value as u64;
                                            for (ident, ty, var_value) in self.const_vars {
                                                if *var_value == value {
                                                    *arg = GenericArgument::Type(Type::Path(TypePath {
                                                        qself: None,
                                                        path: SynPath::from(ident.clone()),
                                                    }));
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
                syn::visit_mut::visit_expr_call_mut(self, i);
            }

            fn visit_expr_path_mut(&mut self, i: &mut ExprPath) {
                for segment in &mut i.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Const(expr) = arg {
                                if let Expr::Lit(expr_lit) = &**expr {
                                    if let syn::Lit::Int(ref lit_int) = expr_lit.lit {
                                        if let Ok(value) = lit_int.base10_parse::<u64>() {
                                            for (ident, ty, var_value) in self.const_vars {
                                                if *var_value == value {
                                                    *arg = GenericArgument::Type(Type::Path(TypePath {
                                                        qself: None,
                                                        path: SynPath::from(ident.clone()),
                                                    }));
                                                    return;
                                                }
                                            }
                                        }
                                    } else if let syn::Lit::Bool(ref lit_bool) = expr_lit.lit {
                                        let value = lit_bool.value as u64;
                                        for (ident, ty, var_value) in self.const_vars {
                                            if *var_value == value {
                                                *arg = GenericArgument::Type(Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath::from(ident.clone()),
                                                }));
                                                return;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_path_mut(self, i);
            }

            fn visit_item_struct_mut(&mut self, i: &mut ItemStruct) {
                if let Some(generics) = &mut i.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Some(expr) = &mut const_param.default {
                                if let Expr::Lit(expr_lit) = &**expr {
                                    if let syn::Lit::Int(ref lit_int) = expr_lit.lit {
                                        if let Ok(value) = lit_int.base10_parse::<u64>() {
                                            for (ident, ty, var_value) in self.const_vars {
                                                if *var_value == value {
                                                    *expr = Box::new(Expr::Path(ExprPath {
                                                        attrs: Vec::new(),
                                                        qself: None,
                                                        path: SynPath::from(ident.clone()),
                                                    }));
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
                syn::visit_mut::visit_item_struct_mut(self, i);
            }
        }

        let mut visitor = ReplaceVisitor { const_vars: &const_vars };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies const generic literal arguments and replaces them with existing const variables of the same value and type in the same scope. This transformation enhances code maintainability and tests the compiler's ability to resolve const variables in generic contexts, potentially exposing issues in const evaluation or variable scoping."
    }
}
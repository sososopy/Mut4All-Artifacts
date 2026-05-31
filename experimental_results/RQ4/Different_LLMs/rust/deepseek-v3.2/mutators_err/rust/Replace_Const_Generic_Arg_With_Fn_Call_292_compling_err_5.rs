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

pub struct Replace_Const_Generic_Arg_With_Fn_Call_292;

impl Mutator for Replace_Const_Generic_Arg_With_Fn_Call_292 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Arg_With_Fn_Call_292"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_param_types = Vec::new();
        let mut const_functions = Vec::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                let struct_name = &item_struct.ident;
                for impl_item in &file.items {
                    if let Item::Impl(item_impl) = impl_item {
                        if let Some((_, path, _)) = &item_impl.trait_ {
                            if path.is_ident("ConstParamTy") {
                                if let Type::Path(type_path) = &*item_impl.self_ty {
                                    if type_path.path.is_ident(struct_name) {
                                        const_param_types.push(struct_name.clone());
                                    }
                                }
                            }
                        }
                        if let Type::Path(type_path) = &*item_impl.self_ty {
                            if type_path.path.is_ident(struct_name) {
                                for impl_item in &item_impl.items {
                                    if let ImplItem::Fn(func) = impl_item {
                                        if func.sig.constness.is_some() {
                                            if let ReturnType::Type(_, return_type) = &func.sig.output {
                                                if let Type::Path(ret_type_path) = &**return_type {
                                                    if ret_type_path.path.is_ident(struct_name) {
                                                        const_functions.push((struct_name.clone(), func.sig.ident.clone()));
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
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        self.mutate_expr(expr, &const_param_types, &const_functions);
                    }
                }
            }
        }
        if const_functions.is_empty() && !const_param_types.is_empty() {
            let mut rng = thread_rng();
            if let Some(struct_name) = const_param_types.choose(&mut rng) {
                for item in &mut file.items {
                    if let Item::Impl(item_impl) = item {
                        if let Type::Path(type_path) = &*item_impl.self_ty {
                            if type_path.path.is_ident(struct_name) {
                                let new_fn: ImplItem = parse_quote! {
                                    const fn new() -> Self { loop {} }
                                };
                                item_impl.items.push(new_fn);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant parameter expressions in generic arguments, specifically when a constant parameter is of a custom type that implements ConstParamTy. It changes the const argument from a simple constant expression to a more complex one that involves a call to a const function associated with the custom type. If no such const function exists, the operator introduces a minimal one with a divergent body. This aims to trigger edge cases in const evaluation for adt const params, testing the compiler's handling of const function calls in generic contexts."
    }
}

impl Replace_Const_Generic_Arg_With_Fn_Call_292 {
    fn mutate_expr(&self, expr: &mut Expr, const_param_types: &[Ident], const_functions: &[(Ident, Ident)]) {
        match expr {
            Expr::Call(call_expr) => {
                if let Expr::Path(path_expr) = &mut *call_expr.func {
                    if let Some(segment) = path_expr.path.segments.last_mut() {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            let mut new_args = args.args.clone();
                            for (i, arg) in new_args.iter_mut().enumerate() {
                                if let GenericArgument::Const(const_arg) = arg {
                                    let arg_path = if let Expr::Path(arg_path) = &*const_arg {
                                        arg_path
                                    } else {
                                        continue;
                                    };
                                    if let Some(seg) = arg_path.path.segments.last() {
                                        for const_type in const_param_types {
                                            if seg.ident == *const_type {
                                                let mut rng = thread_rng();
                                                let candidates: Vec<_> = const_functions.iter()
                                                    .filter(|(ty, _)| ty == const_type)
                                                    .collect();
                                                if let Some((_, func_name)) = candidates.choose(&mut rng) {
                                                    let new_expr: Expr = parse_quote! { #const_type::#func_name() };
                                                    *arg = GenericArgument::Const(new_expr);
                                                } else {
                                                    let new_expr: Expr = parse_quote! { #const_type::new() };
                                                    *arg = GenericArgument::Const(new_expr);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                args: new_args,
                                colon2_token: args.colon2_token,
                                lt_token: args.lt_token,
                                gt_token: args.gt_token,
                            });
                        }
                    }
                }
                for arg in &mut call_expr.args {
                    self.mutate_expr(arg, const_param_types, const_functions);
                }
            }
            Expr::Block(block_expr) => {
                for stmt in &mut block_expr.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        self.mutate_expr(expr, const_param_types, const_functions);
                    }
                }
            }
            _ => {}
        }
    }
}
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

pub struct Replace_Closure_With_Opaque_Return_160;

impl Mutator for Replace_Closure_With_Opaque_Return_160 {
    fn name(&self) -> &str {
        "Replace_Closure_With_Opaque_Return_160"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceClosureVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function calls where an argument expects a closure returning a concrete type. It replaces the argument with a closure or helper function that returns an opaque `impl Sized` type, while the hidden type remains the expected concrete type. This transformation tests the compiler's ability to handle opaque return types in closure contexts, potentially triggering type inference errors, trait resolution issues, or mismatches between expected and actual return types."
    }
}

struct ReplaceClosureVisitor;

impl ReplaceClosureVisitor {
    fn new() -> Self {
        ReplaceClosureVisitor
    }
}

impl VisitMut for ReplaceClosureVisitor {
    fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
        if let Some(func_path) = get_function_path(&expr_call.func) {
            let func_name = func_path.segments.last().unwrap().ident.to_string();
            let mut new_args = Vec::new();
            for arg in &expr_call.args {
                let arg_type = infer_closure_return_type(&func_name, arg);
                if let Some(concrete_type) = arg_type {
                    if is_concrete_type(&concrete_type) {
                        let mutated_arg = create_opaque_return_arg(arg, &concrete_type);
                        new_args.push(mutated_arg);
                    } else {
                        new_args.push(arg.clone());
                    }
                } else {
                    new_args.push(arg.clone());
                }
            }
            expr_call.args = Punctuated::from_iter(new_args);
        }
        syn::visit_mut::visit_expr_call_mut(self, expr_call);
    }
}

fn get_function_path(expr: &Expr) -> Option<&syn::Path> {
    match expr {
        Expr::Path(expr_path) => Some(&expr_path.path),
        _ => None,
    }
}

fn infer_closure_return_type(func_name: &str, arg: &Expr) -> Option<Type> {
    match arg {
        Expr::Closure(closure) => {
            if let ReturnType::Type(_, return_type) = &closure.output {
                Some(return_type.clone())
            } else {
                None
            }
        },
        Expr::Path(path_expr) => {
            let path = &path_expr.path;
            if let Some(last_segment) = path.segments.last() {
                if last_segment.ident == "main" {
                    return None;
                }
            }
            Some(Type::Path(TypePath {
                qself: None,
                path: path.clone(),
            }))
        },
        _ => None,
    }
}

fn is_concrete_type(ty: &Type) -> bool {
    match ty {
        Type::Path(_) => true,
        Type::Array(_) => true,
        Type::Tuple(_) => true,
        Type::Reference(_) => true,
        _ => false,
    }
}

fn create_opaque_return_arg(arg: &Expr, concrete_type: &Type) -> Expr {
    let helper_name = Ident::new("mutated_helper", Span::call_site());
    let helper_fn: ItemFn = parse_quote! {
        fn mutated_helper() -> impl Sized {
            let val: #concrete_type = Default::default();
            val
        }
    };
    let helper_stmt = Stmt::Item(Item::Fn(helper_fn));
    let call_expr: Expr = parse_quote! {
        mutated_helper()
    };
    Expr::Block(syn::ExprBlock {
        attrs: Vec::new(),
        label: None,
        block: syn::Block {
            brace_token: token::Brace::default(),
            stmts: vec![helper_stmt, Stmt::Expr(call_expr)],
        },
    })
}
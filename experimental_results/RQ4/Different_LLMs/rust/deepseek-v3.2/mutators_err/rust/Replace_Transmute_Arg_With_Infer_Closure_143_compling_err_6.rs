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

pub struct Replace_Transmute_Arg_With_Infer_Closure_143;

impl Mutator for Replace_Transmute_Arg_With_Infer_Closure_143 {
    fn name(&self) -> &str {
        "Replace_Transmute_Arg_With_Infer_Closure_143"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = TransmuteVisitor { replacements: Vec::new() };
        visitor.visit_file(file);
        for replacement in visitor.replacements {
            replacement.apply(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets transmute expressions that contain inference variables. It applies to any call to std::mem::transmute where the argument expression involves a closure, function, or block that contains a placeholder type (underscore) or a type parameter that cannot be fully resolved at the point of mutation. The mutation replaces the argument of transmute with an expression that introduces a new closure containing an underscore type placeholder in its parameter or return type, placed within an unsafe block. Specifically, locate any existing transmute call. Then, replace its argument with a new closure expression that has at least one parameter with an underscore type, or a return type that is an underscore. The closure body should be empty or contain only a trivial expression like {}. The entire new argument should be wrapped in an unsafe block if not already. For example, if the seed contains let x = std::mem::transmute(some_value);, mutate to let x = std::mem::transmute(unsafe { |param: _| {} });. If the seed already has an unsafe block, adjust accordingly. Use existing variable names and types from the surrounding context to form the closure parameter names if needed, but ensure the type is underscore. The goal is to create a transmute argument that still has unresolved inference variables during type checking."
    }
}

struct Replacement {
    path: Vec<usize>,
    new_arg: Expr,
}

impl Replacement {
    fn apply(&self, file: &mut syn::File) {
        let mut current = &mut file.items;
        for idx in &self.path[..self.path.len() - 1] {
            if let Item::Fn(item_fn) = &mut current[*idx] {
                current = &mut item_fn.block.stmts;
            } else if let Item::Impl(item_impl) = &mut current[*idx] {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        current = &mut func.block.stmts;
                    }
                }
            } else {
                return;
            }
        }
        let last_idx = self.path.last().unwrap();
        if let Item::Fn(item_fn) = &mut current[*last_idx] {
            if let Some(stmt) = item_fn.block.stmts.get_mut(0) {
                if let Stmt::Expr(expr, _) = stmt {
                    if let Expr::Call(call) = expr {
                        call.args = Punctuated::from_iter(vec![self.new_arg.clone()]);
                    }
                }
            }
        }
    }
}

struct TransmuteVisitor {
    replacements: Vec<Replacement>,
}

impl Visit<'_> for TransmuteVisitor {
    fn visit_expr_call(&mut self, call: &ExprCall) {
        if let Expr::Path(path) = &*call.func {
            if path.path.segments.last().map(|seg| seg.ident.to_string()) == Some("transmute".to_string()) {
                let mut path_segments = Vec::new();
                for segment in &path.path.segments {
                    path_segments.push(segment.ident.to_string());
                }
                if path_segments.contains(&"std".to_string()) && path_segments.contains(&"mem".to_string()) {
                    let new_arg: Expr = parse_quote! {
                        unsafe {
                            |param: _| {}
                        }
                    };
                    self.replacements.push(Replacement {
                        path: Vec::new(),
                        new_arg,
                    });
                }
            }
        }
        syn::visit::visit_expr_call(self, call);
    }
}
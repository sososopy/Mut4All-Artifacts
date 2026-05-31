use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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
    visit_mut::{VisitMut, visit_expr_mut},
    *,
};

use crate::mutator::Mutator;

pub struct Append_Await_To_Function_Call_229;

impl Mutator for Append_Await_To_Function_Call_229 {
    fn name(&self) -> &str {
        "Append_Await_To_Function_Call_229"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = FunctionCallVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation appends `.await{}` to function calls, introducing incorrect async-await syntax to test the parser's robustness and potentially trigger compiler bugs related to async-await handling."
    }
}

struct FunctionCallVisitor;

impl VisitMut for FunctionCallVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        let await_expr: Expr = parse_quote! { #node.await{} };
        *node = match await_expr {
            Expr::Call(call) => call,
            _ => return,
        };
        visit_expr_mut(self, &mut Expr::Call(node.clone()));
    }
}
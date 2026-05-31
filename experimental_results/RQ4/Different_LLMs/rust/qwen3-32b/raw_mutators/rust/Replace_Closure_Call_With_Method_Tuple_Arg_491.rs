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

pub struct Replace_Closure_Call_With_Method_Tuple_Arg_491;

impl Mutator for Replace_Closure_Call_With_Method_Tuple_Arg_491 {
    fn name(&self) -> &str {
        "Replace_Closure_Call_With_Method_Tuple_Arg_491"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureCallToMockVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms closure invocations into method calls on a mock object, wrapping arguments in a tuple. This tests the compiler's handling of unboxed closure arguments and tuple expectations in MIR and inlining logic by enforcing explicit tuple construction and method call semantics."
    }
}

struct ClosureCallToMockVisitor;

impl VisitMut for ClosureCallToMockVisitor {
    fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
        if let Expr::Closure(_) = &*expr_call.func {
            let args = &expr_call.args;
            if args.len() == 1 {
                if let Expr::Tuple(_) = &**args.first().unwrap() {
                    let arg = args.first().unwrap().clone();
                    let new_expr = parse_quote! { mock.call_mut(#arg) };
                    *expr_call = new_expr;
                } else {
                    let tuple_expr = ExprTuple {
                        attrs: Vec::new(),
                        paren_token: Default::default(),
                        elems: args.clone(),
                    };
                    let new_expr = parse_quote! { mock.call_mut(#tuple_expr) };
                    *expr_call = new_expr;
                }
            } else {
                let tuple_expr = ExprTuple {
                    attrs: Vec::new(),
                    paren_token: Default::default(),
                    elems: args.clone(),
                };
                let new_expr = parse_quote! { mock.call_mut(#tuple_expr) };
                *expr_call = new_expr;
            }
        } else {
            syn::visit_mut::visit_expr_call_mut(self, expr_call);
        }
    }
}
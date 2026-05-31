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
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Function_Call_Async_Await_351;

impl Mutator for Function_Call_Async_Await_351 {
    fn name(&self) -> &str {
        "Function_Call_Async_Await_351"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct CallVisitor;

        impl VisitMut for CallVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Call(call) = expr {
                    let new_expr: Expr = parse_quote! {
                        #call.await{}
                    };
                    *expr = new_expr;
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = CallVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function calls by appending .await{} to them, regardless of their async nature. The intent is to introduce an erroneous usage pattern for await expressions, testing the parser's robustness and potentially uncovering compiler bugs related to async-await syntax handling."
    }
}
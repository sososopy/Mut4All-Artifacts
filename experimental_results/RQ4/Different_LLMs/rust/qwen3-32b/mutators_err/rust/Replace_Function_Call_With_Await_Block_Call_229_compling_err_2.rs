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

pub struct Replace_Function_Call_With_Await_Block_Call_229;

impl Mutator for Replace_Function_Call_With_Await_Block_Call_229 {
    fn name(&self) -> &str {
        "Replace_Function_Call_With_Await_Block_Call_229"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceFunctionCallsVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces valid function calls with an invalid async-await expression followed by a call. This transformation injects an `await { ... }()` pattern, where the block contains the original function's path. The structure is syntactically invalid due to the misplaced `await` and block, creating a malformed expression that still attempts to call the original function. This tests the compiler's ability to handle malformed async-await syntax and invalid function call structures."
    }
}

struct ReplaceFunctionCallsVisitor;

impl VisitMut for ReplaceFunctionCallsVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(call) = expr {
            let func = call.func.clone();
            let args = call.args.clone();
            
            let block_tokens = quote! { { #func } };
            let block_expr = syn::parse2::<ExprBlock>(block_tokens).unwrap();
            
            let await_expr = ExprAwait {
                attrs: Vec::new(),
                base: Box::new(Expr::Block(block_expr)),
                await_token: syn::token::Await::new(Span::call_site()),
            };
            
            let new_call = ExprCall {
                attrs: Vec::new(),
                func: Box::new(Expr::Await(await_expr)),
                args,
                paren_token: Default::default(),
            };
            
            *expr = syn::Expr::Call(new_call);
        } else {
            syn::visit_mut::visit_expr_mut(self, expr);
        }
    }
}
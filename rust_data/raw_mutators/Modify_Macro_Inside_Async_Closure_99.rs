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

pub struct Modify_Macro_Inside_Async_Closure_99;

impl Mutator for Modify_Macro_Inside_Async_Closure_99 {
    fn name(&self) -> &str {
        "Modify_Macro_Inside_Async_Closure_99"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                let mut visitor = AsyncClosureVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets async closures and modifies macro calls within them. By introducing an undefined variable in the macro call, it triggers potential macro expansion errors. This can help identify issues in macro resolution and error handling within async contexts."
    }
}

struct AsyncClosureVisitor;

impl VisitMut for AsyncClosureVisitor {
    fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
        if node.asyncness.is_some() {
            if let Expr::Block(ref mut block) = *node.body {
                for stmt in &mut block.block.stmts {
                    if let Stmt::Semi(Expr::Macro(ref mut mac), _) = stmt {
                        let macro_name = mac.mac.path.segments.last().unwrap().ident.to_string();
                        if macro_name == "println" {
                            let new_args: Expr = parse_quote!({ "{:?}", undefined_var });
                            mac.mac.tokens = new_args.into_token_stream();
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_closure_mut(self, node);
    }
}
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

pub struct Macro_Argument_Type_Mismatch_206;

impl Mutator for Macro_Argument_Type_Mismatch_206 {
    fn name(&self) -> &str {
        "Macro_Argument_Type_Mismatch_206"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ExpressionCollector {
            expressions: Vec<Expr>,
        }

        impl ExpressionCollector {
            fn new() -> Self {
                ExpressionCollector { expressions: Vec::new() }
            }
        }

        impl Visit for ExpressionCollector {
            fn visit_expr(&mut self, expr: &Expr) {
                self.expressions.push(expr.clone());
                syn::visit::visit_expr(self, expr);
            }
        }

        let mut collector = ExpressionCollector::new();
        collector.visit_file(file);
        let expressions = collector.expressions;

        if expressions.is_empty() {
            return;
        }

        for item in &mut file.items {
            if let syn::Item::Macro(macro_item) = item {
                let macro_tokens = macro_item.mac.tokens.clone();
                if let Ok(_) = syn::parse2::<SynPath>(macro_tokens.clone()) {
                    let expr = expressions.choose(&mut thread_rng()).unwrap().clone();
                    let block_tokens = quote! {
                        {
                            #expr
                        }
                    };
                    macro_item.mac.tokens = block_tokens.into_token_stream();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macros that expect a `path` argument and replaces them with a block `{ ... }` expression. This forces the compiler to handle non-path arguments during macro expansion, potentially exposing validation errors in macro argument parsing and path resolution logic."
    }
}
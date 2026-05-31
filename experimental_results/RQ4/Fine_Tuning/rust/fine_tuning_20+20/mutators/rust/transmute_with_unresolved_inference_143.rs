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

pub struct Transmute_With_Unresolved_Inference_143;

impl Mutator for Transmute_With_Unresolved_Inference_143 {
    fn name(&self) -> &str {
        "Transmute_With_Unresolved_Inference_143"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TransmuteFinder {
            transmute_args: Vec<Expr>,
        }
        impl<'ast> Visit<'ast> for TransmuteFinder {
            fn visit_expr_call(&mut self, i: &'ast ExprCall) {
                if let Expr::Path(ExprPath { path, .. }) = &*i.func {
                    if path.is_ident("transmute") {
                        if let Some(arg) = i.args.first() {
                            self.transmute_args.push(arg.clone());
                        }
                    }
                }
                syn::visit::visit_expr_call(self, i);
            }
        }
        struct TransmuteReplacer {
            target_args: Vec<Expr>,
        }
        impl VisitMut for TransmuteReplacer {
            fn visit_expr_call_mut(&mut self, i: &mut ExprCall) {
                if let Expr::Path(ExprPath { path, .. }) = &*i.func {
                    if path.is_ident("transmute") {
                        if let Some(arg) = i.args.first_mut() {
                            for target in &self.target_args {
                                if arg == target {
                                    *arg = parse_quote!(|o_b: Option<_>| {});
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, i);
            }
        }
        let mut finder = TransmuteFinder {
            transmute_args: Vec::new(),
        };
        finder.visit_file(file);
        let mut replacer = TransmuteReplacer {
            target_args: finder.transmute_args,
        };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets `transmute` calls with unresolved type inference arguments, replacing them with a closure that captures an `Option<_>` parameter. It exploits type inference ambiguities to stress the compiler's resolution mechanisms, potentially triggering ICEs related to inference and transmute operations."
    }
}
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

pub struct AsyncClosureWithMacroExpansion_208;

impl Mutator for AsyncClosureWithMacroExpansion_208 {
    fn name(&self) -> &str {
        "AsyncClosureWithMacroExpansion_208"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureVisitor;
        impl VisitMut for ClosureVisitor {
            fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
                for arg in &mut call.args {
                    if let Expr::Closure(closure) = arg {
                        let mut macro_args: Punctuated<Expr, Comma> = Punctuated::new();
                        for param in &closure.inputs {
                            if let Pat::Ident(ident) = param {
                                let expr = Expr::Path(ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: ident.ident.clone().into(),
                                });
                                macro_args.push(expr);
                            }
                        }
                        let macro_expr = parse_quote!(macro!(#macro_args));
                        let new_closure = parse_quote! {
                            async move |#closure.inputs| #macro_expr
                        };
                        *arg = new_closure;
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, call);
            }
        }
        let mut visitor = ClosureVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces closure arguments with async closures containing a macro invocation that uses the closure's parameters. This stresses the resolver by combining async syntax with macro expansion that consumes closure parameters, potentially exposing resolver issues in complex async-macro interactions."
    }
}
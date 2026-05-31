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

pub struct Add_Nested_Async_Composition_Layer_303;

impl Mutator for Add_Nested_Async_Composition_Layer_303 {
    fn name(&self) -> &str {
        "Add_Nested_Async_Composition_Layer_303"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct NestedAsyncVisitor;
        impl<'ast> VisitMut for NestedAsyncVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::MethodCall(method_call) = expr {
                    if method_call.method == "and_then" {
                        let new_call = ExprMethodCall {
                            attrs: vec![],
                            receiver: Box::new(Expr::MethodCall(method_call.clone())),
                            method: method_call.method.clone(),
                            turbofish: None,
                            args: method_call.args.clone(),
                            dot_token: method_call.dot_token.clone(),
                            paren_token: method_call.paren_token.clone(),
                        };
                        *expr = Expr::MethodCall(new_call);
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        let mut visitor = NestedAsyncVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator increases the nesting depth of async composition chains by inserting an additional `and_then` call into existing method call sequences. This transformation creates redundant composition layers with identical return types, potentially leading to type metadata duplication in the TypeMap and stressing the compiler's handling of nested async trait method chains."
    }
}
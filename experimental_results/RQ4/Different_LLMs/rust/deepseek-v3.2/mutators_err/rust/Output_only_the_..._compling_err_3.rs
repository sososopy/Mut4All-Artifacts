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

pub struct Insert_Type_Ascription_In_Path_232;

impl Mutator for Insert_Type_Ascription_In_Path_232 {
    fn name(&self) -> &str {
        "Insert_Type_Ascription_In_Path_232"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut dummy_inserted = false;
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == "Dummy" {
                    dummy_inserted = true;
                }
            }
        }
        if !dummy_inserted {
            let dummy_struct: ItemStruct = parse_quote! { struct Dummy; };
            file.items.insert(0, Item::Struct(dummy_struct));
        }

        let mut visitor = PathMutatorVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets path segments in expression contexts where a double colon is immediately followed by a parenthesized list, e.g., `Path::(a, b)`. It inserts a colon and a type that is already in scope (or a dummy type if none exists) followed by the never type `!` between the `::` and the opening parenthesis, resulting in a malformed type ascription like `Path::(TypeName: !)(a, b)`. This transformation aims to trigger parser error handling edge cases by creating syntactically valid but semantically odd constructs. The mutator must first locate suitable patterns in the AST, then find an existing type identifier in the current scope, or introduce a dummy type `struct Dummy;` at an appropriate place (e.g., module scope) to ensure the mutation is syntactically valid. The goal is to maximize coverage of the seed program's code by targeting path segments in expression contexts, such as function call arguments, struct literals, or similar. The mutator should be designed to handle various edge cases, such as nested paths, generic arguments, and lifetime parameters, to ensure robust mutation effects across different Rust codebases."
    }
}

struct PathMutatorVisitor;

impl PathMutatorVisitor {
    fn new() -> Self {
        PathMutatorVisitor
    }
}

impl VisitMut for PathMutatorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(call) = expr {
            if let Expr::Path(path_expr) = &*call.func {
                if let Some(last_segment) = path_expr.path.segments.last() {
                    if last_segment.arguments.is_empty() {
                        let mut new_segments = path_expr.path.segments.clone();
                        let last_segment = new_segments.pop().unwrap();
                        let new_last_segment = syn::PathSegment {
                            ident: last_segment.ident.clone(),
                            arguments: syn::PathArguments::Parenthesized(
                                syn::ParenthesizedGenericArguments {
                                    paren_token: syn::token::Paren::default(),
                                    inputs: parse_quote!((Dummy: !)),
                                    output: ReturnType::Default,
                                }
                            ),
                        };
                        new_segments.push(new_last_segment);
                        let new_path = syn::ExprPath {
                            attrs: path_expr.attrs.clone(),
                            qself: None,
                            path: syn::Path {
                                segments: new_segments,
                                leading_colon: path_expr.path.leading_colon,
                            },
                        };
                        call.func = Box::new(Expr::Path(new_path));
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}
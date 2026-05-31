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

pub struct Mutate_Box_NewIn_With_Mutable_Reference_1;

impl Mutator for Mutate_Box_NewIn_With_Mutable_Reference_1 {
    fn name(&self) -> &str {
        "Mutate_Box_NewIn_With_Mutable_Reference_1"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = BoxNewInVisitor::default();
                visitor.visit_item_fn(func);
                if visitor.has_box_new_in {
                    let data_ident = Ident::new("data", Span::call_site());
                    let data_stmt: Stmt = parse_quote! {
                        let #data_ident = &mut [0, 1];
                    };
                    func.block.stmts.insert(0, data_stmt);
                    for stmt in &mut func.block.stmts {
                        if let syn::Stmt::Expr(expr, _) = stmt {
                            if let syn::Expr::Call(expr_call) = expr {
                                if let syn::Expr::Path(expr_path) = &*expr_call.func {
                                    if expr_path.path.segments.len() == 3
                                        && expr_path.path.segments[0].ident == "Box"
                                        && expr_path.path.segments[1].ident == "new_in"
                                    {
                                        if expr_call.args.len() == 2 {
                                            expr_call.args[0] =
                                                syn::Expr::Reference(syn::ExprReference {
                                                    attrs: Vec::new(),
                                                    and_token: token::And::default(),
                                                    raw: Default::default(),
                                                    mutability: Some(token::Mut::default()),
                                                    expr: Box::new(syn::Expr::Path(
                                                        syn::ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: syn::Path::from(data_ident),
                                                        },
                                                    )),
                                                });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function calls using `Box::new_in` with an allocator argument. It introduces a mutable reference to the data being boxed by injecting a `let data = &mut [0, 1];` statement and modifying the function call to use this mutable reference. This transformation simulates potential allocator mismanagement and aims to uncover bugs related to memory allocation and reference handling in the Rust compiler."
    }
}

#[derive(Default)]
struct BoxNewInVisitor {
    has_box_new_in: bool,
}

impl<'ast> Visit<'ast> for BoxNewInVisitor {
    fn visit_expr_call(&mut self, node: &'ast ExprCall) {
        if let Expr::Path(expr_path) = &*node.func {
            if expr_path.path.segments.len() == 3
                && expr_path.path.segments[0].ident == "Box"
                && expr_path.path.segments[1].ident == "new_in"
            {
                if node.args.len() == 2 {
                    self.has_box_new_in = true;
                }
            }
        }
        syn::visit::visit_expr_call(self, node);
    }
}
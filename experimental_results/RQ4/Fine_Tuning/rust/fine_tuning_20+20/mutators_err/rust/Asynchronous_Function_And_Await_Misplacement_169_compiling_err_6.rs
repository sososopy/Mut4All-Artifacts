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

pub struct Asynchronous_Function_And_Await_Misplacement_169;

impl Mutator for Asynchronous_Function_And_Await_Misplacement_169 {
    fn name(&self) -> &str {
        "Asynchronous_Function_And_Await_Misplacement_169"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut async_fns = HashSet::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    async_fns.insert(item_fn.sig.ident.to_string());
                }
            }
        }
        struct CallVisitor {
            async_fns: HashSet<String>,
            calls: Vec<(usize, usize, String)>,
        }
        impl<'ast> Visit<'ast> for CallVisitor {
            fn visit_expr_call(&mut self, node: &'ast ExprCall) {
                if let Expr::Path(ExprPath { path, .. }) = &*node.func {
                    if path.segments.len() == 1 {
                        let ident = &path.segments.first().unwrap().ident;
                        if self.async_fns.contains(&ident.to_string()) {
                            self.calls.push((node.span().start().line, node.span().start().column, ident.to_string()));
                        }
                    }
                }
                syn::visit::visit_expr_call(self, node);
            }
        }
        let mut visitor = CallVisitor {
            async_fns,
            calls: Vec::new(),
        };
        visitor.visit_file(file);
        let mut rng = thread_rng();
        visitor.calls.shuffle(&mut rng);
        let mut misplaced = false;
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some((call_line, call_col, call_ident)) = visitor.calls.first() {
                    if item_fn.block.brace_token.span.open().line <= *call_line
                        && item_fn.block.brace_token.span.close().line >= *call_line
                    {
                        let mut await_pos = None;
                        for (i, stmt) in item_fn.block.stmts.iter().enumerate() {
                            if stmt.span().start().line == *call_line {
                                if let Stmt::Expr(expr, _) = stmt {
                                    if let Expr::Await(_) = expr {
                                        await_pos = Some(i);
                                    }
                                }
                            }
                        }
                        if let Some(pos) = await_pos {
                            item_fn.block.stmts.remove(pos);
                            let mut new_stmts = Vec::new();
                            for stmt in &item_fn.block.stmts {
                                new_stmts.push(stmt.clone());
                                if stmt.span().start().line == *call_line {
                                    if let Stmt::Expr(expr, semi) = stmt {
                                        if let Expr::Call(expr_call) = expr {
                                            if let Expr::Path(ExprPath { path, .. }) =
                                                &*expr_call.func
                                            {
                                                if path.segments.len() == 1 {
                                                    let ident =
                                                        &path.segments.first().unwrap().ident;
                                                    if ident == call_ident {
                                                        new_stmts.pop();
                                                        let new_call = Expr::Call(expr_call.clone());
                                                        new_stmts.push(Stmt::Expr(new_call, *semi));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            item_fn.block.stmts = new_stmts;
                            let mut misplaced = false;
                            for stmt in &mut item_fn.block.stmts {
                                if stmt.span().start().line == *call_line {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Await(_) = expr {
                                            misplaced = true;
                                        }
                                    }
                                }
                            }
                            if misplaced {
                                continue;
                            }
                            for stmt in &mut item_fn.block.stmts {
                                if stmt.span().start().line == *call_line {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(expr_call) = expr {
                                            if let Expr::Path(ExprPath { path, .. }) =
                                                &*expr_call.func
                                            {
                                                if path.segments.len() == 1 {
                                                    let ident =
                                                        &path.segments.first().unwrap().ident;
                                                    if ident == call_ident {
                                                        *stmt = Stmt::Expr(
                                                            Expr::Call(expr_call.clone()),
                                                            None,
                                                        );
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            let mut new_stmts = Vec::new();
                            for stmt in &item_fn.block.stmts {
                                new_stmts.push(stmt.clone());
                                if stmt.span().start().line == *call_line {
                                    new_stmts.push(Stmt::Expr(
                                        Expr::Await(ExprAwait {
                                            attrs: Vec::new(),
                                            base: Box::new(Expr::Path(ExprPath {
                                                attrs: Vec::new(),
                                                qself: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: {
                                                        let mut segments = Punctuated::new();
                                                        segments.push(PathSegment {
                                                            ident: Ident::new(
                                                                "initial_exchange",
                                                                Span::call_site(),
                                                            ),
                                                            arguments:
                                                                PathArguments::None,
                                                        });
                                                        segments
                                                    },
                                                },
                                            })),
                                            dot_token: token::Dot::default(),
                                            await_token: token::Await::default(),
                                        }),
                                        None,
                                    ));
                                }
                            }
                            item_fn.block.stmts = new_stmts;
                            misplaced = true;
                        }
                    }
                }
            }
        }
        if misplaced {
            return;
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some((call_line, call_col, call_ident)) = visitor.calls.first() {
                    if item_fn.block.brace_token.span.open().line <= *call_line
                        && item_fn.block.brace_token.span.close().line >= *call_line
                    {
                        let mut await_pos = None;
                        for (i, stmt) in item_fn.block.stmts.iter().enumerate() {
                            if stmt.span().start().line == *call_line {
                                if let Stmt::Expr(expr, _) = stmt {
                                    if let Expr::Await(_) = expr {
                                        await_pos = Some(i);
                                    }
                                }
                            }
                        }
                        if let Some(pos) = await_pos {
                            item_fn.block.stmts.remove(pos);
                            let mut new_stmts = Vec::new();
                            for stmt in &item_fn.block.stmts {
                                new_stmts.push(stmt.clone());
                                if stmt.span().start().line == *call_line {
                                    if let Stmt::Expr(expr, semi) = stmt {
                                        if let Expr::Call(expr_call) = expr {
                                            if let Expr::Path(ExprPath { path, .. }) =
                                                &*expr_call.func
                                            {
                                                if path.segments.len() == 1 {
                                                    let ident =
                                                        &path.segments.first().unwrap().ident;
                                                    if ident == call_ident {
                                                        new_stmts.pop();
                                                        let new_call = Expr::Call(expr_call.clone());
                                                        new_stmts.push(Stmt::Expr(new_call, *semi));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            item_fn.block.stmts = new_stmts;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets asynchronous function calls by removing the .await keyword from the call and relocating it to a non-async context within the same function. It identifies async function calls, strips the await, and repositions it after a non-async expression, such as a Result, to create a syntactically invalid await usage. This transformation aims to test the compiler's handling of misplaced await expressions and the robustness of its async-await desugaring process."
    }
}
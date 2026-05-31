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

pub struct Alter_Macro_Path_With_Block_205;

impl Mutator for Alter_Macro_Path_With_Block_205 {
    fn name(&self) -> &str {
        "Alter_Macro_Path_With_Block_205"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut macro_definitions = HashSet::new();
        for item in &file.items {
            if let Item::Macro(mac) = item {
                macro_definitions.insert(mac.mac.path.segments[0].ident.to_string());
            }
        }
        let mut visitor = MacroPathVisitor {
            macro_definitions,
            candidates: Vec::new(),
        };
        visitor.visit_file_mut(file);
        let mut rng = thread_rng();
        let mut expr_visitor = ExpressionCollector {
            expressions: Vec::new(),
        };
        expr_visitor.visit_file(file);
        let expr_candidates = expr_visitor.expressions;
        let mut stmt_visitor = StatementCollector {
            statements: Vec::new(),
        };
        stmt_visitor.visit_file(file);
        let stmt_candidates = stmt_visitor.statements;
        for (index, candidate) in visitor.candidates.iter().enumerate() {
            if index == 0 {
                let mut new_args = Vec::new();
                for (arg_index, arg) in candidate.args.iter().enumerate() {
                    if arg_index == candidate.path_index {
                        let new_arg = if let Some(expr) = expr_candidates.choose(&mut rng) {
                            let expr_str = expr.to_token_stream().to_string();
                            parse_quote! { { #expr_str } }
                        } else if let Some(stmt) = stmt_candidates.choose(&mut rng) {
                            let stmt_str = stmt.to_token_stream().to_string();
                            parse_quote! { { #stmt_str } }
                        } else {
                            parse_quote! { { 42 } }
                        };
                        new_args.push(new_arg);
                    } else {
                        new_args.push(arg.clone());
                    }
                }
                let new_macro = Macro {
                    path: candidate.path.clone(),
                    bang_token: candidate.bang_token,
                    delimiter: candidate.delimiter.clone(),
                    tokens: new_args.into_iter().collect(),
                };
                let new_item = Item::Macro(ItemMacro {
                    attrs: candidate.attrs.clone(),
                    ident: None,
                    mac: new_macro,
                    semi_token: candidate.semi_token,
                });
                *candidate.item = new_item;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macro invocations with path arguments, which are defined using `macro_rules!` and have a `$attr_path: path` pattern. It transforms the path argument into a block expression by embedding it within `{}` and injecting additional code inside the block. The injected code is selected from existing expressions or statements in the file, ensuring that it uses available variables, functions, or types. This mutation leverages the flexibility of Rust's macro system to create complex, non-standard path expressions that challenge the compiler's macro expansion and path resolution logic, aiming to uncover subtle bugs related to macro hygiene, scope management, and syntactic ambiguity."
    }
}

struct MacroPathCandidate<'a> {
    item: &'a mut syn::Item,
    attrs: Vec<Attribute>,
    path: SynPath,
    bang_token: syn::token::Bang,
    delimiter: syn::MacroDelimiter,
    args: Vec<proc_macro2::TokenStream>,
    path_index: usize,
    semi_token: Option<syn::token::Semi>,
}

struct MacroPathVisitor<'a> {
    macro_definitions: HashSet<String>,
    candidates: Vec<MacroPathCandidate<'a>>,
}

impl<'a> VisitMut for MacroPathVisitor<'a> {
    fn visit_item_macro_mut(&mut self, node: &mut ItemMacro) {
        let macro_name = node.mac.path.segments[0].ident.to_string();
        if !self.macro_definitions.contains(&macro_name) {
            return;
        }
        let mut args = Vec::new();
        let mut path_index = None;
        for (i, arg) in node.mac.tokens.clone().into_iter().enumerate() {
            if let Ok(path) = syn::parse2::<SynPath>(arg.clone().into()) {
                args.push(arg.into());
                if path_index.is_none() {
                    path_index = Some(i);
                }
            } else {
                args.push(arg.into());
            }
        }
        if let Some(path_index) = path_index {
            self.candidates.push(MacroPathCandidate {
                item: unsafe { &mut *(node as *mut ItemMacro as *mut Item) },
                attrs: node.attrs.clone(),
                path: node.mac.path.clone(),
                bang_token: node.mac.bang_token,
                delimiter: node.mac.delimiter.clone(),
                args,
                path_index,
                semi_token: node.semi_token,
            });
        }
    }
}

struct ExpressionCollector {
    expressions: Vec<Expr>,
}

impl<'ast> Visit<'ast> for ExpressionCollector {
    fn visit_expr(&mut self, node: &'ast Expr) {
        self.expressions.push(node.clone());
    }
}

struct StatementCollector {
    statements: Vec<Stmt>,
}

impl<'ast> Visit<'ast> for StatementCollector {
    fn visit_stmt(&mut self, node: &'ast Stmt) {
        self.statements.push(node.clone());
    }
}
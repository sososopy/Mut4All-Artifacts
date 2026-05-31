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

pub struct Replace_Macro_Path_Argument_With_Block_206;

impl Mutator for Replace_Macro_Path_Argument_With_Block_206 {
    fn name(&self) -> &str {
        "Replace_Macro_Path_Argument_With_Block_206"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                let mut visitor = MacroPathVisitor {
                    paths: Vec::new(),
                };
                visitor.visit_item_macro(&item_macro);
                if visitor.paths.is_empty() {
                    continue;
                }
                let target_path = visitor.paths[0];
                let new_block = syn::ExprBlock {
                    attrs: Vec::new(),
                    label: None,
                    block: syn::Block {
                        brace_token: syn::token::Brace::default(),
                        stmts: vec![
                            syn::Stmt::Local(syn::Local {
                                attrs: Vec::new(),
                                let_token: syn::token::Let::default(),
                                pat: syn::Pat::Ident(syn::PatIdent {
                                    attrs: Vec::new(),
                                    by_ref: None,
                                    mutability: None,
                                    ident: syn::Ident::new("x", Span::call_site()),
                                    subpat: None,
                                }),
                                init: Some(syn::LocalInit {
                                    eq_token: syn::token::Eq::default(),
                                    expr: syn::Expr::Lit(syn::ExprLit {
                                        attrs: Vec::new(),
                                        lit: syn::Lit::Int(syn::LitInt::new("1", Span::call_site())),
                                    }),
                                }),
                                semi_token: syn::token::Semi::default(),
                            }),
                            syn::Stmt::Expr(syn::Expr::Macro(syn::ExprMacro {
                                attrs: Vec::new(),
                                mac: syn::Macro {
                                    path: syn::Path::from(syn::Ident::new("println", Span::call_site())),
                                    bang_token: syn::token::Bang::default(),
                                    delimiter: syn::MacroDelimiter::Paren(syn::token::Paren::default()),
                                    tokens: proc_macro2::TokenStream::from(quote::quote! { "{}", x }),
                                },
                            }), None),
                            syn::Stmt::Expr(syn::Expr::Path(syn::ExprPath {
                                attrs: Vec::new(),
                                qself: None,
                                path: target_path,
                            }), None),
                        ],
                    },
                };
                let new_arg = syn::Expr::Block(new_block);
                let mut replacer = MacroPathReplacer {
                    target_path: target_path,
                    new_arg: new_arg,
                };
                replacer.visit_item_macro_mut(item_macro);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations where a path argument is expected (e.g., in attribute contexts). It replaces the path with a block expression containing arbitrary statements and a trailing path expression, mimicking the bug report's syntax. This transformation stresses the resolver's handling of non-canonical path fragments embedded in blocks, potentially triggering resolution panics due to missing entries in internal maps."
    }
}

struct MacroPathVisitor {
    paths: Vec<syn::Path>,
}

impl Visit<'_> for MacroPathVisitor {
    fn visit_expr(&mut self, expr: &syn::Expr) {
        if let syn::Expr::Path(expr_path) = expr {
            self.paths.push(expr_path.path.clone());
        }
        syn::visit::visit_expr(self, expr);
    }
}

struct MacroPathReplacer {
    target_path: syn::Path,
    new_arg: syn::Expr,
}

impl VisitMut for MacroPathReplacer {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Path(expr_path) = expr {
            if expr_path.path == self.target_path {
                *expr = self.new_arg.clone();
                return;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}
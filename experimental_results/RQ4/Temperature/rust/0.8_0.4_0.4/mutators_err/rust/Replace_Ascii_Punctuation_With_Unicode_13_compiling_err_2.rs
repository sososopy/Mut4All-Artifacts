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
    token::{Paren, Plus, Bracket, Brace},
    visit::Visit,
    visit_mut::VisitMut,
    ImplItem, *,
};

use crate::mutator::Mutator;

pub struct Replace_Ascii_Punctuation_With_Unicode_13;

impl Mutator for Replace_Ascii_Punctuation_With_Unicode_13 {
    fn name(&self) -> &str {
        "Replace_Ascii_Punctuation_With_Unicode_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct PunctuationReplacer;

        impl VisitMut for PunctuationReplacer {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                match expr {
                    Expr::Paren(expr_paren) => {
                        expr_paren.paren_token = token::Paren {
                            span: Span::call_site().into(),
                        };
                        self.visit_expr_mut(&mut *expr_paren.expr);
                    }
                    Expr::Index(expr_index) => {
                        expr_index.bracket_token = token::Bracket {
                            span: Span::call_site().into(),
                        };
                        self.visit_expr_mut(&mut *expr_index.expr);
                        self.visit_expr_mut(&mut *expr_index.index);
                    }
                    Expr::Block(expr_block) => {
                        expr_block.block.brace_token = token::Brace {
                            span: Span::call_site().into(),
                        };
                        for stmt in &mut expr_block.block.stmts {
                            self.visit_stmt_mut(stmt);
                        }
                    }
                    _ => syn::visit_mut::visit_expr_mut(self, expr),
                }
            }
        }

        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    PunctuationReplacer.visit_block_mut(&mut item_fn.block);
                }
                Item::Impl(item_impl) => {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            PunctuationReplacer.visit_block_mut(&mut method.block);
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces ASCII punctuation marks like parentheses, brackets, and braces with their visually similar Unicode counterparts. This transformation aims to test the compiler's handling of Unicode characters in syntactic constructs, potentially revealing issues in tokenization and parsing when ASCII and Unicode characters are used interchangeably."
    }
}
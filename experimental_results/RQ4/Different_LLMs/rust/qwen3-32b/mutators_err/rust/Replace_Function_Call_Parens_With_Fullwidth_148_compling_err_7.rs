use proc_macro2::{Span, *};
use proc_macro2::DelimSpan;
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

pub struct Replace_Function_Call_Parens_With_Fullwidth_148;

impl Mutator for Replace_Function_Call_Parens_With_Fullwidth_148 {
    fn name(&self) -> &str {
        "Replace_Function_Call_Parens_With_Fullwidth_148"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceParensVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces standard parentheses in function calls with visually similar fullwidth Unicode counterparts (‘（’ and ‘）’). This transformation corrupts syntax while preserving structural validity, challenging the compiler's lexical analysis and parser to distinguish between valid and visually deceptive syntax patterns."
    }
}

struct ReplaceParensVisitor;

impl VisitMut for ReplaceParensVisitor {
    fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
        let open = proc_macro2::Punct::new('（', proc_macro2::Spacing::Alone);
        let close = proc_macro2::Punct::new('）', proc_macro2::Spacing::Alone);
        let new_paren = token::Paren {
            span: DelimSpan::call_site(),
        };
        call.paren_token = new_paren;
        self.visit_expr_mut(&mut call.func);
        for arg in &mut call.args {
            self.visit_expr_mut(arg);
        }
    }
}
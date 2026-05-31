use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprMacro, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Asm_With_Global_Local_172;

impl Mutator for Replace_Asm_With_Global_Local_172 {
    fn name(&self) -> &str {
        "Replace_Asm_With_Global_Local_172"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AsmMacroVisitor;
        impl<'ast> VisitMut for AsmMacroVisitor {
            fn visit_expr_macro_mut(&mut self, expr: &mut ExprMacro) {
                if let Some(last) = expr.mac.path.segments.last() {
                    if last.ident == "asm" && expr.mac.path.segments.len() >= 3 {
                        let core = &expr.mac.path.segments[0];
                        let arch = &expr.mac.path.segments[1];
                        if core.ident == "core" && arch.ident == "arch" {
                            let macro_str = expr.mac.tokens.to_string();
                            let mut new_macro_str = macro_str.clone();
                            let re = Regex::new(r#""([^"]*)""#).unwrap();
                            if let Some(captures) = re.find(&new_macro_str) {
                                let first_str = captures.as_str().to_string();
                                let start = captures.start();
                                let end = captures.end();
                                new_macro_str.replace_range(start..end, r#""global.get 0""#);
                                new_macro_str.insert_str(end, r#", "local.set {0}"#);
                                if let Some(out_pos) = new_macro_str.find("out(reg)") {
                                    new_macro_str.replace_range(out_pos..out_pos + "out(reg)".len(), "out(local)");
                                }
                            }
                            let new_tokens = new_macro_str.parse::<proc_macro2::TokenStream>().unwrap();
                            expr.mac.tokens = new_tokens;
                        }
                    }
                }
                syn::visit_mut::visit_expr_macro_mut(self, expr);
            }
        }
        let mut visitor = AsmMacroVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
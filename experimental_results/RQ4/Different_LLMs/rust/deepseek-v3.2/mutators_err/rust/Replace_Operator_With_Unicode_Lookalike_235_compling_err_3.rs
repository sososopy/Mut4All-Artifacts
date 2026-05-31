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

pub struct Replace_Operator_With_Unicode_Lookalike_235;

impl Mutator for Replace_Operator_With_Unicode_Lookalike_235 {
    fn name(&self) -> &str {
        "Replace_Operator_With_Unicode_Lookalike_235"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let operator_replacements = vec![
            ("=", vec!["⩵", "≔", "≣", "﹦", "＝"]),
            ("==", vec!["⩵", "≣", "≡", "≐", "≑"]),
            ("!=", vec!["≠", "≭", "≢", "≦", "≧"]),
            ("<=", vec!["≤", "≦", "≲", "⩽", "⪅"]),
            (">=", vec!["≥", "≧", "≳", "⩾", "⪆"]),
            ("+", vec!["＋", "⧺", "⨁", "⨄", "⨃"]),
            ("-", vec!["－", "−", "⁻", "₋", "⨩"]),
            ("*", vec!["∗", "✱", "⨉", "⨯", "⨰"]),
            ("/", vec!["∕", "⧸", "÷", "⨼", "⨽"]),
            ("%", vec!["％", "٪", "⁒", "⨹", "⨺"]),
            ("&", vec!["＆", "∧", "⩓", "⩔", "⩕"]),
            ("|", vec!["｜", "∨", "⩖", "⩗", "⩘"]),
            ("^", vec!["＾", "∧", "⩘", "⩚", "⩛"]),
            ("!", vec!["！", "ǃ", "ⵑ", "ⵒ", "ⵓ"]),
            ("~", vec!["～", "˜", "⁓", "∽", "⸞"]),
            ("<", vec!["＜", "‹", "≪", "⪡", "⩹"]),
            (">", vec!["＞", "›", "≫", "⪢", "⩺"]),
            ("?", vec!["？", "¿", "⸮", "⳺", "⳻"]),
            (".", vec!["．", "․", "‧", "⋅", "⸱"]),
            (":", vec!["：", "ː", "˸", "։", "׃"]),
            (";", vec!["；", "⁏", "⨾", "⸵", "⸼"]),
            (",", vec!["，", "‚", "⸲", "⸴", "⹁"]),
            ("@", vec!["＠", "﹫", "⨷", "⨸", "⨹"]),
            ("#", vec!["＃", "⌗", "ⵌ", "ⵍ", "ⵎ"]),
            ("$", vec!["＄", "﹩", "ⵏ", "ⵐ", "ⵑ"]),
            ("(", vec!["（", "❨", "⦅", "⦇", "⦗"]),
            (")", vec!["）", "❩", "⦆", "⦈", "⦘"]),
            ("[", vec!["［", "❲", "⦋", "⦍", "⦏"]),
            ("]", vec!["］", "❳", "⦌", "⦎", "⦐"]),
            ("{", vec!["｛", "❴", "⦃", "⦅", "⦇"]),
            ("}", vec!["｝", "❵", "⦄", "⦆", "⦈"]),
        ];
        let mut candidates = Vec::new();
        struct OperatorVisitor<'a> {
            candidates: &'a mut Vec<(proc_macro2::Span, String, String)>,
            operator_replacements: &'a Vec<(&'a str, Vec<&'a str>)>,
        }
        impl<'a> Visit<'a> for OperatorVisitor<'a> {
            fn visit_expr(&mut self, expr: &'a syn::Expr) {
                if let syn::Expr::Binary(bin) = expr {
                    let op_str = format!("{}", bin.op);
                    for (op, replacements) in self.operator_replacements {
                        if op_str == *op {
                            if let Some(replacement) = replacements.choose(&mut thread_rng()) {
                                self.candidates.push((bin.op.span(), op_str.clone(), replacement.to_string()));
                            }
                        }
                    }
                }
                syn::visit::visit_expr(self, expr);
            }
        }
        let mut visitor = OperatorVisitor {
            candidates: &mut candidates,
            operator_replacements: &operator_replacements,
        };
        syn::visit::visit_file(&mut visitor, file);
        if let Some((span, original, replacement)) = candidates.choose(&mut rng) {
            let source_text = span.source_text().unwrap_or_default();
            if source_text == original {
                let mut token_iter = file.clone().into_token_stream().into_iter();
                let mut new_tokens = proc_macro2::TokenStream::new();
                for token in token_iter {
                    if token.span() == *span {
                        let ident = proc_macro2::Ident::new(&replacement, *span);
                        new_tokens.extend(quote!(#ident));
                    } else {
                        new_tokens.extend(quote!(#token));
                    }
                }
                *file = syn::parse2(new_tokens).unwrap_or_else(|_| file.clone());
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces single-byte ASCII operators with visually similar multi-byte Unicode characters. This transformation targets the lexer and parser's handling of multi-byte tokens, potentially causing span misalignments, diagnostic errors, or assertion failures in token recognition. By substituting operators like '=' with '⩵' or '==' with '⩵', it tests the compiler's robustness against Unicode look-alikes that are not valid Rust operators."
    }
}
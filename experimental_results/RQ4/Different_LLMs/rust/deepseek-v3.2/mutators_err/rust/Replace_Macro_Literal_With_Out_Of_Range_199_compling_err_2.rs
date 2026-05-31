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

pub struct Replace_Macro_Literal_With_Out_Of_Range_199;

impl Mutator for Replace_Macro_Literal_With_Out_Of_Range_199 {
    fn name(&self) -> &str {
        "Replace_Macro_Literal_With_Out_Of_Range_199"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MacroLiteralVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets literal expressions inside macro definitions (macro_rules!). It replaces integer, float, character, and string literals with out-of-range values based on common type inference. For integers, it uses 1000000 to overflow 16-bit contexts; for floats, 1e400 to overflow f32; for chars, a Unicode scalar beyond 0x10FFFF; for strings, a very long string. This transformation directly modifies the macro's token tree, aiming to provoke overflow errors, type mismatches, and literal parsing edge cases during macro expansion."
    }
}

struct MacroLiteralVisitor;

impl MacroLiteralVisitor {
    fn new() -> Self {
        Self
    }
}

impl VisitMut for MacroLiteralVisitor {
    fn visit_item_macro_mut(&mut self, i: &mut ItemMacro) {
        if let Some(macro_body) = &mut i.mac.body {
            let mut new_tokens = proc_macro2::TokenStream::new();
            let mut tokens = macro_body.stream().into_iter().peekable();
            while let Some(token) = tokens.next() {
                match token {
                    proc_macro2::TokenTree::Literal(lit) => {
                        let lit_str = lit.to_string();
                        let new_lit = if lit_str.starts_with('"') {
                            let long_string = "\"".to_string() + &"a".repeat(10000) + "\"";
                            proc_macro2::Literal::string(&long_string)
                        } else if lit_str.starts_with('\'') && lit_str.len() > 2 {
                            let invalid_char = '\u{10FFFF}';
                            proc_macro2::Literal::character(invalid_char)
                        } else if lit_str.contains('.') || lit_str.contains('e') || lit_str.contains('E') {
                            proc_macro2::Literal::f64_unsuffixed(1e400)
                        } else if let Ok(int_val) = lit_str.parse::<i64>() {
                            if int_val >= 0 {
                                proc_macro2::Literal::u64_unsuffixed(1000000)
                            } else {
                                proc_macro2::Literal::i64_unsuffixed(-1000000)
                            }
                        } else {
                            lit
                        };
                        new_tokens.extend(quote!(#new_lit));
                    }
                    _ => {
                        new_tokens.extend(quote!(#token));
                    }
                }
            }
            i.mac.body = Some(parse_quote!(#new_tokens));
        }
        syn::visit_mut::visit_item_macro_mut(self, i);
    }
}
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

pub struct Replace_Inline_Assembly_Instruction_171;

impl Mutator for Replace_Inline_Assembly_Instruction_171 {
    fn name(&self) -> &str {
        "Replace_Inline_Assembly_Instruction_171"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AsmVisitor;
        impl VisitMut for AsmVisitor {
            fn visit_expr_macro(&mut self, expr: &mut ExprMacro) {
                if let Some(ident) = expr.path.get_ident() {
                    if ident == "asm" || ident == "global_asm" {
                        if let Ok(parsed_args) = syn::parse2::<Punctuated<Expr, Comma>>(expr.tokens.clone()) {
                            if let Some(first_expr) = parsed_args.first() {
                                if let Expr::Lit(expr_lit) = first_expr {
                                    if let syn::Lit::Str(lit_str) = expr_lit.lit {
                                        let original_str = lit_str.value();
                                        let modified_str = replace_instruction(&original_str);
                                        let new_lit_str = syn::LitStr::new(&modified_str, lit_str.span());
                                        let new_first_expr = syn::Expr::Lit(syn::ExprLit {
                                            attrs: Vec::new(),
                                            lit: syn::Lit::Str(new_lit_str),
                                            expr_token: expr_lit.expr_token,
                                        });
                                        let rest_args: Vec<&Expr> = parsed_args.iter().skip(1).collect();
                                        let new_tokens = quote! {
                                            #new_first_expr, #(#rest_args),*
                                        };
                                        expr.tokens = new_tokens;
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_macro(self, expr);
            }
        }
        let mut visitor = AsmVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly blocks (`asm!` or `global_asm!`) and replaces a valid assembly instruction with an invalid mnemonic by prefixing it with `IN_`. This transformation introduces syntactically invalid assembly instructions, potentially exposing compiler errors in inline assembly parsing or error handling logic."
    }
}

fn replace_instruction(s: &str) -> String {
    let re = Regex::new(r"\b(\w+)\b").unwrap();
    re.replace(s, "IN_$1").to_string()
}
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

pub struct Fix_Unclosed_Char_Literal_In_Generics_233;

impl Mutator for Fix_Unclosed_Char_Literal_In_Generics_233 {
    fn name(&self) -> &str {
        "Fix_Unclosed_Char_Literal_In_Generics_233"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = CharLiteralFixer::new();
        visitor.visit_file_m2ut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unclosed character literals within generic argument contexts (angle brackets). It identifies char literal tokens that are missing a closing single quote and are immediately followed by a delimiter (comma, greater-than, etc.). The mutation inserts a closing single quote, fixing the syntax. This transformation tests the compiler's resilience to malformed literals in generic contexts and may trigger parsing or type resolution edge cases."
    }
}

struct CharLiteralFixer;

impl CharLiteralFixer {
    fn new() -> Self {
        Self
    }
}

impl VisitMut for CharLiteralFixer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Lit(expr_lit) = expr {
            if let syn::Lit::Char(lit_char) = &expr_lit.lit {
                let span = lit_char.span();
                let token_str = lit_char.token().to_string();
                if !token_str.ends_with('\'') {
                    let fixed_token = token_str.trim_end_matches('\'').to_string() + "'";
                    let new_lit = syn::Lit::Char(syn::LitChar::new(fixed_token.chars().next().unwrap_or('\''), span));
                    expr_lit.lit = new_lit;
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            for segment in &mut type_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(Expr::Lit(expr_lit)) = arg {
                            if let syn::Lit::Char(lit_char) = &expr_lit.lit {
                                let span = lit_char.span();
                                let token_str = lit_char.token().to_string();
                                if !token_str.ends_with('\'') {
                                    let fixed_token = token_str.trim_end_matches('\'').to_string() + "'";
                                    let new_lit = syn::Lit::Char(syn::LitChar::new(fixed_token.chars().next().unwrap_or('\''), span));
                                    expr_lit.lit = new_lit;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_generic_argument_mut(&mut self, arg: &mut GenericArgument) {
        if let GenericArgument::Const(Expr::Lit(expr_lit)) = arg {
            if let syn::Lit::Char(lit_char) = &expr_lit.lit {
                let span = lit_char.span();
                let token_str = lit_char.token().to_string();
                if !token_str.ends_with('\'') {
                    let fixed_token = token_str.trim_end_matches('\'').to_string() + "'";
                    let new_lit = syn::Lit::Char(syn::LitChar::new(fixed_token.chars().next().unwrap_or('\''), span));
                    expr_lit.lit = new_lit;
                }
            }
        }
        syn::visit_mut::visit_generic_argument_mut(self, arg);
    }
}
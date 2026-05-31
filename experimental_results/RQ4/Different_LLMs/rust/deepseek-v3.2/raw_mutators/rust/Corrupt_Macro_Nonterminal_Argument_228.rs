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

pub struct Corrupt_Macro_Nonterminal_Argument_228;

impl Mutator for Corrupt_Macro_Nonterminal_Argument_228 {
    fn name(&self) -> &str {
        "Corrupt_Macro_Nonterminal_Argument_228"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MacroNonterminalVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations with nonterminal matchers (e.g., $:stmt, $:expr, $:ty). It replaces the argument token tree with a syntactically invalid sequence that starts with a token that could begin the expected nonterminal but then deviates into malformed syntax. For stmt matchers, it uses 'let ;' or 'let'. For expr matchers, it uses 'x +'. For ty matchers, it uses 'Vec<'. This aims to trigger parser error recovery or missing token handling within the macro expansion's nonterminal parsing, potentially exposing edge cases in the compiler's macro argument parsing and recovery mechanisms."
    }
}

struct MacroNonterminalVisitor;

impl VisitMut for MacroNonterminalVisitor {
    fn visit_expr_macro_mut(&mut self, node: &mut syn::ExprMacro) {
        self.process_macro_invocation(&node.mac.path, &mut node.mac.tokens);
        syn::visit_mut::visit_expr_macro_mut(self, node);
    }

    fn visit_item_macro_mut(&mut self, node: &mut syn::ItemMacro) {
        if let Some(ref mut mac) = node.mac {
            self.process_macro_invocation(&mac.path, &mut mac.tokens);
        }
        syn::visit_mut::visit_item_macro_mut(self, node);
    }
}

impl MacroNonterminalVisitor {
    fn process_macro_invocation(&self, path: &syn::Path, tokens: &mut proc_macro2::TokenStream) {
        let token_string = tokens.to_string();
        if token_string.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let nonterminal_type = rng.gen_range(0..3);
        let replacement = match nonterminal_type {
            0 => {
                let variant = rng.gen_range(0..2);
                match variant {
                    0 => quote! { let ; },
                    1 => quote! { let },
                    _ => quote! { let ; },
                }
            }
            1 => quote! { x + },
            2 => quote! { Vec < },
            _ => quote! { let ; },
        };

        *tokens = replacement.into();
    }
}
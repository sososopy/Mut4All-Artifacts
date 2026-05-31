use proc_macro2::{Span, Delimiter, Literal, Punct, Spacing, TokenTree};
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

pub struct Asm_Const_Division_By_Zero_175;

impl Mutator for Asm_Const_Division_By_Zero_175 {
    fn name(&self) -> &str {
        "Asm_Const_Division_By_Zero_175"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if let Some(macro_path) = item_macro.mac.path.segments.last() {
                    if macro_path.ident == "asm" || macro_path.ident == "global_asm" {
                        let mut tokens = item_macro.mac.tokens.clone();
                        let mut new_tokens = Vec::new();
                        let mut in_const = false;
                        let mut const_start = 0;
                        let mut const_end = 0;
                        let mut token_iter = tokens.into_iter().peekable();
                        
                        while let Some(token) = token_iter.next() {
                            let token_clone = token.clone();
                            new_tokens.push(token);
                            if token_clone.to_string() == "const" {
                                in_const = true;
                                const_start = new_tokens.len() - 1;
                                let mut expr_tokens = Vec::new();
                                while let Some(next_token) = token_iter.peek() {
                                    if next_token.to_string() == "," || next_token.to_string() == ")" {
                                        break;
                                    }
                                    expr_tokens.push(token_iter.next().unwrap());
                                }
                                const_end = new_tokens.len() + expr_tokens.len();
                                let expr_tokens_clone = expr_tokens.clone();
                                new_tokens.extend(expr_tokens);
                                
                                if expr_tokens_clone.len() > 0 {
                                    let expr_str = expr_tokens_clone.iter().map(|t| t.to_string()).collect::<String>();
                                    if expr_str.trim().chars().all(|c| c.is_digit(10)) {
                                        new_tokens.push(TokenTree::Punct(Punct::new('/', Spacing::Alone)));
                                        new_tokens.push(TokenTree::Literal(Literal::i32_unsuffixed(0)));
                                    } else {
                                        new_tokens.insert(const_start + 1, TokenTree::Group(proc_macro2::Group::new(Delimiter::Parenthesis, proc_macro2::TokenStream::new())));
                                        new_tokens.push(TokenTree::Punct(Punct::new('/', Spacing::Alone)));
                                        new_tokens.push(TokenTree::Literal(Literal::i32_unsuffixed(0)));
                                    }
                                }
                                in_const = false;
                            }
                        }
                        item_macro.mac.tokens = proc_macro2::TokenStream::from_iter(new_tokens);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions used as operands in inline assembly (`asm!` or `global_asm!`) within Rust programs that use the `asm_const` feature. It modifies a constant operand to introduce a division by zero, which forces a compile-time evaluation failure. The operator applies only to constant operands that are integer literals or simple arithmetic expressions involving integer literals. For each eligible constant operand `const expr` in an `asm!` or `global_asm!` macro, replace `expr` with `expr / 0` if `expr` is an integer literal, or wrap the existing `expr` as `(expr) / 0` if it is a more complex expression. This mutation is designed to test the compiler's ability to handle erroneous constants in assembly contexts without crashing."
    }
}
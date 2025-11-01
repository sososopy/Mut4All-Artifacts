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
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Declaration_With_Unicode_Character_357;

impl Mutator for Modify_Const_Declaration_With_Unicode_Character_357 {
    fn name(&self) -> &str {
        "Modify_Const_Declaration_With_Unicode_Character_357"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                let const_value = &item_const.expr;
                let mut rng = thread_rng();
                let unicode_replacements = vec![
                    ('=', '⩵'),
                    ('+', '＋'),
                    ('-', '−'),
                    ('*', '∗'),
                    ('/', '∕')
                ];
                
                let mut mutated_expr = quote! { #const_value }.to_string();
                if let Some((orig, replacement)) = unicode_replacements.choose(&mut rng) {
                    mutated_expr = mutated_expr.replace(*orig, &replacement.to_string());
                }
                
                let new_expr: Expr = parse_str(&mutated_expr).unwrap_or_else(|_| const_value.clone());
                item_const.expr = Box::new(new_expr);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets constant declarations in Rust code. It identifies constant declarations and replaces a valid operator or symbol with a visually similar Unicode character that is not a valid operator in Rust. This can lead to parsing errors or internal compiler errors, as the mutated code will not be syntactically correct. The operator or symbol is chosen randomly from a predefined list of replacements, ensuring a broad application of the mutation across different constant declarations."
    }
}
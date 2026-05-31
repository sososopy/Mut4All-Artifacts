use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use std::{collections::HashSet, fs, path::Path};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, Expr, File, Item, Stmt,
};

use crate::mutator::Mutator;

pub struct Attribute_Misplacement_25;

impl Mutator for Attribute_Misplacement_25 {
    fn name(&self) -> &str {
        "Attribute_Misplacement_25"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let attributes: Vec<syn::Attribute> = vec![
            parse_quote!(#[cfg()]),
            parse_quote!(#[allow()]),
            parse_quote!(#[deny()]),
        ];

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if rng.gen_bool(0.5) {
                            let attr = attributes.choose(&mut rng).unwrap().clone();
                            local.attrs.push(attr);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces randomly selected attributes like `#[cfg()]`, `#[allow()]`, or `#[deny()]` to local variable declarations within functions. This tests the compiler's handling of misplaced or improperly configured attributes, potentially leading to warnings or errors due to attribute misplacement."
    }
}
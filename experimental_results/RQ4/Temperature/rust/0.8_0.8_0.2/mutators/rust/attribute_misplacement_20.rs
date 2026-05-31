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

pub struct Attribute_Misplacement_20;

impl Mutator for Attribute_Misplacement_20 {
    fn name(&self) -> &str {
        "Attribute_Misplacement_20"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(attr) = func.attrs.first() {
                    let original_attr = attr.clone();
                    func.attrs.remove(0);

                    match rng.gen_range(0..3) {
                        0 => {
                            // Move the attribute inside the function body at a random position
                            let mut statements = func.block.stmts.clone();
                            if !statements.is_empty() {
                                let index = rng.gen_range(0..statements.len());
                                statements.insert(index, Stmt::Item(syn::Item::Verbatim(quote! { #original_attr })));
                                func.block.stmts = statements;
                            }
                        }
                        1 => {
                            // Remove the attribute entirely
                            // Already removed above by not adding it back
                        }
                        2 => {
                            // Replace with a nonsensical attribute
                            func.attrs.push(parse_quote!(#[nonsense_attribute]));
                        }
                        _ => {}
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function declarations with attributes that affect lints or warnings. It moves or modifies attributes to expose issues related to attribute processing. By moving the attribute inside the function body, removing it entirely, or replacing it with a nonsensical one, the mutation tests the compiler's handling of misplaced or incorrect attributes."
    }
}
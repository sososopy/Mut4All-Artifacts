use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    *,
};
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

pub struct Mutator_Corrupt_Target_Clause_26;

impl Mutator for Mutator_Corrupt_Target_Clause_26 {
    fn name(&self) -> &str {
        "Mutator_Corrupt_Target_Clause_26"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut collector = IdentCollector { idents: Vec::new() };
        collector.visit_file(file);
        let idents = collector.idents;

        for item in &mut file.items {
            match item {
                Item::Fn(func) => {
                    for attr in &mut func.attrs {
                        if let syn::Meta::List(list) = &mut attr.meta {
                            process_cfg_attr(list, &idents);
                        }
                    }
                }
                Item::Struct(struc) => {
                    for attr in &mut struc.attrs {
                        if let syn::Meta::List(list) = &mut attr.meta {
                            process_cfg_attr(list, &idents);
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets #[cfg(target(...))] attributes with valid key-value pairs and replaces them with two arbitrary expressions (variables, literals, or constants from the program). This transforms the target clause into an invalid syntax form (target(X, Y)), potentially triggering ICEs in attribute parsing logic by violating expected key-value syntax."
    }
}

struct IdentCollector {
    idents: Vec<Ident>,
}

impl<'ast> Visit<'ast> for IdentCollector {
    fn visit_ident(&mut self, i: &'ast Ident) {
        self.idents.push(i.clone());
    }
}

fn process_cfg_attr(list: &mut syn::MetaList, idents: &[Ident]) {
    if list.path.is_ident("cfg") {
        for i in (0..list.next().len()).rev() {
            if let syn::Meta::List(target_list) = &list.next()[i] {
                if target_list.path.is_ident("target") {
                    for j in (0..target_list.next().len()).rev() {
                        if let syn::Meta::NameValue(name_value) = &target_list.next()[j] {
                            let mut rng = thread_rng();
                            let expr1 = idents.choose(&mut rng).unwrap().clone();
                            let expr2 = idents.choose(&mut rng).unwrap().clone();

                            let meta1 = syn::Meta::Path(syn::Path::from(expr1));
                            let meta2 = syn::Meta::Path(syn::Path::from(expr2));

                            let mut nested = target_list.next_mut();
                            nested.remove(j);
                            nested.insert(j, meta1);
                            nested.insert(j + 1, meta2);
                            break;
                        }
                    }
                }
            }
        }
    }
}
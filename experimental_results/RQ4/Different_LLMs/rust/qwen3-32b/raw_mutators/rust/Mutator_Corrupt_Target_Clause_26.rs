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
            for attr in &mut item.attrs {
                if let syn::Attribute::Normal(normal) = attr {
                    for nested in &mut normal.meta.nested {
                        if let syn::NestedMeta::Meta(meta) = nested {
                            if let syn::Meta::List(list) = meta {
                                if list.path.is_ident("cfg") {
                                    process_cfg_attr(list, &idents);
                                }
                            }
                        }
                    }
                }
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
    for nested in &mut list.nested {
        if let syn::NestedMeta::Meta(meta) = nested {
            if let syn::Meta::List(target_list) = meta {
                if target_list.path.is_ident("target") {
                    for i in (0..target_list.nested.len()).rev() {
                        if let syn::NestedMeta::Meta(nested_meta) = &target_list.nested[i] {
                            if let syn::Meta::NameValue(name_value) = nested_meta {
                                let mut rng = thread_rng();
                                let expr1 = idents.choose(&mut rng).unwrap().clone();
                                let expr2 = idents.choose(&mut rng).unwrap().clone();

                                let meta1 = syn::Meta::Word(expr1);
                                let meta2 = syn::Meta::Word(expr2);

                                target_list.nested.remove(i);
                                target_list.nested.insert(i, syn::NestedMeta::Meta(meta1));
                                target_list.nested.insert(i + 1, syn::NestedMeta::Meta(meta2));
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}
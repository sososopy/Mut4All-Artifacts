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

pub struct Corrupt_Proc_Macro_Format_String_252;

impl Mutator for Corrupt_Proc_Macro_Format_String_252 {
    fn name(&self) -> &str {
        "Corrupt_Proc_Macro_Format_String_252"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = CorruptProcMacroVisitor {
            rng: thread_rng(),
            identifiers: HashSet::new(),
        };
        visitor.collect_identifiers(file);
        visitor.mutate_file(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets proc-macro attributes that accept format strings, such as #[error(...)] or #[derive(Debug)] with custom display. It modifies string literal arguments by inserting invalid UTF-8 sequences or non-ASCII characters within placeholder braces, corrupting format string parsing while maintaining valid Rust syntax. This aims to trigger internal compiler errors in format argument expansion, particularly byte-to-character position miscalculations during diagnostic emission."
    }
}

struct CorruptProcMacroVisitor {
    rng: rand::rngs::ThreadRng,
    identifiers: HashSet<String>,
}

impl CorruptProcMacroVisitor {
    fn collect_identifiers(&mut self, file: &syn::File) {
        struct IdentifierCollector<'a> {
            identifiers: &'a mut HashSet<String>,
        }
        impl<'a> Visit<'a> for IdentifierCollector<'a> {
            fn visit_ident(&mut self, ident: &'a Ident) {
                self.identifiers.insert(ident.to_string());
            }
        }
        let mut collector = IdentifierCollector {
            identifiers: &mut self.identifiers,
        };
        collector.visit_file(file);
    }

    fn mutate_file(&mut self, file: &mut syn::File) {
        struct MutatingVisitor<'a, 'b> {
            visitor: &'a mut CorruptProcMacroVisitor,
            file: &'b mut syn::File,
        }
        impl<'a, 'b> VisitMut for MutatingVisitor<'a, 'b> {
            fn visit_attribute_mut(&mut self, attr: &mut syn::Attribute) {
                if self.visitor.is_target_attribute(&attr) {
                    self.visitor.mutate_attribute(attr);
                }
                syn::visit_mut::visit_attribute_mut(self, attr);
            }
        }
        let mut mutating_visitor = MutatingVisitor {
            visitor: self,
            file,
        };
        mutating_visitor.visit_file_mut(&mut mutating_visitor.file);
    }

    fn is_target_attribute(&self, attr: &syn::Attribute) -> bool {
        let path_str = attr.path().segments.iter().map(|seg| seg.ident.to_string()).collect::<Vec<_>>().join("::");
        path_str.contains("error") || path_str.contains("display") || attr.path().segments.iter().any(|seg| seg.ident == "derive")
    }

    fn mutate_attribute(&mut self, attr: &mut syn::Attribute) {
        if let syn::Meta::List(list) = &mut attr.meta {
            let mut tokens = list.tokens.clone();
            let mut new_tokens = proc_macro2::TokenStream::new();
            for tree in tokens.clone().into_iter() {
                match tree {
                    proc_macro2::TokenTree::Literal(lit) => {
                        if let Ok(str_lit) = syn::parse2::<syn::LitStr>(proc_macro2::TokenStream::from(proc_macro2::TokenTree::Literal(lit.clone()))) {
                            let mutated = self.mutate_string_literal(&str_lit);
                            new_tokens.extend(proc_macro2::TokenStream::from(proc_macro2::TokenTree::Literal(proc_macro2::Literal::string(&mutated))));
                        } else {
                            new_tokens.extend(proc_macro2::TokenStream::from(tree));
                        }
                    }
                    _ => new_tokens.extend(proc_macro2::TokenStream::from(tree)),
                }
            }
            list.tokens = new_tokens;
        }
    }

    fn mutate_string_literal(&mut self, lit: &syn::LitStr) -> String {
        let original = lit.value();
        let pattern = Regex::new(r"\{([^\}]*)\}").unwrap();
        let mut result = original.clone();
        if pattern.is_match(&original) {
            for cap in pattern.captures_iter(&original) {
                let placeholder = cap.get(1).unwrap().as_str();
                if self.identifiers.contains(placeholder) {
                    let corrupted = self.corrupt_placeholder(placeholder);
                    result = result.replace(&format!("{{{}}}", placeholder), &format!("{{{}}}", corrupted));
                }
            }
        } else {
            let identifiers_clone = self.identifiers.clone();
            if let Some(ident) = identifiers_clone.iter().next() {
                let corrupted = self.corrupt_placeholder(ident);
                result = format!("{{{}}}", corrupted);
            }
        }
        result
    }

    fn corrupt_placeholder(&mut self, placeholder: &str) -> String {
        let corruption = if self.rng.gen_bool(0.5) {
            format!("{}\x80", placeholder)
        } else {
            format!("{}ㅇ", placeholder)
        };
        corruption
    }
}
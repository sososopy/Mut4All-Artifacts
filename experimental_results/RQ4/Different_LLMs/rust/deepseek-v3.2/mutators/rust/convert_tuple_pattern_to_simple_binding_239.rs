use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprField, ExprMethodCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Convert_Tuple_Pattern_To_Simple_Binding_239;

impl Mutator for Convert_Tuple_Pattern_To_Simple_Binding_239 {
    fn name(&self) -> &str {
        "Convert_Tuple_Pattern_To_Simple_Binding_239"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = TuplePatternVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets tuple patterns in let statements where the right-hand side is a field access or method call that likely returns a non-tuple type. It converts the tuple pattern to a simple identifier binding, selecting the first named binding from the pattern or generating a fresh name. This transformation aims to trigger type mismatches, pattern matching errors, and internal compiler errors related to destructuring non-tuple types, particularly around moves, borrows, and packed structs."
    }
}

struct TuplePatternVisitor;

impl TuplePatternVisitor {
    fn new() -> Self {
        Self
    }

    fn is_field_or_method_call(&self, expr: &Expr) -> bool {
        matches!(expr, Expr::Field(_) | Expr::MethodCall(_))
    }

    fn extract_first_named_binding(&self, pat: &Pat) -> Option<Ident> {
        match pat {
            Pat::Tuple(tuple) => {
                for elem in &tuple.elems {
                    if let Pat::Ident(pat_ident) = elem {
                        if pat_ident.ident != "_" {
                            return Some(pat_ident.ident.clone());
                        }
                    }
                }
                None
            }
            _ => None,
        }
    }

    fn generate_fresh_name(&self, existing_names: &HashSet<String>) -> Ident {
        let mut rng = thread_rng();
        let base_names = ["bound_val", "temp", "value", "result", "item"];
        for name in base_names.iter() {
            if !existing_names.contains(*name) {
                return Ident::new(name, Span::call_site());
            }
        }
        let mut counter = 0;
        loop {
            let candidate = format!("bound_val_{}", counter);
            if !existing_names.contains(&candidate) {
                return Ident::new(&candidate, Span::call_site());
            }
            counter += 1;
        }
    }

    fn collect_existing_idents(&self, file: &syn::File) -> HashSet<String> {
        let mut collector = IdentCollector::new();
        collector.visit_file(file);
        collector.idents
    }
}

impl VisitMut for TuplePatternVisitor {
    fn visit_local_mut(&mut self, local: &mut Local) {
        if let Some(init) = &mut local.init {
            let pat = &mut local.pat;
            let expr = &init.expr;
            if self.is_field_or_method_call(expr) {
                if let Pat::Tuple(_) = &*pat {
                    let existing_idents = self.collect_existing_idents(&syn::File {
                        shebang: None,
                        attrs: vec![],
                        items: vec![],
                    });
                    let new_ident = self.extract_first_named_binding(pat)
                        .unwrap_or_else(|| self.generate_fresh_name(&existing_idents));
                    *pat = Pat::Ident(PatIdent {
                        attrs: vec![],
                        by_ref: None,
                        mutability: None,
                        ident: new_ident,
                        subpat: None,
                    });
                }
            }
        }
        visit_mut::visit_local_mut(self, local);
    }
}

struct IdentCollector {
    idents: HashSet<String>,
}

impl IdentCollector {
    fn new() -> Self {
        Self {
            idents: HashSet::new(),
        }
    }
}

impl Visit<'_> for IdentCollector {
    fn visit_ident(&mut self, ident: &Ident) {
        self.idents.insert(ident.to_string());
        visit::visit_ident(self, ident);
    }

    fn visit_pat_ident(&mut self, pat_ident: &PatIdent) {
        self.idents.insert(pat_ident.ident.to_string());
        visit::visit_pat_ident(self, pat_ident);
    }
}
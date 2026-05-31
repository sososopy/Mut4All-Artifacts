use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprClosure, ExprBlock, ExprCall, ExprPath, File,
    FnArg, GenericArgument, GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam,
    Local, Pat, PatType, Path as SynPath, PathArguments, ReturnType, TraitBound,
    TraitBoundModifier, Type, TypeImplTrait, TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated, spanned::Spanned, token, token::Comma, token::{Paren, Plus},
    visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Replace_Closure_Capture_With_UndefinedVar_11;

impl Mutator for Replace_Closure_Capture_With_UndefinedVar_11 {
    fn name(&self) -> &str {
        "Replace_Closure_Capture_With_UndefinedVar_11"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                // Collect all variables declared in the outer function
                let outer_vars = collect_outer_function_vars(item_fn);
                // Traverse the function's body for closures
                let mut closure_collector = ClosureCollector::new();
                closure_collector.visit_block_mut(&mut item_fn.block);
                for closure in closure_collector.closures {
                    process_closure(closure, &outer_vars);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        ""
    }
}

// Helper to collect all closures in a function's body
struct ClosureCollector<'a> {
    closures: Vec<&'a mut ExprClosure>,
}

impl<'a> ClosureCollector<'a> {
    fn new() -> Self {
        Self { closures: Vec::new() }
    }
}

impl<'a> VisitMut for ClosureCollector<'a> {
    fn visit_expr_closure_mut(&mut self, expr: &'a mut ExprClosure) {
        self.closures.push(expr);
        visit_mut::visit_expr_closure_mut(self, expr);
    }
}

// Helper to collect variables declared in a function's body
fn collect_outer_function_vars(item_fn: &ItemFn) -> HashSet<Ident> {
    let mut vars = HashSet::new();
    let mut visitor = LetVisitor::new();
    visitor.visit_block(&item_fn.block);
    vars.extend(visitor.idents);
    vars
}

// Helper to collect variables declared in a closure's scope
fn collect_declared_vars_in_closure(closure: &ExprClosure) -> HashSet<Ident> {
    let mut vars = HashSet::new();
    // Collect variables from closure parameters
    for arg in &closure.fn_decl.inputs {
        if let FnArg::Typed(pat_type) = arg {
            let mut pattern_visitor = PatternVisitor::new();
            pattern_visitor.visit_pat(&pat_type.pat);
            vars.extend(pattern_visitor.idents);
        }
    }
    // Collect variables from closure body
    if let ExprClosureBody::Block(block) = &closure.body {
        let mut let_visitor = LetVisitor::new();
        let_visitor.visit_block(block);
        vars.extend(let_visitor.idents);
    }
    vars
}

// Helper to collect variables used in a closure's body
fn collect_used_vars_in_closure(closure: &ExprClosure) -> HashSet<Ident> {
    let mut vars = HashSet::new();
    if let ExprClosureBody::Block(block) = &closure.body {
        let mut expr_visitor = ExprVisitor::new();
        expr_visitor.visit_block(block);
        vars.extend(expr_visitor.idents);
    }
    vars
}

// Helper to process a closure and replace a variable
fn process_closure(closure: &mut ExprClosure, outer_vars: &HashSet<Ident>) {
    let declared = collect_declared_vars_in_closure(closure);
    let used = collect_used_vars_in_closure(closure);
    let captured: Vec<_> = used
        .into_iter()
        .filter(|var| !declared.contains(var))
        .collect();
    if captured.is_empty() {
        return;
    }
    // Select a random captured variable
    let mut rng = thread_rng();
    let var_to_replace = captured.choose(&mut rng).unwrap().clone();
    // Generate a new variable name not in outer scope
    let new_var = generate_new_var_name(outer_vars);
    // Replace all occurrences
    replace_variable_in_closure(closure, &var_to_replace, new_var);
}

// Generate a new variable name not in outer_vars
fn generate_new_var_name(outer_vars: &HashSet<Ident>) -> Ident {
    let base_name = "undefined_var";
    let mut i = 1;
    loop {
        let name = format!("{}{}", base_name, i);
        let ident = Ident::new(&name, Span::call_site());
        if !outer_vars.contains(&ident) {
            return ident;
        }
        i += 1;
    }
}

// Replace all occurrences of var_to_replace with new_var in closure
fn replace_variable_in_closure(closure: &mut ExprClosure, var_to_replace: &Ident, new_var: Ident) {
    match &mut closure.body {
        ExprClosureBody::Block(block) => {
            for stmt in &mut block.stmts {
                if let Stmt::Expr(expr, _) = stmt {
                    let mut replacer = VariableReplacer::new(var_to_replace.clone(), new_var.clone());
                    replacer.visit_expr_mut(expr);
                }
            }
        }
        ExprClosureBody::Tail(expr) => {
            let mut replacer = VariableReplacer::new(var_to_replace.clone(), new_var.clone());
            replacer.visit_expr_mut(expr);
        }
    }
}

// Helper to collect variables from let statements
struct LetVisitor {
    idents: HashSet<Ident>,
}

impl LetVisitor {
    fn new() -> Self {
        Self { idents: HashSet::new() }
    }
}

impl Visit for LetVisitor {
    fn visit_local(&mut self, local: &Local) {
        let mut pattern_visitor = PatternVisitor::new();
        pattern_visitor.visit_pat(&local.pat);
        self.idents.extend(pattern_visitor.idents);
    }
}

// Helper to collect identifiers from patterns
struct PatternVisitor {
    idents: HashSet<Ident>,
}

impl PatternVisitor {
    fn new() -> Self {
        Self { idents: HashSet::new() }
    }
}

impl Visit for PatternVisitor {
    fn visit_ident(&mut self, ident: &Ident) {
        self.idents.insert(ident.clone());
    }
}

// Helper to collect variables used in expressions
struct ExprVisitor {
    idents: HashSet<Ident>,
}

impl ExprVisitor {
    fn new() -> Self {
        Self { idents: HashSet::new() }
    }
}

impl Visit for ExprVisitor {
    fn visit_expr_path(&mut self, expr: &ExprPath) {
        if expr.path.segments.len() == 1 {
            if let Some(ident) = expr.path.get_ident() {
                self.idents.insert(ident.clone());
            }
        }
    }
}

// Helper to replace variables in expressions
struct VariableReplacer {
    var_to_replace: Ident,
    new_var: Ident,
}

impl VariableReplacer {
    fn new(var_to_replace: Ident, new_var: Ident) -> Self {
        Self {
            var_to_replace,
            new_var,
        }
    }
}

impl VisitMut for VariableReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(expr_path) = expr {
            if expr_path.path.segments.len() == 1 {
                if let Some(ident) = expr_path.path.get_ident() {
                    if ident == &self.var_to_replace {
                        let new_path = syn::parse2(quote! { #self.new_var }).unwrap();
                        *expr = new_path;
                    }
                }
            }
        }
        visit_mut::visit_expr_mut(self, expr);
    }
}